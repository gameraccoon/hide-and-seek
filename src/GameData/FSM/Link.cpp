#include "GameData/FSM/Link.h"

namespace FSM
{
	namespace LinkRules
	{
		FunctorLink::FunctorLink(FunctorLink::CanFollowFn canFollowFn)
			: mCanFollowFn(canFollowFn)
		{
		}

		bool FunctorLink::canFollow(const FSM::Blackboard &blackboard) const
		{
			return mCanFollowFn(blackboard);
		}
	}
}
