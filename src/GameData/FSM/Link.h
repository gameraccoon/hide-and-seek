#pragma once

#include <any>
#include <typeindex>
#include <functional>
#include <string>

#include "GameData/FSM/Blackboard.h"

namespace FSM
{
	template<typename BlackboardKeyType>
	class LinkRule
	{
	public:
		using BlackboardType = Blackboard<BlackboardKeyType>;

	public:
		virtual ~LinkRule() = default;
		virtual bool canFollow(const BlackboardType& blackboard) const = 0;
	};

	namespace LinkRules
	{
		/**
		 * Link that can be parametrized with functor
		 */
		template<typename BlackboardKeyType>
		class FunctorLink : public LinkRule<BlackboardKeyType>
		{
		public:
			using BlackboardType = Blackboard<BlackboardKeyType>;
			using CanFollowFn = std::function<bool(const BlackboardType& blackboard)>;

		public:
			FunctorLink(CanFollowFn canFollowFn)
				: mCanFollowFn(canFollowFn)
			{
			}

			bool canFollow(const Blackboard<BlackboardKeyType>& blackboard) const
			{
				return mCanFollowFn(blackboard);
			}

		private:
			CanFollowFn mCanFollowFn;
		};

		/**
		 * Link that will be followed if a blackboard variable have a specific value
		 */
		template<typename BlackboardKeyType, typename T>
		class VariableEqualLink : public LinkRule<BlackboardKeyType>
		{
		public:
			using BlackboardType = Blackboard<BlackboardKeyType>;

		public:
			VariableEqualLink(BlackboardKeyType name, T expectedValue)
				: mName(std::forward<BlackboardKeyType>(name))
				, mExpectedValue(std::forward<T>(expectedValue))
			{
			}

			bool canFollow(const BlackboardType& blackboard) const
			{
				return blackboard.template getValue<T>(mName) == mExpectedValue;
			}

		private:
			BlackboardKeyType mName;
			T mExpectedValue;
		};
	}
}
