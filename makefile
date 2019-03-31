OS := $(shell uname)
ifeq ($(OS), Darwin)
    CFLAGS=-framework OpenGL  -framework GLUT
else
    CFLAGS=  -lglut -lGLU -lGL -L /lib64  -l pthread
endif
exe: prac.o
	g++ prac.o $(CFLAGS)  -o exe -Wno-deprecated
prac.o: prac.cpp
	g++ -c prac.cpp -Wno-deprecated
clean:
	rm *.o exe
