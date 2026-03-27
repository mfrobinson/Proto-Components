#include "Proto/Runnable.h"
#include <iostream>


namespace proto {

	Runnable::Runnable() : keep_running(false), done_running(true), state_mutex_ptr(std::make_unique<std::mutex>()), run_complete_condition_ptr(std::make_unique<std::condition_variable>()) {
		return;
	}

	Runnable::Runnable(Runnable&& other) {
		this->move_from(other);
		return;
	}

	Runnable& Runnable::operator=(Runnable&& other) {
		if (&other != this) {
			this->cleanup();
			this->move_from(other);
		}
		return *this;
	}

	Runnable::~Runnable() {
		this->cleanup();
		return;
	}

	void Runnable::run() {
		if (!this->should_start()) {
			return;
		}
		this->internal_run();
		return;
	}

	void Runnable::start() {
		if (!this->should_start()) {
			return;
		}
		std::thread runner(&Runnable::internal_run, this);
		runner.detach();
		return;
	}

	void Runnable::stop() {
		std::unique_lock<std::mutex> state_lock(*this->state_mutex_ptr);
		if (this->keep_running == true) {
			this->keep_running = false;
		}
		this->run_complete_condition_ptr->wait(state_lock, [this]() {
			return this->done_running;
		});
		return;
	}

	bool Runnable::running() {
		std::lock_guard<std::mutex> state_guard(*this->state_mutex_ptr);
		return this->keep_running || !this->done_running;
	}

	bool Runnable::should_run() {
		std::lock_guard<std::mutex> state_guard(*this->state_mutex_ptr);
		return this->keep_running;
	}

	inline void Runnable::move_from(Runnable& other) {
		this->keep_running = other.keep_running;
		this->done_running = other.done_running;
		this->state_mutex_ptr = std::move(other.state_mutex_ptr);
		this->run_complete_condition_ptr = std::move(other.run_complete_condition_ptr);
		other.keep_running = false;
		other.done_running = true;
		other.state_mutex_ptr = std::make_unique<std::mutex>();
		other.run_complete_condition_ptr = std::make_unique<std::condition_variable>();
		return;
	}

	inline void Runnable::cleanup() {
		this->stop();
		return;
	}

	inline bool Runnable::should_start() {
		if (this->running()) {
			return false;
		}
		std::lock_guard<std::mutex> state_lock(*this->state_mutex_ptr);
		this->keep_running = true;
		this->done_running = false;
		return true;
	}

	void Runnable::internal_run() {
		this->execute();
		std::lock_guard<std::mutex> state_lock(*this->state_mutex_ptr);
		this->keep_running = false;
		this->done_running = true;
		return;
	}

}