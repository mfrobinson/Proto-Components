#pragma once
#include <vector>
#include <thread>


namespace proto {

	class Threadpool {
	public:
		Threadpool();
		Threadpool(Threadpool&& other);
		Threadpool(const Threadpool& other) = delete;
		Threadpool& operator=(Threadpool&& other);
		Threadpool& operator=(const Threadpool& other) = delete;
		~Threadpool();

		void assign(std::thread&& thread);
		void join();
		void detach();
	protected:
	private:
		std::vector<std::thread> thread_pool;

		inline void move(Threadpool& other);
		inline void cleanup();
	};

}