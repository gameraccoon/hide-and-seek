#pragma once

#include <vector>
#include <map>
#include <memory>

#include "GameData/FSM/Blackboard.h"
#include "GameData/FSM/Link.h"

namespace FSM
{
	/**
	 * Hierarchical FSM implementation
	 *
	 * The SM can be initialized by adding states via calling addState method.
	 * The current state and the blackboard are stored outside the class, allowing
	 * using one SM for multiple state instanses.
	 *
	 * Allow to have more than one levels of states.
	 * Each parent node should have an unique state for identification purposes, but selecting a parent
	 * node will result in selecting its default child node (if it was set on initialization).
	 *
	 * The same behavior can be achieved with StateMachine class (and in some cases more efficient),
	 * but it will result in links duplication and will be harder to reason about.
	 */
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

			~LinkPair() = default;
			LinkPair(LinkPair&& other) noexcept = default;
			LinkPair& operator=(LinkPair&& other) noexcept = default;

			LinkPair(const LinkPair& other)
				: LinkPair(other.followingState, other.linkFollowRule->makeCopy())
			{}

			LinkPair& operator=(const LinkPair& other)
			{
				 return *this = LinkPair(other.followingState, other.linkFollowRule->makeCopy());
			}

			StateIDType followingState;
			std::unique_ptr<BaseLinkRuleType> linkFollowRule;
		};

		struct StateLinkRules
		{
			// less verbose emplace function
			template <template<typename...> typename LinkRuleType, typename... Types, typename... Args>
			void emplaceLink(StateIDType state, Args... args)
			{
				links.emplace_back(std::forward<StateIDType>(state), std::make_unique<LinkRuleType<BlackboardKeyType, Types...>>(std::forward<Args>(args)...));
			}

			std::vector<LinkPair> links;
		};

	public:
		void addState(StateIDType stateID, StateLinkRules stateLinkRules)
		{
			bool isEmplaced;
			std::tie(std::ignore, isEmplaced) = mStates.emplace(std::forward<StateIDType>(stateID), std::forward<StateLinkRules>(stateLinkRules));
			Assert(isEmplaced, "State is already exists");
		}

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

		StateIDType getNextState(const BlackboardType& blackboard, StateIDType previousState) const
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

				auto stateIt = mStates.find(currentState);
				if (stateIt == mStates.end())
				{
					return currentState;
				}

				for (const LinkPair& link : stateIt->second.links)
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
		bool recursiveUpdateParents(StateIDType& state, const BlackboardType& blackboard) const
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

			auto stateIt = mStates.find(processingState);
			if (stateIt == mStates.end())
			{
				return false;
			}

			for (const LinkPair& link : stateIt->second.links)
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
		std::map<StateIDType, StateLinkRules> mStates;
		std::map<StateIDType, StateIDType> mChildToParentLinks;
		std::map<StateIDType, StateIDType> mParentToChildLinks;
	};
}
