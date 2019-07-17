#pragma once

#include <vector>
#include <map>
#include <memory>

#include "GameData/FSM/Blackboard.h"
#include "GameData/FSM/Link.h"

namespace FSM
{
	/**
	 * Finite State Machine implementation
	 *
	 * The SM can be initialized by adding states calling addState method.
	 * The current state and the blackboard are stored outside the class, allowing
	 * using one SM for multiple state instanses.
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

		virtual StateIDType getNextState(const BlackboardType& blackboard, StateIDType previousState) const
		{
			bool needToProcess = true;
			StateIDType currentState = previousState;
			while (needToProcess == true)
			{
				needToProcess = false;
				auto stateIt = mStates.find(currentState);
				if (stateIt == mStates.end())
				{
					break;
				}

				for (const LinkPair& link : stateIt->second.links)
				{
					if (link.linkFollowRule->canFollow(blackboard))
					{
						currentState = link.followingState;
						needToProcess = true;

						AssertRet(currentState != previousState, currentState, "FSM cycle detected");
						break;
					}
				}
			}

			return currentState;
		}

	protected:
		std::map<StateIDType, StateLinkRules> mStates;
	};
}
