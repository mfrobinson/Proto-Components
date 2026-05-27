#include "Proto/Components/Windows/CompletionPortRunnable.h"


namespace proto::components {

	CompletionPortRunnable::CompletionPortRunnable() {
		return;
	}

	CompletionPortRunnable::~CompletionPortRunnable() {
		if (this->completion_port != NULL) CloseHandle(this->completion_port);
		return;
	}

	bool CompletionPortRunnable::on_start() {
		if (this->completion_port == NULL) this->completion_port = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
		if (this->completion_port == NULL) return false;
		return true;
	}

	void CompletionPortRunnable::interrupt() {
		PostQueuedCompletionStatus(this->completion_port, 0, NULL, NULL);
		return;
	}

	void CompletionPortRunnable::on_stop() {
		DWORD bytes_transferred = 0;
		ULONG_PTR completion_key = NULL;
		LPOVERLAPPED overlapped = NULL;
		while (GetQueuedCompletionStatus(this->completion_port, &bytes_transferred, &completion_key, &overlapped, 0)) {
			if (overlapped != NULL) this->cleanup_overlapped(overlapped);
			if (completion_key != NULL) this->cleanup_completion_key(completion_key);
		}
		return;
	}

	void CompletionPortRunnable::execute() {
		DWORD bytes_transferred = 0;
		ULONG_PTR completion_key = NULL;
		LPOVERLAPPED overlapped = NULL;

		bool result = GetQueuedCompletionStatus(this->completion_port, &bytes_transferred, &completion_key, &overlapped, INFINITE);
		while (result && overlapped != NULL) {
			this->handle_completion_packet(bytes_transferred, completion_key, overlapped);
			if (overlapped != NULL) this->cleanup_overlapped(overlapped);
			result = GetQueuedCompletionStatus(this->completion_port, &bytes_transferred, &completion_key, &overlapped, INFINITE);
		}
		if (overlapped != NULL) this->cleanup_overlapped(overlapped);
		if (completion_key != NULL) this->cleanup_completion_key(completion_key);
		return;
	}

}