#include "Base/precomp.h"

#include "Utils/Jobs/WorkerManager.h"

#include <algorithm>
#include <functional>

namespace Jobs
{
	WorkerManager::WorkerManager(size_t threadsCount)
		: mThreads(threadsCount)
	{
		// start the threads
		for (size_t i = 0; i < threadsCount; ++i)
		{
			mThreads[i] = std::thread(std::bind(&WorkerManager::threadFunction, this));
		}
	}

	WorkerManager::~WorkerManager()
	{
		{
			std::unique_lock<std::mutex> l(mMutex);
			mShutdown = true;
		}

		mPlannedJobsTrigger.notify_all();

		for (auto& threadData : mThreads)
		{
			threadData.join();
		}
	}

	void WorkerManager::runJobs(std::vector<BaseJob::UniquePtr>&& jobs)
	{
		BaseJob::JobGroupID thisJobGroupId = mNextId++;
		size_t runnedJobsCount = jobs.size();

		{
			std::unique_lock<std::mutex> l(mMutex);

			std::for_each(
				begin(jobs),
				end(jobs),
				[thisJobGroupId](auto& job){ job->setJobGroupID(thisJobGroupId); }
			);

			std::move(
				begin(jobs),
				end(jobs),
				back_inserter(mPlannedJobs)
			);

			mPlannedJobsTrigger.notify_all();
		}

		BaseJob::UniquePtr thisThreadActiveJob;
		bool isJobsListEmpty = false;
		std::vector<BaseJob::UniquePtr> jobsToFinalize;
		jobsToFinalize.reserve(runnedJobsCount);
		while (true) // Loop-With-Exit
		{
			if (!isJobsListEmpty)
			{
				std::unique_lock<std::mutex> l(mMutex);

				// find a job to do in this thread while it waiting for finalizing tasks
				auto it = std::find_if(
					std::begin(mPlannedJobs),
					std::end(mPlannedJobs),
					[thisJobGroupId](auto& job){ return job->getJobGroupID() == thisJobGroupId; }
				);

				if (it != mPlannedJobs.end())
				{
					thisThreadActiveJob = std::move(*it);
					mPlannedJobs.erase(it);
				}
				else
				{
					isJobsListEmpty = true;
				}
			}

			if (thisThreadActiveJob)
			{
				thisThreadActiveJob->process();
				thisThreadActiveJob->finalize();
				--runnedJobsCount;
				if (runnedJobsCount == 0) { return; }
				thisThreadActiveJob = nullptr;
			}
			else
			{
				// no jobs to do, waiting for finalization tasks only
				std::unique_lock<std::mutex> l(mMutex);
				while (!mShutdown && mFinishedJobs.empty())
				{
					mFinishedJobsTrigger.wait(l);
				}

				if (mShutdown)
				{
					return;
				}
			}

			{
				std::unique_lock<std::mutex> l(mMutex);

				// find and group finalization tasks
				auto newEnd = std::partition(
					std::begin(mFinishedJobs),
					std::end(mFinishedJobs),
					[thisJobGroupId](auto& job){ return job->getJobGroupID() != thisJobGroupId; }
				);

				// move away to process without locking
				std::move(
					newEnd,
					end(mFinishedJobs),
					back_inserter(jobsToFinalize)
				);

				// erase old records from the queue
				mFinishedJobs.erase(
					newEnd,
					end(mFinishedJobs)
				);
			}

			for (auto& job : jobsToFinalize)
			{
				job->finalize();
				--runnedJobsCount;
				if (runnedJobsCount == 0) { return; }
			}
			jobsToFinalize.clear();
		}
	}

	void WorkerManager::threadFunction()
	{
		BaseJob::UniquePtr currentJob;

		while(true)
		{
			{
				std::unique_lock<std::mutex> l(mMutex);

				// wait for shutdown or a new planned job
				while (!mShutdown && mPlannedJobs.empty())
				{
					mPlannedJobsTrigger.wait(l);
				}

				if (mShutdown)
				{
					return;
				}

				currentJob = std::move(mPlannedJobs.front());
				mPlannedJobs.pop_front();
			}

			currentJob->process();

			{
				std::unique_lock<std::mutex> l(mMutex);
				mFinishedJobs.push_back(std::move(currentJob));
				mFinishedJobsTrigger.notify_all();
			}
		}
	}

	size_t GetAvailableThreadsCount()
	{
		size_t coresCount = std::thread::hardware_concurrency();
		return (coresCount >= 2) ? coresCount - 1 : 1;
	}
}
