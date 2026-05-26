#pragma once
#include <thread>
#include <stop_token>
#include <condition_variable>
#include <mutex>


namespace proto::components {

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
		void request_stop();
	protected:
		// Initialisation code to be run on each execution, if false is returned then execution is aborted
		virtual bool on_start();
		// What this Runnable should do when it is executed
		virtual void execute() = 0;
		// Method to interrupt the execution (execute method) of this Runnable
		virtual void interrupt();
		// Cleanup code to be run after each execution
		virtual void on_stop();
		// Cleanup code to be run in the destructor after ensuring execution has stopped
		virtual void on_cleanup();

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