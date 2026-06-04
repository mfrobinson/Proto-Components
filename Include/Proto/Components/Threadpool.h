#pragma once
#include <vector>
#include <thread>


namespace proto::components {

	/**
	* A basic thread management class that acts as a collection of standard threads.
	* 
	* This class provides basic aggregation over a collection of std::thread objects.
	* This container explicitly supports move operations, allowing thread ownership to
	* be transferred across multiple scopes. However, copying is explicitly not supported.
	*/
	class Threadpool {
	public:
		/**
		* Default constructor initialising an empty thread pool.
		*/
		Threadpool();

		/**
		* Move constructor transferring ownership of all tracked threads from another thread pool.
		* 
		* @param[in] other The rvalue reference of the thread pool to move from.
		*/
		Threadpool(Threadpool&& other);

		Threadpool(const Threadpool& other) = delete;

		/**
		* Move assignment operator transferring ownership of tracked threads from another pool, only
		* after detaching all of the threads currently being tracked by this thread pool object.
		* 
		* @param[in] other The rvalue reference of the thread pool to move from.
		* @return A reference to this updated thread pool object.
		*/
		Threadpool& operator=(Threadpool&& other);

		Threadpool& operator=(const Threadpool& other) = delete;

		/**
		* Destructor which detaches all remaining threads being tracked by this thread pool object.
		*/
		~Threadpool();

		/**
		* Assigns and takes direct ownership of an actively executing thread.
		* 
		* @param[in] thread An rvalue reference to the thread instance to be added to the thread pool.
		*/
		void assign(std::thread&& thread);

		/**
		* Blocks the calling thread until every single thread currently managed by this thread pool object
		* completes execution.
		*/
		void join();

		/**
		* Detaches all currently tracked threads in the pool, letting them run independently from this container.
		*/
		void detach();
	protected:
	private:
		/// Vector container holding the actual managed thread contexts.
		std::vector<std::thread> thread_pool;

		/**
		* Internal helper method that encapsulates the resource movement and ownership transfer logic.
		* 
		* @param[in] other The active thread pool object to pull the internal vector storage from.
		*/
		inline void move(Threadpool& other);

		/**
		* Internal helper method to ensure the object is cleaned up, and ready to transfer ownership from another
		* thread pool object, or finish being destructed.
		*/
		inline void cleanup();
	};

}