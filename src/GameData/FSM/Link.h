#pragma once

#include <any>
#include <typeindex>
#include <functional>
#include <string>

#include "GameData/FSM/Blackboard.h"

namespace FSM
{
	class LinkRule
	{
	public:
		virtual ~LinkRule() = default;
		virtual bool canFollow(const Blackboard& blackboard) const = 0;
	};

	namespace LinkRules
	{
		/**
		 * Link that can be parametrized with functor
		 */
		class FunctorLink : public LinkRule
		{
		public:
			using CanFollowFn = std::function<bool(const Blackboard& blackboard)>;

		public:
			FunctorLink(CanFollowFn canFollowFn);
			bool canFollow(const Blackboard& blackboard) const;

		private:
			CanFollowFn mCanFollowFn;
		};

		/**
		 * Link that will be followed if a blackboard variable have a specific value
		 */
		template<typename T>
		class VariableEqualLink : public LinkRule
		{
		public:
			using CanFollowFn = std::function<bool(const Blackboard& blackboard)>;

		public:
			VariableEqualLink(const std::string& name, T expectedValue)
				: mName(name)
				, mExpectedValue(expectedValue)
			{
			}

			bool canFollow(const Blackboard& blackboard) const
			{
				return blackboard.getValue<T>(mName) == mExpectedValue;
			}

		private:
			std::string mName;
			T mExpectedValue;
		};
	}
}
