CFLAGS=  -lglut -lGLU -Wall -lGL -L /lib64  -l pthread

exe: prac.o Road.o Vehicle.o common.o template.o
	g++   prac.o Road.o Vehicle.o common.o template.o  $(CFLAGS) -o exe -Wno-deprecated

prac.o: prac.cpp Vehicle.cpp Vehicle.h common.cpp common.h Road.cpp Road.h template.cpp template.o
	g++ -c  prac.cpp  -Wno-deprecated

template.o: template.cpp template.h
	g++ -c template.cpp

Road.o: Vehicle.cpp Vehicle.h common.cpp common.h Road.cpp Road.h
	g++ -c Road.cpp
Vehicle.o: Vehicle.cpp Vehicle.h common.cpp common.h Road.cpp Road.h
	g++ -c Vehicle.cpp

common.o : common.cpp common.h
	g++ -c common.cpp
clean:
	rm *.o *.gch exe
