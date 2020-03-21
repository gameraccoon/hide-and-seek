#include "Base/precomp.h"

#include "Utils/Jobs/Job.h"

namespace Jobs
{
	void BaseJob::setJobGroupID(JobGroupID jobGroupID)
	{
		mJobGroupID = jobGroupID;
	}

	BaseJob::JobGroupID Jobs::BaseJob::getJobGroupID()
	{
		return mJobGroupID;
	}
}
