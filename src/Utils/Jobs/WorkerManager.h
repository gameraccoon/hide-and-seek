#pragma once

#include <vector>
#include <list>
#include <atomic>
#include <condition_variable>
#include <thread>

#include "Utils/Jobs/Job.h"

namespace Jobs
{
	class WorkerManager
	{
	public:
		WorkerManager(size_t threadsCount);
		~WorkerManager();
		WorkerManager(const WorkerManager&) = delete;
		WorkerManager& operator=(const WorkerManager&) = delete;
		WorkerManager(WorkerManager&&) = delete;
		WorkerManager& operator=(WorkerManager&&) = delete;

		void runJobs(std::vector<BaseJob::UniquePtr>&& jobs);

		size_t getThreadsCount() { return mThreads.size(); }

	private:
		void threadFunction();

	private:
		std::mutex mMutex;
		bool mShutdown = false;
		// queue for working threads
		std::list<BaseJob::UniquePtr> mPlannedJobs;
		std::condition_variable mPlannedJobsTrigger;
		// queue for started threads for finalization
		std::list<BaseJob::UniquePtr> mFinishedJobs;
		std::condition_variable mFinishedJobsTrigger;
		std::vector<std::thread> mThreads;
		BaseJob::JobGroupID mNextId = 0;
	};

	size_t GetAvailableThreadsCount();
}
