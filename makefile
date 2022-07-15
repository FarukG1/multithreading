main: main.o display.o sem.o lichtsensor.o tempsensor.o
	g++ main.o display.o sem.o lichtsensor.o tempsensor.o -o main -lwiringPi -pthread

main.o: main.cpp
	g++ -c main.cpp

display.o: components/cppfiles/display.cpp components/headerfiles/display.h
	g++ -c components/cppfiles/display.cpp

sem.o: components/cppfiles/sem.cpp components/headerfiles/sem.h
	g++ -c components/cppfiles/sem.cpp

lichtsensor.o: components/cppfiles/lichtsensor.cpp components/headerfiles/lichtsensor.h
	g++ -c components/cppfiles/lichtsensor.cpp

tempsensor.o: components/cppfiles/tempsensor.cpp components/headerfiles/tempsensor.h
	g++ -c components/cppfiles/tempsensor.cpp

clean:
	rm *.o main