#pragma once

#include <memory>

namespace Jobs
{
	class BaseJob
	{
	public:
		using UniquePtr = std::unique_ptr<BaseJob>;
		using JobGroupID = unsigned long;

	public:
		virtual ~BaseJob() = default;

		// called in worker thread
		virtual void process() = 0;
		// called in the requeser thread when the work is done
		virtual void finalize() = 0;

		void setJobGroupID(JobGroupID jobGroupID);
		JobGroupID getJobGroupID();

	private:
		JobGroupID mJobGroupID;
	};
}
