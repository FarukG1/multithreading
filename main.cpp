#include <mutex>
#include <condition_variable>
#include <iostream>           // std::cout
#include <thread>             // std::thread
#include <unistd.h>
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
int turn = 0; // 0 = nothing, 1 = helligkeit, 2 = temperatur

void brightness(){
    while(1){
        semHelligkeit.p();
		sleep(10);
        helligkeit ++;
		turn = 1;
        semDisplay.v();
		turn = 0;
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
