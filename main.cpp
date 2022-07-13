#include <iostream>           // std::cout
#include <thread>             // std::thread
#include <unistd.h>
#include <chrono>			 // Zeit Bibliothek
using std::chrono::operator""ms;

#include "components/sem.cpp"
#include "components/display.cpp"
#include "components/lichtsensor.cpp"
#include "components/tempsensor.cpp"

Semaphore semLichtsensor;
Semaphore semTemperatur;
Semaphore semDisplay;

DisplayController displayController;
LichtSensor lichtSensor;
TempSensor tempSensor;

long helligkeit = 0;
int temperatur = 0;

void brightness(){
    while(1){
		/*
			Diese Funktion bekommt alle 10 Sekunden
			den Helligkeitswert vom Licht Sensor
		*/
        semLichtsensor.p();
		std::this_thread::sleep_for(10000ms);
		helligkeit = lichtSensor.read();
        semDisplay.v();
    }
}

void temp(){
    while(1){
		/*
			Diese Funktion bekommt alle 10 Sekunden
			den Temperaturwert vom Licht Sensor
		*/
        semTemperatur.p(); // Lock this thread
		std::this_thread::sleep_for(15000ms); // warte 15 sekunden
        temperatur = tempSensor.read();	// Lies die Temperatur vom Sensor
        semDisplay.v(); // Unlock display thread
    }
}

void display(){
    while (1){
		/*
			Dieser teil der Funktion sendet dem Sieben-Segment-Display
			den Wert vom Licht Sensor 
		*/
		semDisplay.p();
        std::cout << "H " << helligkeit << "\n";
		displayController.show(helligkeit, 1);
		semLichtsensor.v();

		/*
			Dieser teil der Funktion sendet dem Sieben-Segment-Display
			den Wert vom Temperatur Sensor 
		*/
		semDisplay.p();
		std::cout << "C " << temperatur << "\n";
		displayController.show(temperatur, 2);
		semTemperatur.v();
    }
}

int main()
{
	// Initilisiere die Klassen
	displayController.init();
	lichtSensor.init();
	tempSensor.init();

	// Initilisiere die Semaphores
	semLichtsensor.init(1);
    semTemperatur.init(1);
	semDisplay.init(0);
	
	std::cout << "Programm startet" << "\n";

	// Erzeuge die drei Threads
	std::thread threadHell(brightness);
	std::thread threadTemp(temp);
    std::thread threadDis(display);

	// Warten auf das Ende der drei Threads
	threadHell.join();
	threadTemp.join();
    threadDis.join();
	return 0;
}