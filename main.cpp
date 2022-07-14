#include <iostream>
#include <thread>
#include <unistd.h>
#include <chrono>
using std::chrono::operator""s;

#include "components/sem.cpp"
#include "components/display.cpp"
#include "components/lichtsensor.cpp"
#include "components/tempsensor.cpp"

Semaphore semLicht;
Semaphore semTemp;
Semaphore semDisplay;

DisplayController displayController;
LichtSensor lichtSensor;
TempSensor tempSensor;

long helligkeit = 0;
int temperatur = 0;
int deltaTime = 0;

void brightness(){
	helligkeit = lichtSensor.read();
    while(1){
		/*
			Diese Funktion bekommt den 
			Helligkeitswert vom Licht Sensor
		*/
		semLicht.p();
		helligkeit = lichtSensor.read();
		semDisplay.v();
		if(deltaTime / 60 == 1) {break;}
    }
}

void temp(){
	temperatur = tempSensor.read();
    while(1){
		/*
			Diese Funktion bekommt den
			Temperaturwert vom Licht Sensor
		*/
        semTemp.p();
        temperatur = tempSensor.read();
		semDisplay.v();
		if(deltaTime / 60 == 1) {break;}
    }
}

void display(){
    while (1){
		std::this_thread::sleep_for(1s);
		deltaTime++;
		/*
			Dieser teil der Funktion sendet alle 10 sekunden 
			dem Sieben-Segment-Display den Wert vom Licht Sensor
		*/
		if(deltaTime % 10 == 0){
			semDisplay.p();
			displayController.resetDisplay();
			std::cout << "H " << helligkeit << "\n";
			displayController.show(helligkeit, 1);
			semLicht.v();
		}
		/*
			Dieser teil der Funktion sendet alle 15 sekunden 
			dem Sieben-Segment-Display den Wert vom Temperatur Sensor 
		*/
		if(deltaTime % 15 == 0){
			semDisplay.p();
			displayController.resetDisplay();
			std::cout << "C " << temperatur << "\n";
			displayController.show(temperatur, 2);
			semTemp.v();
		}
		if(deltaTime / 60 == 1) {
			displayController.resetDisplay();
			break;
		}
    }
}

int main()
{
	// Initilisiere die Klassen
	displayController.init();
	lichtSensor.init();
	tempSensor.init();

	// Initilisiere die Semaphores
	semLicht.init(0);
    semTemp.init(0);
	semDisplay.init(1);
	
	std::cout << "Programm startet" << "\n";

	// Erzeuge die drei Threads
	std::thread threadHell(brightness);
	std::thread threadTemp(temp);
    std::thread threadDis(display);

	// Warten auf das Ende der drei Threads
	threadHell.join();
	threadTemp.join();
    threadDis.join();

	std::cout << "Programm beendet" << "\n";
	return 0;
}