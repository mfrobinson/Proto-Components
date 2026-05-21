#pragma once
#include <thread>
#include <stop_token>
#include <condition_variable>
#include <mutex>


namespace proto {

	class Runnable {
	public:
		Runnable();
		Runnable(Runnable&& other) = delete;
		Runnable(const Runnable& other) = delete;
		Runnable& operator=(Runnable&& other) = delete;
		Runnable& operator=(const Runnable& other) = delete;
		~Runnable();

		void run();
		void start();
		void stop();
		bool running();
	protected:
		virtual bool on_start() = 0;
		virtual void execute() = 0;
		virtual void on_stop() = 0;

		bool should_run();
	private:
		std::jthread runner_thread;
		std::stop_source stop_source;
		std::condition_variable_any stop_condition;
		std::mutex state_mutex;
		bool currently_running;

		void internal_run();

		inline void set_up_to_run();
		inline bool internal_running();
	};

}