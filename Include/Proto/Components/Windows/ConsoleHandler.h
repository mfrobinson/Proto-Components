#pragma once
#include "Proto/Components/Runnable.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>


namespace proto::components {

	/**
	* An abstract Windows-specific implementation of an interactive console loop.
	* 
	* This class inherits from Runnable to manage an active console interface, which
	* can potentially be run in a separate thread of execution. It utilises native Windows
	* synchronisation handles to manage interrupting blocking waits safely.
	*/
	class ConsoleHandler : public Runnable {
	public:
		/**
		* Default constructor configuring native Windows interaction handles.
		*/
		ConsoleHandler();

		ConsoleHandler(ConsoleHandler&& other) = delete;
		ConsoleHandler(const ConsoleHandler& other) = delete;
		ConsoleHandler& operator=(ConsoleHandler&& other) = delete;
		ConsoleHandler& operator=(const ConsoleHandler& other) = delete;

		/**
		* Destructor ensuring native Windows event resources are fully released.
		*/
		~ConsoleHandler();
	protected:
		/**
		* Setup routine to configure the interrupt event handle.
		* 
		* @return True if initialisation succeeds and execution can proceed, false to abort.
		*/
		virtual bool on_start() override;

		/**
		* The primary processing loop monitoring the standard console input.
		* 
		* This overrides the pure virtual execute method from Runnable to run the Windows
		* console lifecycle.
		*/
		virtual void execute() override;

		/**
		* Final cleanup routine to safely close out low-level platform handles.
		*/
		virtual void on_cleanup() override;

		/**
		* Abstract interface method triggered whenever a line of input is submitted.
		* 
		* A derived class must implement this method to process custom commands or raw
		* user input.
		* 
		* @param[in] input The raw string of input captured from the standard input line.
		*/
		virtual void handle_input(const std::string& input) = 0;
	private:
		/**
		* Native Windows event primitive used to interrupt waiting for a keyboard event on
		* the standard input line.
		*/
		HANDLE interrupt_event_handle;

		/**
		* Helper function to safely adjust the access rights on low-level console subsystem
		* targets.
		* 
		* @param[in] console_handle Reference to the native Windows platform console identifier.
		* @param[in] writable True to enable writing actions, false to disable them.
		*/
		inline void set_console_writable(HANDLE& console_handle, bool writable);
	};

}