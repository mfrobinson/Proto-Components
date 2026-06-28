#pragma once
#include "Proto/Components/Runnable.h"
#include <chrono>
#include <functional>
#include <mutex>
#include <queue>
#include <vector>
#include <condition_variable>
#include <set>


namespace proto::components::single_threaded {

	class Timer : public Runnable {
	public:
		Timer();
		Timer(Timer&& other) = delete;
		Timer(const Timer& other) = delete;
		Timer& operator=(Timer&& other) = delete;
		Timer& operator=(const Timer& other) = delete;
		~Timer();

		uint64_t set_timeout(const std::chrono::nanoseconds timeout, std::function<void()> callback);
		void cancel_timeout(const uint64_t timeout_id);
	protected:
		virtual bool on_start() override;
		virtual void execute() override;
		virtual void interrupt() override;
	private:
		struct TimeoutInfo {
			uint64_t timeout_id;
			std::chrono::steady_clock::time_point timeout_point;
			std::function<void()> callback;
		};
		struct GreaterTimeoutInfo {
			static bool operator()(const TimeoutInfo& t1, const TimeoutInfo& t2) {
				return t1.timeout_point < t2.timeout_point;
			}
		};
		uint64_t next_timeout_id;
		std::mutex queue_mutex;
		// Min heap to order the timeouts
		std::unordered_map<uint64_t, std::multiset<TimeoutInfo, GreaterTimeoutInfo>::iterator> iterators_of_ids;
		std::multiset<TimeoutInfo, GreaterTimeoutInfo> timeout_queue;

		std::condition_variable_any timeout_cv;
		std::stop_source stop_source;
	};

}