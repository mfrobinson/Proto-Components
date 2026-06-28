#include "Proto/Components/SingleThreaded/Timer.h"


namespace proto::components::single_threaded {

	Timer::Timer() : Runnable(), next_timeout_id(0) {
		return;
	}

	Timer::~Timer() {
		this->stop();
		return;
	}

	uint64_t Timer::set_timeout(const std::chrono::nanoseconds timeout, std::function<void()> callback) {
		auto target_time = std::chrono::steady_clock::now() + timeout;
		uint64_t timeout_id;
		{
			std::lock_guard<std::mutex> lock(this->queue_mutex);
			timeout_id = this->next_timeout_id++;
			auto iterator = this->timeout_queue.emplace(timeout_id, target_time, std::move(callback));
			this->iterators_of_ids[timeout_id] = iterator;

		}
		this->timeout_cv.notify_all();
		return timeout_id;
	}

	void Timer::cancel_timeout(const uint64_t timeout_id) {
		std::lock_guard<std::mutex> lock(this->queue_mutex);
		auto iterator = this->iterators_of_ids.find(timeout_id);
		if (iterator != this->iterators_of_ids.end()) {
			this->timeout_queue.erase(iterator->second);
			this->iterators_of_ids.erase(iterator);
		}
		return;
	}

	bool Timer::on_start() {
		this->timeout_queue = std::multiset<TimeoutInfo, GreaterTimeoutInfo>();
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
			auto iterator_of_next_timeout = this->timeout_queue.begin();

			if (iterator_of_next_timeout->timeout_point <= now) {
				TimeoutInfo current = std::move(*iterator_of_next_timeout);
				this->timeout_queue.erase(iterator_of_next_timeout);
				this->iterators_of_ids.erase(current.timeout_id);
				queue_lock.unlock();
				current.callback();
				queue_lock.lock();
			}
			else {
				size_t number_of_timeouts_before_wait = this->timeout_queue.size();
				// Wait until the timer is stopped, the next timeout in order is reached, or a NEW timeout is added (which could be sooner than the previous next_timeout)
				this->timeout_cv.wait_until(queue_lock, this->stop_source.get_token(), iterator_of_next_timeout->timeout_point, [this, number_of_timeouts_before_wait] {return this->timeout_queue.size() > number_of_timeouts_before_wait; });
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