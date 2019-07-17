#pragma once

#include <vector>
#include <map>
#include <memory>

#include "GameData/FSM/StateMachine.h"

namespace FSM
{
	/**
	 * Hierarchical FSM implementation
	 *
	 * Uses the same semantics as StateMachine class but allow to have more than one levels of states.
	 * Each parent node should have an unique state for identification purposes, but selecting a parent
	 * node will result in selecting its default child node (if it was set on initialization).
	 *
	 * The same behavior can be achieved with StateMachine class (and in some cases more efficient),
	 * but it will result in links duplication and will be harder to reason about.
	 */
	template <typename StateIDType, typename BlackboardKeyType>
	class HierarchicalStateMachine : public StateMachine<StateIDType, BlackboardKeyType>
	{
	public:
		using Super = StateMachine<StateIDType, BlackboardKeyType>;

	public:
		void linkStates(StateIDType childStateID, StateIDType parentStateID, bool isDefaultState = false)
		{
			mChildToParentLinks.emplace(std::forward<StateIDType>(childStateID), std::forward<StateIDType>(parentStateID));
			if (isDefaultState)
			{
				bool isEmplaced;
				std::tie(std::ignore, isEmplaced) = mParentToChildLinks.emplace(std::forward<StateIDType>(parentStateID), std::forward<StateIDType>(childStateID));
				Assert(isEmplaced, "More than one initial state set for a parent state");
			}
		}

		StateIDType getNextState(const typename Super::BlackboardType& blackboard, StateIDType previousState) const
		{
			bool needToProcess = true;
			StateIDType currentState = previousState;
			while (needToProcess == true)
			{
				needToProcess = false;

				bool isProcessedByParent = recursiveUpdateParents(currentState, blackboard);
				if (isProcessedByParent)
				{
					replaceToChildState(currentState);
					needToProcess = true;
					AssertRet(currentState != previousState, currentState, "FSM cycle detected");
					continue;
				}

				auto stateIt = Super::mStates.find(currentState);
				if (stateIt == Super::mStates.end())
				{
					return currentState;
				}

				for (const typename Super::LinkPair& link : stateIt->second.links)
				{
					if (link.linkFollowRule->canFollow(blackboard))
					{
						currentState = link.followingState;

						replaceToChildState(currentState);
						needToProcess = true;

						AssertRet(currentState != previousState, currentState, "FSM cycle detected");
						break;
					}
				}
			}

			return currentState;
		}

	private:
		bool recursiveUpdateParents(StateIDType& state, const typename Super::BlackboardType& blackboard) const
		{
			auto parentIt = mChildToParentLinks.find(state);
			if (parentIt == mChildToParentLinks.end())
			{
				return false;
			}

			StateIDType processingState = parentIt->second;

			bool isProcessed = recursiveUpdateParents(processingState, blackboard);
			if (isProcessed)
			{
				return true;
			}

			auto stateIt = Super::mStates.find(processingState);
			if (stateIt == Super::mStates.end())
			{
				return false;
			}

			for (const typename Super::LinkPair& link : stateIt->second.links)
			{
				if (link.linkFollowRule->canFollow(blackboard))
				{
					state = link.followingState;
					return true;
				}
			}

			return false;
		}

		void replaceToChildState(StateIDType& state) const
		{
			typename std::map<StateIDType, StateIDType>::const_iterator childIt;

			while(true)
			{
				childIt = mParentToChildLinks.find(state);
				if (childIt == mParentToChildLinks.end())
				{
					break;
				}
				state = childIt->second;
			}
		}

	private:
		std::map<StateIDType, StateIDType> mChildToParentLinks;
		std::map<StateIDType, StateIDType> mParentToChildLinks;
	};
}
