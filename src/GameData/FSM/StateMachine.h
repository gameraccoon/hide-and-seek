#pragma once

#include <vector>
#include <map>
#include <memory>

#include "GameData/FSM/Blackboard.h"
#include "GameData/FSM/Link.h"

namespace FSM
{
	template <typename StateIDType, typename BlackboardKeyType>
	class StateMachine
	{
	public:
		using BlackboardType = Blackboard<BlackboardKeyType>;
		using BaseLinkRuleType = LinkRule<BlackboardKeyType>;

		struct LinkPair
		{
			LinkPair(StateIDType followingState, std::unique_ptr<BaseLinkRuleType> linkFollowRule)
				: followingState(std::forward<StateIDType>(followingState))
				, linkFollowRule(std::forward<std::unique_ptr<BaseLinkRuleType>>(linkFollowRule))
			{}

			StateIDType followingState;
			std::unique_ptr<BaseLinkRuleType> linkFollowRule;
		};

		struct StateLinks
		{
			std::vector<LinkPair> links;
		};

	public:
		void addState(StateIDType stateID, StateLinks stateLinks)
		{
			mStates.emplace(std::forward<StateIDType>(stateID), std::forward<StateLinks>(stateLinks));
		}

		void update()
		{
			bool needToProcess = true;
			StateIDType previousState = mCurrentState;
			while (needToProcess == true)
			{
				needToProcess = false;
				for (const LinkPair& link : mStates[mCurrentState].links)
				{
					if (link.linkFollowRule->canFollow(mBlackboard))
					{
						mCurrentState = link.followingState;
						needToProcess = true;

						AssertRetVoid(mCurrentState != previousState, "FSM cycle detected");
					}
				}
			}
		}

		StateIDType getCurrentState() const { return mCurrentState; }
		void setState(StateIDType newState) { mCurrentState = newState; }
		BlackboardType& getBlackboardRef() { return mBlackboard; }

	private:
		std::map<StateIDType, StateLinks> mStates;
		BlackboardType mBlackboard;
		StateIDType mCurrentState;
	};
}
