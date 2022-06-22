#include <mutex>
#include <condition_variable>
#include <iostream>           // std::cout
#include <thread>             // std::thread
#include <unistd.h>
#include <wiringPiI2C.h>

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

int helligkeitSensor = 0;
int helligkeit = 0;
int temperatur = 0;
int turn = 0; // 0 = nothing, 1 = helligkeit, 2 = temperatur

void brightness(){
    while(1){
        semHelligkeit.p();
		sleep(10);
        int data = wiringPiI2CReadReg16(helligkeitSensor,0x00);
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
        if (turn == 1) std::cout << "H " << helligkeit << "\n";
        if (turn == 2) std::cout << "C " << temperatur << "\n";
    }
}

int main()
{
	helligkeitSensor = wiringPiI2CSetup(bh1750);
	sleep(1);
	if(helligkeitSensor == -1) {
		std::cout << "Fehler, Helligkeit Sensor nicht verfügbar" << "\n";
		return -1;
	}
	else {
		std::cout << "Helligkeit Sensor wird eingerichtet" << "\n";
		int sensWrite = wiringPiI2CWrite(helligkeitSensor,0x10);
		sleep(1);
		if(sensWrite == -1) {
			std::cout << "Fehler, Helligkeit Sensor konnte nicht eingerichtet werden" << "\n";
			return -1;
		}
		std::cout << "Helligkeit Sensor erfolgreich eingerichtet" << "\n";
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
