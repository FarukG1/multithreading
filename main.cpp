#include <iostream>           // std::cout
#include <thread>             // std::thread
#include <unistd.h>
#include <chrono>
using std::chrono::operator""ms;

#include "components/sem.cpp"
#include "components/display.cpp"
#include "components/lichtsensor.cpp"
#include "components/tempsensor.cpp"

#define bmp280 0x76 // Temp

Semaphore semHelligkeit;
Semaphore semTemperatur;
Semaphore semDisplay;

DisplayController displayController;
LichtSensor lichtSensor;
TempSensor tempSensor;

int helligkeit = 0;
int temperatur = 0;

void brightness(){
    while(1){
        semHelligkeit.p();
		std::this_thread::sleep_for(10000ms);
		helligkeit = lichtSensor.read();
        semDisplay.v();
		
    }
}

void temp(){
    while(1){
        semTemperatur.p();
		std::this_thread::sleep_for(15000ms);
        temperatur = tempSensor.read();
        semDisplay.v();
		
    }
}

void display(){
    while (1){
		semDisplay.p();
		semHelligkeit.v();
		
        std::cout << "H " << helligkeit << "\n";
		displayController.show(helligkeit, 1);
	
		semDisplay.p();
		semTemperatur.v();
		std::cout << "C " << temperatur << "\n";
		displayController.show(temperatur, 2);
		
    }
}

int main()
{
	displayController.init();
	lichtSensor.init();

	semHelligkeit.init(1);
    semTemperatur.init(1);
	semDisplay.init(0);
	
	std::cout << "Programm startet" << "\n";

	std::thread threadHell(brightness);
	std::thread threadTemp(temp);
    std::thread threadDis(display);

	// Warten auf das Ende der drei Threads
	threadHell.join();
	threadTemp.join();
    threadDis.join();
	return 0;
}