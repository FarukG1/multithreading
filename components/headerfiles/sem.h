#pragma once
#include <mutex>
#include <condition_variable>

class Semaphore {
	private:
		std::mutex mutex_;
		std::condition_variable condition_;	
		unsigned long count_ = 0;
	public:
		void init(int n);
		void v();
		void p();
};