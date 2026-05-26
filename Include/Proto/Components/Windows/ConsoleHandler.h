#pragma once
#include "Proto/Components/Runnable.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>


namespace proto::components {

	class ConsoleHandler : public Runnable {
	public:
		using Runnable::Runnable;
	protected:
		// Initialisation code to be run on each execution, if false is returned then execution is aborted
		//virtual bool on_start();
		// What this Runnable should do when it is executed
		virtual void execute() override;
		// Method to interrupt the execution (execute method) of this Runnable
		//virtual void interrupt();
		// Cleanup code to be run after each execution
		//virtual void on_stop();
		// Cleanup code to be run in the destructor after ensuring execution has stopped
		//virtual void on_cleanup();

		virtual void handle_input(const std::string& input) = 0;
	private:
		inline void set_console_writable(HANDLE& console_handle, bool writable);
	};

}