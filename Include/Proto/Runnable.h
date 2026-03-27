#pragma once
#include <thread>
#include <mutex>
#include <memory>


namespace proto {

	class Runnable {
	public:
		Runnable();
		Runnable(Runnable&& other);
		Runnable(const Runnable& other) = delete;
		Runnable& operator=(Runnable&& other);
		Runnable& operator=(const Runnable& other) = delete;
		~Runnable();

		void run();
		void start();
		void stop();
		bool running();
	protected:
		virtual void execute() = 0;
		bool should_run();
	private:
		bool keep_running;
		bool done_running;
		std::unique_ptr<std::mutex> state_mutex_ptr;
		std::unique_ptr<std::condition_variable> run_complete_condition_ptr;

		inline void move_from(Runnable& other);
		inline void copy_from(const Runnable& other) = delete;
		inline void cleanup();

		inline bool should_start();

		void internal_run();
	};

}