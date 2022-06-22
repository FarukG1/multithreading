#include <mutex>
#include <condition_variable>
#include <iostream>           // std::cout
#include <thread>             // std::thread
#include <unistd.h>
#include <chrono>
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include "display.cpp"
using std::chrono::operator""ms;
#define bmp280 0x76
#define bh1750 0x23

class semaphore
{
	private:
		std::mutex mutex_;
		std::condition_variable condition_;	
		unsigned long count_ = 0; // Initialisierungswert

	public:
		void init(int n)
		{
			count_ = n;
		}

		void v() // V-Operation zum Aufheben des Schutzes
		{
			std::lock_guard<std::mutex> lock(mutex_);
			++count_;
			condition_.notify_one();
		}

		void p() // P-Operation zum Setzen des Schutzes
		{
			std::unique_lock<std::mutex> lock(mutex_);
			while (!count_) // Sperren, wenn der Zaehler null ist
				condition_.wait(lock);
			--count_;
		}
};

semaphore semHelligkeit;
semaphore semTemperatur;
semaphore semDisplay;

int helligkeit = 0;
int temperatur = 0;

void brightness(){
    while(1){
        semHelligkeit.p();
		std::this_thread::sleep_for(10000ms);
        // int data = wiringPiI2CReadReg16(lichtSensor,0x00);
		// if(data == -1) {
		// 	std::cout << "Fehler, Sensor Daten nicht verfügbar" << "\n";
		// }
		// else {
		// 	helligkeit = ((data & 0xff00)>>8) | ((data & 0x00ff)<<8);
		// 	turn = 1;
        // 	semDisplay.v();
		// }
		helligkeit+=3;
        semDisplay.v();
		
    }
}

void temp(){
    while(1){
        semTemperatur.p();
		std::this_thread::sleep_for(15000ms);
        temperatur += 4;
        semDisplay.v();
		
    }
}

void display(){
    while (1){
		semDisplay.p();
		semHelligkeit.v();
		
        std::cout << "H " << helligkeit << "\n";
		resetDisplay();
		displayNumber(helligkeit, 1);
	
		semDisplay.p();
		semTemperatur.v();
		std::cout << "C " << temperatur << "\n";
		resetDisplay();
		displayNumber(temperatur, 2);
		
    }
}

int main()
{
	wiringPiSetup();
	initDisplay();

	lichtSensor = wiringPiI2CSetup(bh1750);
	//sleep(1);
	// if(lichtSensor == -1) {
	// 	std::cout << "Fehler, Licht Sensor nicht verfügbar" << "\n";
	// 	return -1;
	// }
	// else {
	// 	std::cout << "Licht Sensor wird eingerichtet" << "\n";
	// 	int sensWrite = wiringPiI2CWrite(lichtSensor,0x10);
	// 	sleep(1);
	// 	if(sensWrite == -1) {
	// 		std::cout << "Fehler, Licht Sensor konnte nicht eingerichtet werden" << "\n";
	// 		//return -1;
	// 	}
	// 	else std::cout << "Licht Sensor erfolgreich eingerichtet" << "\n";
	// }
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
