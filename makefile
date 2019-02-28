exe: hello.o
	g++ hello.o -lglut -lGLU -lGL  -o exe

hello.o: hello.cpp
	g++ -c hello.cpp

clean:
	rm *.o exe
