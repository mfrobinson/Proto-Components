#pragma once
#include "Proto/Components/Runnable.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


namespace proto::components {

	/**
	* An abstract Windows-specific Runnable base class wrapping an I/O Completion Port (IOCP) loop.
	* 
	* This class encapsulates the high-performance Windows asynchronous I/O model. It manages
	* the native completion port handle and routes incoming completion packets through a virtual
	* method.
	*/
	class CompletionPortRunnable : public Runnable {
	public:
		/**
		* Default constructor initialising the kernel completion port state.
		*/
		CompletionPortRunnable();

		CompletionPortRunnable(CompletionPortRunnable&& other) = delete;
		CompletionPortRunnable(const CompletionPortRunnable& other) = delete;
		CompletionPortRunnable& operator=(CompletionPortRunnable&& other) = delete;
		CompletionPortRunnable& operator=(const CompletionPortRunnable& other) = delete;

		/**
		* Destructor ensuring the underlying completion port handle is cleanly closed.
		*/
		~CompletionPortRunnable();
	protected:
		/// Native Win32 handle to the underlying I/O Completion Port instance.
		HANDLE completion_port;

		/**
		* Specific completion key value that instructs the main packet capture and process
		* loop to terminate.
		*/
		static const ULONG_PTR IOCP_SHUTDOWN_KEY;

		/**
		* System setup callback to instantiate the kernel completion port if it has not been
		* successfully initialised already.
		* 
		* @return True if the kernel port creation succeeds, false to abort execution.
		*/
		virtual bool on_start() override;

		/**
		* Posts a completion packet to the completion port with a value equal to the
		* IOCP_SHUTDOWN_KEY member variable as the completion key structure pointer.
		* This instructs the main packet capture and process loop to terminate.
		*/
		virtual void interrupt() override;

		/**
		* The main packet capture and process loop.
		* 
		* Continuously waits for completion packets to arrive at the completion port. Once
		* a completion packet arrives, it is retrieved and passed to the handle_completion_packet
		* virtual method, unless it has a completion key equal to IOCP_SHUTDOWN_KEY, in which case
		* this method's main loop terminates and execution finishes.
		*/
		virtual void execute() override;

		/**
		* Cleanup logic to retreive and cleanup all completion packets still left in the completion port.
		* 
		* The virtual cleanup methods for completion keys and overlapped structures are invoked on any valid
		* remaining structures.
		*/
		virtual void on_stop() override;

		/**
		* Abstract event handler triggered whenever a valid completion packet arrives at the port.
		* 
		* @param[in] bytes_transferred The total number of bytes transferred during the I/O operation.
		* @param[in] completion_key The unique user-defined identification value to identify what resource was operated on.
		* @param[in] overlapped A pointer to the standard Windows asynchronous control block structure.
		*/
		virtual void handle_completion_packet(DWORD bytes_transferred, ULONG_PTR completion_key, LPOVERLAPPED overlapped) = 0;

		/**
		* Abstract lifecycle memory cleanup method for valid completion keys.
		* 
		* @param[in] completion_key The unique tracking key identifying the channel or resource needing cleanup.
		*/
		virtual void cleanup_completion_key(ULONG_PTR completion_key) = 0;

		/**
		* Abstract lifecycle memory cleanup method for valid overlapped structures.
		* 
		* @param[in] overlapped A pointer to the specific control block structure requiring memory deallocation.
		*/
		virtual void cleanup_overlapped(LPOVERLAPPED overlapped) = 0;
	private:
	};

}