#include <mutex>
#include <condition_variable>
#include "../headerfiles/sem.h"
// Semaphore Class
void Semaphore::init(int n)
{
	count_ = n;
}
void Semaphore::v() // V-Operation zum Aufheben des Schutzes
{
	std::lock_guard<std::mutex> lock(mutex_);
	++count_;
	condition_.notify_one();
}
void Semaphore::p() // P-Operation zum Setzen des Schutzes
{
	std::unique_lock<std::mutex> lock(mutex_);
	while (!count_) // Sperren, wenn der Zaehler null ist
		condition_.wait(lock);
	--count_;
}