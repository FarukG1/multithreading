#include <iostream>
#include <thread>
#include <unistd.h>
#include <chrono>
using std::chrono::operator""s;

#include "components/headerfiles/sem.h"
#include "components/headerfiles/display.h"
#include "components/headerfiles/lichtsensor.h"
#include "components/headerfiles/tempsensor.h"

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

		/*
			Endet die dauerschleife nach einer gewissen
			anzahl an Sekunden
		*/
		if(deltaTime / 180 == 1) {break;}
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

		/*
			Endet die dauerschleife nach einer gewissen
			anzahl an Sekunden
		*/
		if(deltaTime / 180 == 1) {break;}
    }
}

void display(){
    while (1){
		std::this_thread::sleep_for(1s);
		deltaTime++;

		// Fürs Debuggen: gibt die abgelaufenen sekunden an
		//std::cout << deltaTime << "s"<< "\n";

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

		/*
			Endet die dauerschleife nach einer gewissen
			anzahl an Sekunden
		*/
		if(deltaTime / 180 == 1) {
			std::this_thread::sleep_for(5s);
			displayController.resetDisplay();
			break;
		}
    }
}

int main(){
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