#include "Proto/Threadpool.h"


namespace proto {

	Threadpool::Threadpool() {
		return;
	}

	Threadpool::Threadpool(Threadpool&& other) {
		this->move(other);
		return;
	}

	Threadpool& Threadpool::operator=(Threadpool&& other) {
		if (&other != this) {
			this->cleanup();
			this->move(other);
		}
		return *this;
	}

	Threadpool::~Threadpool() {
		this->cleanup();
		return;
	}

	void Threadpool::assign(std::thread&& thread) {
		this->thread_pool.push_back(std::move(thread));
		return;
	}

	void Threadpool::join() {
		for (std::thread& thread : this->thread_pool) {
			thread.join();
		}
		this->thread_pool.clear();
		return;
	}

	void Threadpool::detach() {
		for (std::thread& thread : this->thread_pool) {
			thread.detach();
		}
		this->thread_pool.clear();
		return;
	}

	void Threadpool::move(Threadpool& other) {
		this->thread_pool = std::move(other.thread_pool);
		return;
	}

	void Threadpool::cleanup() {
		this->detach();
		return;
	}

}