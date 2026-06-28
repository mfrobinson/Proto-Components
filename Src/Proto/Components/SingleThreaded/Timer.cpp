#include "Proto/Components/SingleThreaded/Timer.h"


namespace proto::components::single_threaded {

	Timer::Timer() : Runnable() {
		return;
	}

	Timer::~Timer() {
		this->stop();
		return;
	}

	void Timer::set_timeout(const std::chrono::nanoseconds timeout, std::function<void()> callback) {
		auto target_time = std::chrono::steady_clock::now() + timeout;
		{
			std::lock_guard<std::mutex> lock(this->queue_mutex);
			this->timeout_queue.push({ target_time, std::move(callback) });
		}
		this->timeout_cv.notify_all();
	}

	bool Timer::on_start() {
		this->timeout_queue = std::priority_queue<TimeoutInfo, std::vector<TimeoutInfo>, GreaterTimeoutInfo>();
		this->stop_source = std::stop_source();
		return true;
	}

	void Timer::execute() {
		std::unique_lock<std::mutex> queue_lock(this->queue_mutex);
		while (!this->stop_source.stop_requested() || !this->timeout_queue.empty()) {
			if (this->timeout_queue.empty()) {
				// Wait until the timer is stopped, or a timeout is set.
				this->timeout_cv.wait(queue_lock, this->stop_source.get_token(), [this] {return !this->timeout_queue.empty(); });
				if (this->timeout_queue.empty()) continue;
			}

			auto now = std::chrono::steady_clock::now();
			auto& next_timeout = this->timeout_queue.top();

			if (next_timeout.timeout_point <= now) {
				TimeoutInfo current = std::move(const_cast<TimeoutInfo&>(next_timeout));
				this->timeout_queue.pop();

				queue_lock.unlock();
				current.callback();
				queue_lock.lock();
			}
			else {
				size_t number_of_timeouts_before_wait = this->timeout_queue.size();
				// Wait until the timer is stopped, the next timeout in order is reached, or a NEW timeout is added (which could be sooner than the previous next_timeout)
				this->timeout_cv.wait_until(queue_lock, this->stop_source.get_token(), next_timeout.timeout_point, [this, number_of_timeouts_before_wait] {return this->timeout_queue.size() > number_of_timeouts_before_wait; });
			}
		}
		return;
	}
	void Timer::interrupt() {
		this->stop_source.request_stop();
		this->timeout_cv.notify_all();
		return;
	}

}