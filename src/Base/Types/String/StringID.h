#pragma once

/**
 * String type used for technical identifiers that
 * don't need to be presented to the player in any form
 *
 * Can be fully replaced with integer values in production builds
 */
class StringID
{
public:
	friend struct std::hash<StringID>;
	friend class StringIDManager;

	using KeyType = uint64_t;

public:
	constexpr StringID() = default;

	[[nodiscard]] bool isValid() const noexcept { return hash != InvalidValue; }

	[[nodiscard]] bool operator ==(const StringID& other) const noexcept
	{
		return hash == other.hash;
	}

	[[nodiscard]] bool operator !=(const StringID& other) const noexcept
	{
		return hash != other.hash;
	}

	[[nodiscard]] bool operator <(const StringID& other) const noexcept
	{
		return hash < other.hash;
	}

	[[nodiscard]] bool operator >(const StringID& other) const noexcept
	{
		return hash > other.hash;
	}

	[[nodiscard]] bool operator <=(const StringID& other) const noexcept
	{
		return hash <= other.hash;
	}

	[[nodiscard]] bool operator >=(const StringID& other) const noexcept
	{
		return hash >= other.hash;
	}

private:
	constexpr StringID(KeyType hash) noexcept
		: hash(hash)
	{
	}

private:
	static const KeyType InvalidValue = 0;
	KeyType hash = InvalidValue;
};

/**
 * A singletone that converts strings to StringIDs and back.
 *
 * Don't use this class explicitly.
 * For conversations to StringID use STR_TO_ID on your string literals or runtime strings.
 * For getting a string from StringID use ID_TO_STR.
 */
class StringIDManager
{
public:
	StringIDManager();
	static StringIDManager& Instance();

	// don't call these functions explicitly
	// use STR_TO_ID macro as it being parsed by external code preprocessing tools)
	// see hideandseek/scripts/build/generators/generate_string_ids.py
	static constexpr StringID StringToID(const char* const stringLiteral) noexcept
	{
		return StringID(hash_64_fnv1a_const(stringLiteral));
	}

	static StringID StringToID(const std::string& string) noexcept
	{
		return StringIDManager::Instance().stringToID(string);
	}

	StringID stringToID(const std::string& stringValue);
	const std::string& getStringFromID(StringID id);

private:
	std::unordered_map<StringID::KeyType, std::string> mStringIDsToStringsMap;
};

#define STR_TO_ID(strLiteral) StringIDManager::StringToID(strLiteral)
#define ID_TO_STR(id) StringIDManager::Instance().getStringFromID(id)

namespace std
{
	template<> struct hash<StringID>
	{
		size_t operator()(const StringID& stringID) const noexcept
		{
			// it's already a unique hash
			return static_cast<size_t>(stringID.hash);
		}
	};
}

void to_json(nlohmann::json& outJson, const StringID& stringID);
void from_json(const nlohmann::json& json, StringID& stringID);
