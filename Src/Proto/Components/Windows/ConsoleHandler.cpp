#include "Proto/Components/Windows/ConsoleHandler.h"
#include <iostream>


namespace proto::components {

	void ConsoleHandler::execute() {
		std::string input;
		HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
		DWORD number_of_events = 0;
		INPUT_RECORD input_buffer;
		set_console_writable(hInput, false);
		std::cout << std::flush;

		while (this->should_run()) {
			WaitForSingleObject(hInput, INFINITE);

			if (ReadConsoleInput(hInput, &input_buffer, 1, &number_of_events)) {
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

	void ConsoleHandler::set_console_writable(HANDLE& console_handle, bool writable) {
		DWORD mode;
		GetConsoleMode(console_handle, &mode);
		if (writable) mode |= (ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);
		else mode &= ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);
		SetConsoleMode(console_handle, mode);
		return;
	}

}