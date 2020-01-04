#pragma once

#include <bitset>
#include <limits>

#include <neargye/magic_enum.hpp>

/**
 * Bitset that can be used with enum class
*/
template <typename T>
class Bitset
{
public:
	Bitset() = default;
	// can be implicitly created from T
	template<typename... Flags>
	Bitset(Flags... initialValues) { initWithValue(initialValues...); }

	template<typename... Flags>
	void set(Flags... flags) { return setInner(true, flags...); }
	template<typename... Flags>
	void unset(Flags... flags) { return setInner(false, flags...); }

	bool has(T flag) { return mBitset.test(static_cast<typename std::underlying_type<T>::type>(flag)); }
	template<typename... Flags>
	bool hasAll(Flags... flags) { return hasAllTheFlagsInner(flags...); }
	template<typename... Flags>
	bool hasAnyOf(Flags... flags) { return hasAnyOfTheFlagsInner(flags...); }

private:
	void initWithValue(T flag)
	{
		mBitset.set(static_cast<typename std::underlying_type<T>::type>(flag), true);
	}
	template<typename... Flags>
	void initWithValue(T firstFlag, Flags... otherFlags)
	{
		initWithValue(firstFlag);
		initWithValue(otherFlags...);
	}

	void setInner(bool isSet, T flag)
	{
		mBitset.set(static_cast<typename std::underlying_type<T>::type>(flag), isSet);
	}
	template<typename... Flags>
	void setInner(bool isSet, T firstFlag, Flags... otherFlags)
	{
		setInner(isSet, firstFlag);
		setInner(isSet, otherFlags...);
	}

	bool hasAllTheFlagsInner(T flag)
	{
		return has(flag);
	}
	template<typename... Flags>
	bool hasAllTheFlagsInner(T firstFlag, Flags... otherFlags)
	{
		return has(firstFlag) && hasAllTheFlagsInner(otherFlags...);
	}

	bool hasAnyOfTheFlagsInner(T flag)
	{
		return has(flag);
	}
	template<typename... Flags>
	bool hasAnyOfTheFlagsInner(T firstFlag, Flags... otherFlags)
	{
		return has(firstFlag) || hasAnyOfTheFlagsInner(otherFlags...);
	}

private:
	std::bitset<magic_enum::enum_count<T>()> mBitset;
};
