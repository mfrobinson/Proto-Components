#include "Proto/Components/Windows/ConsoleHandler.h"
#include <iostream>


namespace proto::components {

	ConsoleHandler::ConsoleHandler() : interrupt_event_handle(CreateEvent(NULL, TRUE, FALSE, NULL)) {
		return;
	}

	ConsoleHandler::~ConsoleHandler() {
		return;
	}

	bool ConsoleHandler::on_start() {
		if (!__super::on_start()) return false;
		if (this->interrupt_event_handle == NULL) this->interrupt_event_handle = CreateEvent(NULL, TRUE, FALSE, NULL);
		if (this->interrupt_event_handle == NULL) return false;
		if (ResetEvent(this->interrupt_event_handle) == 0) return false;
		return true;
	}

	void ConsoleHandler::execute() {
		std::string input;
		HANDLE input_handle = GetStdHandle(STD_INPUT_HANDLE);
		DWORD number_of_events = 0;
		INPUT_RECORD input_buffer;
		set_console_writable(input_handle, false);
		HANDLE wait_handles[2] = { this->interrupt_event_handle, input_handle };
		std::cout << std::flush;

		DWORD signalled_object;
		while (this->should_run()) {
			signalled_object = WaitForMultipleObjects(2, wait_handles, FALSE, INFINITE);
			if (signalled_object == WAIT_OBJECT_0) {
				break;
			}
			if (ReadConsoleInput(input_handle, &input_buffer, 1, &number_of_events)) {
				if (number_of_events > 0 && input_buffer.EventType == KEY_EVENT && input_buffer.Event.KeyEvent.bKeyDown) {
					char character = input_buffer.Event.KeyEvent.uChar.AsciiChar;
					const auto& virtual_key_code = input_buffer.Event.KeyEvent.wVirtualKeyCode;
					switch (virtual_key_code) {
						case VK_BACK:
							std::cout << "\b \b";
							if (input.length() > 0) {
								input = input.substr(0, input.length() - 1);
							}
							break;
						case VK_RETURN:
							std::cout << std::endl;
							this->handle_input(input);
							input = "";
							break;
						default:
							if (character >= 32 && character <= 126) {
								input += character;
								std::cout << character;
							}
							break;
					}
				}
			}
		}
		return;

	}

	void ConsoleHandler::on_cleanup() {
		CloseHandle(this->interrupt_event_handle);
		__super::on_cleanup();
		return;
	}

	void ConsoleHandler::set_console_writable(HANDLE& console_handle, bool writable) {
		DWORD mode;
		GetConsoleMode(console_handle, &mode);
		if (writable) mode |= (ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);
		else mode &= ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);
		SetConsoleMode(console_handle, mode);
		return;
	}

}