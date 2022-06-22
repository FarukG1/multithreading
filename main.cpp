#include <mutex>
#include <condition_variable>
#include <iostream>           // std::cout
#include <thread>             // std::thread
#include <unistd.h>
#include <wiringPiI2C.h>
#include <wiringPi.h>

#define HIGH 1
#define LOW 0

#define bmp280 0x76
#define bh1750 0x23

// Display
#define DIN 27
#define LOAD 28
#define CLK 29

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

int lichtSensor = 0;
int helligkeit = 0;
int temperatur = 0;
int turn = 0; // 0 = nothing, 1 = helligkeit, 2 = temperatur

void brightness(){
    while(1){
        semHelligkeit.p();
		sleep(10);
        int data = wiringPiI2CReadReg16(lichtSensor,0x00);
		if(data == -1) {
			std::cout << "Fehler, Sensor Daten nicht verfügbar" << "\n";
		}
		else {
			helligkeit = ((data & 0xff00)>>8) | ((data & 0x00ff)<<8);
			turn = 1;
        	semDisplay.v();
			turn = 0;
		}
        semHelligkeit.v();
    }
}

void temp(){
    while(1){
        semTemperatur.p();
		sleep(15);
        temperatur ++;
		turn = 2;
        semDisplay.v();
		turn = 0;
        semTemperatur.v();
    }
}

void display(){
    while (1){
		digitalWrite(LOAD, LOW);
        if (turn == 1) {
			std::cout << "H " << helligkeit << "\n";
			// din = 16bit data
			for(int i = 0; i < 16; i++){
				digitalWrite(CLK, HIGH);
				digitalWrite(CLK, LOW);
			}
		}
        else if (turn == 2) {
			std::cout << "C " << temperatur << "\n";
			//			 NO-CARE  ADDRESS  . X   DIGIT
			int *data = {0,0,0,0, 0,0,0,1, 0,0, 0,0,0,0};
			for(int i = 0; i < 16; i++){
				digitalWrite(DIN, data[i]);
				digitalWrite(CLK, HIGH);
				digitalWrite(CLK, LOW);
			}
		}
		digitalWrite(LOAD, HIGH);
    }
}

int main()
{
	wiringPiSetup();
	std::cout << "Display wird eingerichtet" << "\n";
	pinMode(DIN, OUTPUT);
	pinMode(LOAD, OUTPUT);
	pinMode(CLK, OUTPUT);
	std::cout << "Display erfolgreich eingerichtet" << "\n";

	lichtSensor = wiringPiI2CSetup(bh1750);
	sleep(1);
	if(lichtSensor == -1) {
		std::cout << "Fehler, Licht Sensor nicht verfügbar" << "\n";
		return -1;
	}
	else {
		std::cout << "Licht Sensor wird eingerichtet" << "\n";
		int sensWrite = wiringPiI2CWrite(lichtSensor,0x10);
		sleep(1);
		if(sensWrite == -1) {
			std::cout << "Fehler, Licht Sensor konnte nicht eingerichtet werden" << "\n";
			//return -1;
		}
		std::cout << "Licht Sensor erfolgreich eingerichtet" << "\n";
	}

	semHelligkeit.init(1);
    semTemperatur.init(1);
    semDisplay.init(0);

	std::thread threadHell(brightness);
	std::thread threadTemp(temp);
    std::thread threadDis(display);

	// Warten auf das Ende der drei Threads
	threadHell.join();
	threadTemp.join();
    threadDis.join();
	return 0;
}
