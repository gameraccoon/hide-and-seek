#pragma once

#include <vector>
#include <map>
#include <string>
#include <typeindex>
#include <functional>
#include <memory>

#include "GameData/FSM/Blackboard.h"
#include "GameData/FSM/Link.h"

namespace FSM
{
	template <typename StateIDType>
	class StateMachine
	{
	public:
		struct LinkPair
		{
			LinkPair(StateIDType followingState, std::unique_ptr<LinkRule> linkFollowRule)
				: followingState(std::forward<StateIDType>(followingState))
				, linkFollowRule(std::forward<std::unique_ptr<LinkRule>>(linkFollowRule))
			{}

			StateIDType followingState;
			std::unique_ptr<LinkRule> linkFollowRule;
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
		Blackboard& getBlackboardRef() { return mBlackboard; }

	private:
		std::map<StateIDType, StateLinks> mStates;
		Blackboard mBlackboard;
		StateIDType mCurrentState;
	};
}
