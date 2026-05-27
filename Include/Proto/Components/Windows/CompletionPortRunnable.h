#pragma once
#include "Proto/Components/Runnable.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


namespace proto::components {

	class CompletionPortRunnable : public Runnable {
	public:
		CompletionPortRunnable();
		~CompletionPortRunnable();
	protected:
		HANDLE completion_port;

		virtual bool on_start() override;
		virtual void interrupt() override;
		virtual void execute() override;
		virtual void on_stop() override;

		virtual void handle_completion_packet(DWORD bytes_transferred, ULONG_PTR completion_key, LPOVERLAPPED overlapped) = 0;
		virtual void cleanup_completion_key(ULONG_PTR completion_key) = 0;
		virtual void cleanup_overlapped(LPOVERLAPPED overlapped) = 0;
	private:
	};

}