#pragma once
#include "Proto/Components/Runnable.h"
#include <chrono>
#include <functional>
#include <mutex>
#include <queue>
#include <vector>
#include <condition_variable>


namespace proto::components::single_threaded {

	class Timer : public Runnable {
	public:
		Timer();
		Timer(Timer&& other);
		Timer(const Timer& other) = delete;
		Timer& operator=(Timer&& other);
		Timer& operator=(const Timer& other) = delete;
		~Timer();

		void set_timeout(const std::chrono::nanoseconds timeout, std::function<void()> callback);
	protected:
		virtual bool on_start() override;
		virtual void execute() override;
		virtual void interrupt() override;
	private:
		struct TimeoutInfo {
			std::chrono::steady_clock::time_point timeout_point;
			std::function<void()> callback;
		};
		struct GreaterTimeoutInfo {
			bool operator()(const TimeoutInfo& t1, const TimeoutInfo& t2) {
				return t1.timeout_point > t2.timeout_point;
			}
		};
		std::mutex queue_mutex;
		// Min heap to order the timeouts
		std::priority_queue<TimeoutInfo, std::vector<TimeoutInfo>, GreaterTimeoutInfo> timeout_queue;

		std::condition_variable_any timeout_cv;
		std::stop_source stop_source;
	};

}