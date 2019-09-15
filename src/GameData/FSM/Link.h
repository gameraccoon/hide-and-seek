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
		virtual std::unique_ptr<LinkRule<BlackboardKeyType>> makeCopy() const = 0;
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

			bool canFollow(const Blackboard<BlackboardKeyType>& blackboard) const override
			{
				return mCanFollowFn(blackboard);
			}

			std::unique_ptr<LinkRule<BlackboardKeyType>> makeCopy() const override
			{
				return std::make_unique<FunctorLink<BlackboardKeyType>>(mCanFollowFn);
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

			bool canFollow(const BlackboardType& blackboard) const override
			{
				return blackboard.template getValue<T>(mName) == mExpectedValue;
			}

			std::unique_ptr<LinkRule<BlackboardKeyType>> makeCopy() const override
			{
				return std::make_unique<VariableEqualLink<BlackboardKeyType, T>>(mName, mExpectedValue);
			}

		private:
			BlackboardKeyType mName;
			T mExpectedValue;
		};
	}
}