#include <mutex>
#include <condition_variable>
#include <iostream>           // std::cout
#include <thread>             // std::thread

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
float time = 0;
bool turn = false; // false = helligkeit, true = temperatur

void brightness(){
    while(1){
        semHelligkeit.p();
        if(time == 10){
            helligkeit ++;
            semDisplay.v();
        }
        semHelligkeit.v();
    }
}

void display(){
    while (1){
        semDisplay.p();
        if (turn) std:cout << "H " << helligkeit;
        if (!turn) std:cout << "C " << temperatur;
    }
}

int main()
{

	semHelligkeit.init(1);
    semTemperatur.init(1);
    semDisplay.init(1);
	//Erzeugen von zwei Threads, die beide die Funktion print_char ausfuehren sollen
	std::thread threadHell(brightness);
	//std::thread threadTemp(print_char, 20, '+');
    std::thread threadDis(display);
	// Warten auf das Ende der beiden Threads
	threadHell.join();
	//threadTemp.join();
    threadDis.join();
	return 0;
}
