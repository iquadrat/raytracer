/* *************************************************************************
                          noise.cpp  -  description
                             -------------------
    begin                : Die Jul 15 2003
    copyright            : (C) 2003 by Micha Riser
    email                : micha@povworld.org
    
    $Id: noise.cpp,v 1.3 2003/07/21 23:41:51 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#include "../src/noise.h"
#include "/home/micha/download/iaca-lin64/include/iacaMarks.h"

//#define PERLIN_NOISE_AVX
//#define PERLIN_NOISE_VGATHER
//#define PAPI

#include <x86intrin.h>

#ifdef PERLIN_NOISE_AVX
#include <immintrin.h>
#endif

namespace {

#ifdef PERLIN_NOISE_AVX
static __m256d dotProd4(
           __m256d v0, __m256d w0,
           __m256d v1, __m256d w1,
           __m256d v2, __m256d w2,
           __m256d v3, __m256d w3
           ) {
           __m256d m0 = _mm256_mul_pd(v0, w0);
           __m256d m1 = _mm256_mul_pd(v1, w1);
           __m256d m2 = _mm256_mul_pd(v2, w2);
           __m256d m3 = _mm256_mul_pd(v3, w3);

           // m00+m01 m10+m11 m02+m03 m12+m13
           __m256d temp01 = _mm256_hadd_pd( m0, m1 );
           // m20+m21 m30+m31 m22+m23 m32+m33
           __m256d temp23 = _mm256_hadd_pd( m2, m3 );

           // low to high: m02+m03 m12+m13 m20+m21 m30+m31
           __m256d swapped = _mm256_permute2f128_pd( temp01, temp23, 0x21 );

           // low to high: m00+m01 m10+m11 m22+m23 m32+m33
           __m256d blended = _mm256_blend_pd(temp01, temp23, 0b1100);

           return _mm256_add_pd( swapped, blended );
       }

  __m256d load(const Vector3& v) {
      //return _mm256_blend_pd(_mm256_load_pd((double*)&v), _mm256_setzero_pd(), 0b1000);
      return _mm256_load_pd((double*)&v);
      //return _mm256_set_pd(0, v[2], v[1], v[0]);
  }

#ifdef PERLIN_NOISE_VGATHER
  __m256d loadX(const Vector3* tbl, unsigned long long offset) {
        //return _mm256_blend_pd(_mm256_load_pd((double*)&v), _mm256_setzero_pd(), 0b1000);
        double *dptr = (double*)tbl;
        return _mm256_load_pd(&dptr[offset]);
        //return _mm256_set_pd(0, v[2], v[1], v[0]);
    }
#endif

#endif
}

#include <papi.h>
#include <stdio.h>

DBL PerlinNoiseCommon::evaluate(const Vector3& point) const {
#ifdef PAPI
    int events[] = { PAPI_L1_DCM, PAPI_LD_INS };
    PAPI_start_counters(events, 2);
#endif
//    IACA_START
    int gridPtLDB[4];

#ifndef PERLIN_NOISE_AVX
    DBL distLDB[4];
    DBL distRUF[4];
    DBL s[4];

    setupValues(point[0], gridPtLDB[0], distLDB[0], distRUF[0]);
    setupValues(point[1], gridPtLDB[1], distLDB[1], distRUF[1]);
    setupValues(point[2], gridPtLDB[2], distLDB[2], distRUF[2]);

    s[0] = sCurve(distLDB[0]);
    s[1] = sCurve(distLDB[1]);
    s[2] = sCurve(distLDB[2]);
#else
    // TODO try _mm256_mask_load_pd
    //__m256d p = _mm256_set_pd(0, point[2], point[1], point[0]);
    //__m256d p = point._v;
    //__m256d p =  _mm256_load_pd((double*)&point);
    //__m256d p = _mm256_blend_pd(_mm256_loadu_pd((double*)&point), _mm256_setzero_pd(), 0b1000);
    __m256d p = load(point);

    // setupValues
    __m256d intpart = _mm256_floor_pd(p);
    __m128i pi = _mm256_cvtpd_epi32(p);

    __m256d distLDB = _mm256_sub_pd(p, intpart);
    __m256d distRUF = _mm256_sub_pd(distLDB, _mm256_set1_pd(1.0));
    __m128i v_gridPtLDB = _mm_and_si128(pi, _mm_set1_epi32(255));
#ifdef PERLIN_NOISE_VGATHER
    __m128i v_gridPtLDBp1 = _mm_add_epi32(v_gridPtLDB, _mm_set1_epi32(1));
#else
    _mm_store_si128((__m128i *) gridPtLDB, v_gridPtLDB);
#endif

    // sCurve: (6*v-15)*v+10)*pow3(v)
    __m256d v_s = _mm256_mul_pd(distLDB, _mm256_set1_pd(6.0));
    v_s = _mm256_sub_pd(v_s, _mm256_set1_pd(15.0));
    v_s = _mm256_mul_pd(v_s, distLDB);
    v_s = _mm256_add_pd(v_s, _mm256_set1_pd(10.0));
    v_s = _mm256_mul_pd(v_s, distLDB);
    v_s = _mm256_mul_pd(v_s, distLDB);
    v_s = _mm256_mul_pd(v_s, distLDB);

#endif

#ifndef PERLIN_NOISE_VGATHER
    int idxL = permutationTbl[gridPtLDB[0]];
    int idxR = permutationTbl[gridPtLDB[0] + 1];

    int idxLD = permutationTbl[idxL + gridPtLDB[1]];
    int idxRD = permutationTbl[idxR + gridPtLDB[1]];
    int idxLU = permutationTbl[idxL + gridPtLDB[1] + 1];
    int idxRU = permutationTbl[idxR + gridPtLDB[1] + 1];

#ifndef PERLIN_NOISE_AVX
    DBL u1 = Vector3::dotProd( gradTbl[idxLD + gridPtLDB[2]], Vector3(distLDB[0], distLDB[1], distLDB[2]) );
    DBL u2 = Vector3::dotProd( gradTbl[idxLU + gridPtLDB[2]], Vector3(distLDB[0], distRUF[1], distLDB[2]) );
    DBL u3 = Vector3::dotProd( gradTbl[idxLD + gridPtLDB[2] + 1], Vector3(distLDB[0], distLDB[1], distRUF[2]) );
    DBL u4 = Vector3::dotProd( gradTbl[idxLU + gridPtLDB[2] + 1], Vector3(distLDB[0], distRUF[1], distRUF[2]) );

    DBL v1 = Vector3::dotProd( gradTbl[idxRD + gridPtLDB[2]], Vector3(distRUF[0], distLDB[1], distLDB[2]) );
    DBL v2 = Vector3::dotProd( gradTbl[idxRU + gridPtLDB[2]], Vector3(distRUF[0], distRUF[1], distLDB[2]) );
    DBL v3 = Vector3::dotProd( gradTbl[idxRD + gridPtLDB[2] + 1], Vector3(distRUF[0], distLDB[1], distRUF[2]) );
    DBL v4 = Vector3::dotProd( gradTbl[idxRU + gridPtLDB[2] + 1], Vector3(distRUF[0], distRUF[1], distRUF[2]) );
#else // PERLIN_NOISE_AVX
    __m256d u1324 = dotProd4(
            load(gradTbl[idxLD + gridPtLDB[2]]), distLDB,
            load(gradTbl[idxLD + gridPtLDB[2] + 1]), _mm256_blend_pd(distLDB, distRUF, 0b0100), // Vector3(distLDB[0], distLDB[1], distRUF[2]),
            load(gradTbl[idxLU + gridPtLDB[2]]), _mm256_blend_pd(distLDB, distRUF, 0b0010), // Vector3(distLDB[0], distRUF[1], distLDB[2]),
            load(gradTbl[idxLU + gridPtLDB[2] + 1]) ,  _mm256_blend_pd(distLDB, distRUF, 0b0110) ); //Vector3(distLDB[0], distRUF[1], distRUF[2]) );

    __m256d v1324 = dotProd4(
            load(gradTbl[idxRD + gridPtLDB[2]]), _mm256_blend_pd(distLDB, distRUF, 0b0001), // Vector3(distRUF[0], distLDB[1], distLDB[2]),
            load(gradTbl[idxRD + gridPtLDB[2] + 1]),  _mm256_blend_pd(distLDB, distRUF, 0b0101), // Vector3(distRUF[0], distLDB[1], distRUF[2]),
            load(gradTbl[idxRU + gridPtLDB[2]]),  _mm256_blend_pd(distLDB, distRUF, 0b0011), //Vector3(distRUF[0], distRUF[1], distLDB[2]),
            load(gradTbl[idxRU + gridPtLDB[2] + 1]), distRUF); // Vector3(distRUF[0], distRUF[1], distRUF[2]) );
#endif    
    
#else // PERLIN_NOISE_VGATHER
    __m128i gridPt_l0_h0_l1_h1 = _mm_unpacklo_epi32(v_gridPtLDB, v_gridPtLDBp1);

    __m128i gridPt_l0_l0_h0_h0 = _mm_unpacklo_epi32(gridPt_l0_h0_l1_h1, gridPt_l0_h0_l1_h1);
    __m128i idxLLRR = _mm_i32gather_epi32(permutationTbl, gridPt_l0_l0_h0_h0, 4);

    __m128i gridPt_l1_h1_l1_h1 = _mm_shuffle_epi32(gridPt_l0_h0_l1_h1, _MM_SHUFFLE(3,2,3,2));
    __m128i idxLD_LU_RD_RU = _mm_i32gather_epi32(permutationTbl, _mm_add_epi32(idxLLRR, gridPt_l1_h1_l1_h1), 4);

    __m128i gridPt_l2_h2_xx_xx = _mm_unpackhi_epi32(v_gridPtLDB, v_gridPtLDBp1);
    __m128i gridPt_l2_h2_l2_h2 = _mm_shuffle_epi32(gridPt_l2_h2_xx_xx, _MM_SHUFFLE(1,0,1,0));

    __m128i idxLD_LD_LU_LU = _mm_unpacklo_epi32(idxLD_LU_RD_RU, idxLD_LU_RD_RU);
    __m128i u_offsets = _mm_add_epi32(idxLD_LD_LU_LU, gridPt_l2_h2_l2_h2);

    __m128i idxRD_RD_RU_RU = _mm_unpackhi_epi32(idxLD_LU_RD_RU, idxLD_LU_RD_RU);
    __m128i v_offsets = _mm_add_epi32(idxRD_RD_RU_RU, gridPt_l2_h2_l2_h2);

    int uoffsets[4];
    int voffsets[4];

    _mm_store_si128((__m128i *) uoffsets, _mm_slli_epi32(u_offsets, 2));
    _mm_store_si128((__m128i *) voffsets, _mm_slli_epi32(v_offsets, 2));

    __m256d u1324 = dotProd4(
            loadX(gradTbl, uoffsets[0]), distLDB,
            loadX(gradTbl, uoffsets[1]), _mm256_blend_pd(distLDB, distRUF, 0b0100), // Vector3(distLDB[0], distLDB[1], distRUF[2]),
            loadX(gradTbl, uoffsets[2]), _mm256_blend_pd(distLDB, distRUF, 0b0010), // Vector3(distLDB[0], distRUF[1], distLDB[2]),
            loadX(gradTbl, uoffsets[3]),  _mm256_blend_pd(distLDB, distRUF, 0b0110) ); //Vector3(distLDB[0], distRUF[1], distRUF[2]) );

    __m256d v1324 = dotProd4(
            loadX(gradTbl, voffsets[0]), _mm256_blend_pd(distLDB, distRUF, 0b0001), // Vector3(distRUF[0], distLDB[1], distLDB[2]),
            loadX(gradTbl, voffsets[1]),  _mm256_blend_pd(distLDB, distRUF, 0b0101), // Vector3(distRUF[0], distLDB[1], distRUF[2]),
            loadX(gradTbl, voffsets[2]),  _mm256_blend_pd(distLDB, distRUF, 0b0011), //Vector3(distRUF[0], distRUF[1], distLDB[2]),
            loadX(gradTbl, voffsets[3]), distRUF); // Vector3(distRUF[0], distRUF[1], distRUF[2]) );
#endif // PERLIN_NOISE_VGATHER

    DBL result;

#ifndef PERLIN_NOISE_AVX
    DBL a1 = linInterp(s[0], u1, v1);
    DBL a2 = linInterp(s[0], u3, v3);
    DBL b1 = linInterp(s[0], u2, v2);
    DBL b2 = linInterp(s[0], u4, v4);

    DBL c = linInterp(s[1], a1, b1);
    DBL d = linInterp(s[1], a2, b2);

    result = linInterp(s[2], c, d);
#else
    // linear interpolation: t*(b - a) + a
    __m128d s01 = _mm256_castpd256_pd128(v_s);
    __m256d s0 = _mm256_broadcastsd_pd(s01);
    __m256d a1a2b1b2 =
            _mm256_add_pd(u1324,
                    _mm256_mul_pd(_mm256_sub_pd(v1324, u1324), s0));

    __m128d s1 = _mm_unpackhi_pd(s01, s01);
    __m128d a1a2 = _mm256_castpd256_pd128(a1a2b1b2);
    __m128d b1b2 = _mm256_extractf128_pd(a1a2b1b2, 1);
    __m128d cd = _mm_add_pd(a1a2,
                        _mm_mul_pd(_mm_sub_pd(b1b2, a1a2), s1));

    __m128d s2 = _mm256_extractf128_pd(v_s, 1);
    __m128d dc = _mm_shuffle_pd(cd, cd, _MM_SHUFFLE2(0, 1));
    __m128d res =  _mm_add_sd(cd, _mm_mul_sd(_mm_sub_sd(dc, cd), s2));

    result =  _mm_cvtsd_f64(res);
#endif

#ifdef PAPI
    long long counters[2];
    PAPI_stop_counters(counters, 2);

    printf("%lld %lld\n", counters[0], counters[1]);
#endif
//    long long stop = _rdtscp(&mem);
//    printf("%lld\n", stop - start);
//    IACA_END
    return result;
}



void PerlinNoiseCommon::
  setupValues(DBL comp, int & gridIdx0, DBL& dist0, DBL& dist1) const {
    DBL intpart = floor(comp);
    DBL fractpart = comp - intpart;

    gridIdx0 = ((int)intpart) & 255;
    dist0 = fractpart;
    dist1 = dist0 - 1;
}

void PerlinNoiseRandomGradients::seed(long seedVal) {

    Random rng(seedVal);

    for(int j=0; j<256; j++) {
        permutationTbl[j] = j;
        gradTbl[j] = rng.pointInSphere();
    }

    for(int j=0; j<256; j++) {
        int k = int(rng.getDouble()*256);
        std::swap(permutationTbl[j],permutationTbl[k]);
    }

    for(int j=0; j<258; j++) {
        permutationTbl[256 + j] = permutationTbl[j];
    }

    for(int j=0; j<514; j++) {
        gradTbl[j] = gradTbl[j%256];
    }

}

void PerlinNoiseBaseGradients::seed(long seedVal) {

    Random rng(seedVal);

    for(int j=0; j<256; j++) {
        permutationTbl[j] = j;
    }

    for(int j=0; j<256; j++) {
        int k = int(rng.getDouble()*256);
        std::swap(permutationTbl[j],permutationTbl[k]);
    }

    for(int j=0; j<258; j++) {
        permutationTbl[256 + j] = permutationTbl[j];
    }

    for(int j=0; j<514; j++) {
        gradTbl[j] = baseGradients[j%16];
    }

}

const Vector3 PerlinNoiseBaseGradients::baseGradients[16] =
{
    Vector3(1, 1, 0), Vector3(-1, 1, 0), Vector3( 1,-1, 0), Vector3(-1,-1, 0),
    Vector3(1, 0, 1), Vector3(-1, 0, 1), Vector3( 1, 0,-1), Vector3(-1, 0,-1),
    Vector3(0, 1, 1), Vector3( 0,-1, 1), Vector3( 0, 1,-1), Vector3( 0,-1,-1),
    Vector3(1, 1, 0), Vector3(-1, 1, 0), Vector3( 0,-1, 1), Vector3( 0,-1,-1)
};
