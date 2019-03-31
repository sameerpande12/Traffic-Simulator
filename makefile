OS := $(shell uname)
ifeq ($(OS), Darwin)
    CFLAGS=-framework OpenGL  -framework GLUT
else
		CFLAGS = -lglut -lGLU -Wall -lGL -L /lib64  -l pthread
endif
exe: prac.o  updateCreatePrint.o Road.o Vehicle.o  common.o template.o processKeys.o
	g++   prac.o updateCreatePrint.o Road.o Vehicle.o common.o template.o processKeys.o  $(CFLAGS) -o exe -Wno-deprecated

prac.o: prac.cpp Vehicle.cpp Vehicle.h common.cpp common.h Road.cpp Road.h template.cpp template.h processKeys.cpp processKeys.h updateCreatePrint.cpp updateCreatePrint.h
	g++ -c  prac.cpp  -Wno-deprecated

processKeys.o: processKeys.cpp processKeys.h common.h common.cpp
	g++ ${CFLAGS} -c processKeys.cpp


template.o: template.cpp template.h common.cpp common.h
	g++ -c template.cpp

updateCreatePrint.o: updateCreatePrint.cpp updateCreatePrint.h Vehicle.cpp Vehicle.h common.cpp common.h Road.cpp Road.h
	g++ -c updateCreatePrint.cpp

Road.o: Vehicle.cpp Vehicle.h common.cpp common.h Road.cpp Road.h
	g++ -c Road.cpp
Vehicle.o: Vehicle.cpp Vehicle.h common.cpp common.h Road.cpp Road.h
	g++ -c Vehicle.cpp

common.o : common.cpp common.h
	g++ -c common.cpp
clean:
	rm *.o  exe
