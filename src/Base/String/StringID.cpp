#include "Base/String/StringID.h"

#include <nlohmann/json.hpp>

#include "Base/Debug/Assert.h"
#include "Base/String/GatheredStringIDs.generated.h"

void to_json(nlohmann::json& outJson, const StringID& stringID)
{
	outJson = nlohmann::json(ID_TO_STR(stringID));
}

void from_json(const nlohmann::json& json, StringID& stringID)
{
	stringID = STR_TO_ID(json.get<std::string>());
}

StringIDManager::StringIDManager()
	: mStringIDsToStringsMap(getGatheredStringIDs())
{
}

StringIDManager& StringIDManager::Instance()
{
	static StringIDManager instance;
	return instance;
}

StringID StringIDManager::stringToID(const std::string& stringValue)
{
	const StringID::KeyType hash = hash_64_fnv1a_const(stringValue.c_str());
	AssertFatal(hash != 0UL, "String hashing result should not be 0: '%s'", stringValue.c_str());
	auto [it, hasInserted] = mStringIDsToStringsMap.emplace(hash, stringValue);
	AssertFatal(hasInserted || it->second == stringValue, "Hash collision for '%s' and '%s'", stringValue.c_str(), it->second.c_str());
	return StringID(hash);
}

std::string StringIDManager::getStringFromID(StringID id)
{
	auto it = mStringIDsToStringsMap.find(id.hash);
	if (it != mStringIDsToStringsMap.end())
	{
		return it->second;
	}
	else
	{
		ReportError("String representation for id '%d' not found", id.hash);
		return std::string();
	}
}

bool operator ==(const StringID& a, const StringID& b)
{
	return a.hash == b.hash;
}

bool operator !=(const StringID& a, const StringID& b)
{
	return a.hash != b.hash;
}

bool operator <(const StringID& a, const StringID& b)
{
	return a.hash < b.hash;
}

bool operator >(const StringID& a, const StringID& b)
{
	return a.hash > b.hash;
}

bool operator <=(const StringID& a, const StringID& b)
{
	return a.hash <= b.hash;
}

bool operator >=(const StringID& a, const StringID& b)
{
	return a.hash >= b.hash;
}

static_assert(sizeof(StringID) == sizeof(uint64_t), "StringID is too big");
static_assert(std::is_trivially_copyable<StringID>(), "StringID should be trivially copyable");
