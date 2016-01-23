This is a simple C++ Ray-Tracer that I wrote in 2005.

The project has its home at
> http://www.povworld.org/raytracer

On a Posix system you should be able to compile the program using
> g++ -O3 -march=native src/*.cpp -o raytracer

Once compiled you find a binary 'raytracer'. You start the ray tracer with ./raytracer. Note that there is currently no parser! Thus, to create your own scenes you will have to program them in C++ (see examples.cpp).

The compiled program provides the example scenes 0 through 11. You can specify the scene number using '-n NUMBER' option. Further, you can specify the image with and height using '-h HEIGHT' and '-w WIDTH'.

E.g.
> ./raytracer -n 3 -w 320 -h 240 
will render scene number 3 in 320x240.

Send comments to the program and source code, possible bugs or any questions to micha@povworld.org.

You are welcome to contribute to the project.

Have fun!
- Micha Riser
