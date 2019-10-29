#pragma once

#include <string>
#include <type_traits>

#include <nlohmann/json_fwd.hpp>

/**
 * String type used for technical identifiers that
 * don't need to be presented to the player in any form
 *
 * Can be fully replaced with integer values in production builds
 */
class StringID : public std::string
{
public:
	using std::string::string;
	explicit StringID(std::string&& val) : std::string(val) {}
	explicit StringID(const std::string& val) : std::string(val) {}
};

namespace std
{
	template<> struct hash<StringID>
	{
		std::size_t operator()(StringID const& path) const noexcept
		{
			return std::hash<std::string>{}(static_cast<std::string>(path));
		}
	};
}

void to_json(nlohmann::json& outJson, const StringID& path);
void from_json(const nlohmann::json& json, StringID& path);

//static_assert(sizeof(StringID) == sizeof(int)*2, "StringID is too big");
//static_assert(std::is_trivially_copyable<StringID>(), "StringID should be trivially copyable");
