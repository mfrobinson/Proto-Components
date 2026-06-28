#include "Proto/Components/Runnable.h"
#include <stdexcept>
#include <iostream>

namespace proto::components {

	Runnable::Runnable() : currently_running(false) {
		return;
	}

	Runnable::~Runnable() {
		this->stop();
		if (this->runner_thread.joinable()) this->runner_thread.join();
		this->on_cleanup();
		return;
	}

	bool Runnable::on_start() {
		return true;
	}

	void Runnable::interrupt() {
		return;
	}

	void Runnable::on_stop() {
		return;
	}

	void Runnable::on_cleanup() {
		return;
	}

	void Runnable::run() {
		{
			std::lock_guard<std::mutex> state_guard(this->state_mutex);
			if (this->internal_running()) return;
			if (this->runner_thread.joinable()) this->runner_thread.join();
			if (!this->on_start()) return;
			this->set_up_to_run();
		}
		this->internal_run();
		return;
	}

	void Runnable::start() {
		std::lock_guard<std::mutex> state_guard(this->state_mutex);
		if (this->internal_running()) return;
		if (this->runner_thread.joinable()) this->runner_thread.join();
		if (!this->on_start()) return;
		this->set_up_to_run();
		this->runner_thread = std::jthread(&Runnable::internal_run, this);
		return;
	}

	void Runnable::stop() {
		std::unique_lock<std::mutex> state_lock(this->state_mutex);
		if (!this->internal_running()) return;
		this->stop_source.request_stop();
		this->interrupt();
		std::stop_token stop_token(this->stop_source.get_token());
		this->stop_condition.wait(state_lock, [this]() {
			return !this->internal_running();
		});
		return;
	}

	bool Runnable::running() {
		std::lock_guard<std::mutex> state_guard(this->state_mutex);
		return this->internal_running();
	}

	void Runnable::request_stop() {
		this->stop_source.request_stop();
		return;
	}

	bool Runnable::should_run() {
		return !this->stop_source.stop_requested();
	}

	void Runnable::internal_run() {
		this->execute();
		std::lock_guard<std::mutex> state_lock(this->state_mutex);
		this->currently_running = false;
		this->on_stop();
		this->stop_condition.notify_all();
		return;
	}

	bool Runnable::internal_running() {
		return this->currently_running;
	}

	void Runnable::set_up_to_run() {
		this->stop_source = std::stop_source();
		this->currently_running = true;
		return;
	}

}