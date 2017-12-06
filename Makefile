mandelbrot: mandelbrot.cpp
	g++ mandelbrot.cpp -o mandelbrot -lglut -lGLU -lGL -lm
clean:
	rm *.o mandelbrot