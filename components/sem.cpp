#include <mutex>
#include <condition_variable>
// Semaphore Class
class Semaphore {
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