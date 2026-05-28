#pragma once
#include "Proto/Components/Runnable.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>


namespace proto::components {

	class ConsoleHandler : public Runnable {
	public:
		ConsoleHandler();
		ConsoleHandler(ConsoleHandler&& other) = delete;
		ConsoleHandler(const ConsoleHandler& other) = delete;
		ConsoleHandler& operator=(ConsoleHandler&& other) = delete;
		ConsoleHandler& operator=(const ConsoleHandler& other) = delete;
		~ConsoleHandler();
	protected:
		virtual bool on_start() override;
		virtual void execute() override;
		virtual void on_cleanup() override;

		virtual void handle_input(const std::string& input) = 0;
	private:
		HANDLE interrupt_event_handle;
		inline void set_console_writable(HANDLE& console_handle, bool writable);
	};

}