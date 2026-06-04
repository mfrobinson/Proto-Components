#pragma once
#include <thread>
#include <stop_token>
#include <condition_variable>
#include <mutex>


namespace proto::components {

	/**
	* An abstract base class managing synchronous and asynchronous execution loops.
	* 
	* This class provides lifecycle management for executable tasks. It encapsulates
	* thread creation using modern C++ execution tokens (std::jthread) and safe teardown
	* patterns. It cannot be copied or moved due to its underlying synchronisation primitives.
	*/
	class Runnable {
	public:
		/**
		* Default constructor initialising the execution state.
		*/
		Runnable();

		Runnable(Runnable&& other) = delete;
		Runnable(const Runnable& other) = delete;
		Runnable& operator=(Runnable&& other) = delete;
		Runnable& operator=(const Runnable& other) = delete;

		/**
		* Destructor ensuring the running thread is fully stopped and resources are cleaned up.
		*/
		~Runnable();

		/**
		* Execute this runnable in the current calling thread.
		*/
		void run();

		/**
		* Deploy a new background thread to execute this runnable asynchronously.
		*/
		void start();

		/**
		* Blocks the calling thread until execution has fully completed and the state has been safely reset.
		*/
		void stop();

		/**
		* Returns whether this runnable is currently in a state of executing.
		* 
		* @return True if executing, false otherwise.
		*/
		bool running();

		/**
		* Signals this runnable to stop execution but does not block the calling thread.
		*/
		void request_stop();
	protected:
		/**
		* Initialisation code to be run before each execution.
		* 
		* This is invoked before the main execution logic begins.
		* 
		* @return True to proceed with execution, false to abort immediately.
		*/
		virtual bool on_start();

		/**
		* The logic of the primary task of this executable.
		* 
		* This pure virtual method must be implemented by a derived class.
		*/
		virtual void execute() = 0;

		/**
		* Method to interrupt this runnable's execution logic.
		* 
		* Derived classes should override this to ensure fast termination instead of relying on polling.
		*/
		virtual void interrupt();

		/**
		* Cleanup code to be run after each execution.
		* 
		* This should correspond to what has been initialised in the on_start method.
		*/
		virtual void on_stop();

		/**
		* Cleanup code to be run inside the destructor after any executions are guarunteed to have been successfully stopped.
		* 
		* This should correspond to what has been initialised in the constructor.
		*/
		virtual void on_cleanup();

		/**
		* Checks if a stop has been requested or signalled.
		* 
		* @return True if execution should continue as normal, false if termination of this execution has been requested.
		*/
		bool should_run();
	private:
		/// The underlying thread context executing the task/service.
		std::jthread runner_thread;
		/// Management token to coordinate stop requests.
		std::stop_source stop_source;
		/// Synchronisation variable to handle thread blocking until successful termination.
		std::condition_variable_any stop_condition;
		/// Synchronisation primitive guarding internal execution flags.
		std::mutex state_mutex;
		/// Tracks the internal execution runtime status.
		bool currently_running;

		/**
		* Internal execution wrapper method that encapsulates the common execution logic.
		* 
		* This wrapper handles invoking the virtual execute method, resetting the state after execution has completed,
		* and notifying all listeners of the condition variable member stop_condition.
		*/
		void internal_run();

		/**
		* Configures the internal state variables to prepare for an upcoming execution.
		*/
		inline void set_up_to_run();

		/**
		* Internal method to access whether or not this runnable is currently executing.
		* 
		* This method bypasses the state_mutex guard since it is to be used when the caller has already
		* locked and gained ownership of the mutex and must retain it for other operations which must
		* immediately follow without relinquishing the mutex between.
		* 
		* @return True if this runnable is currently executing, false otherwise.
		*/
		inline bool internal_running();
	};

}