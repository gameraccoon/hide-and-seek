#include "Base/String/StringID.h"

#include <nlohmann/json.hpp>

void to_json(nlohmann::json& outJson, const StringID& path)
{
	outJson = nlohmann::json::object({path.c_str()});
}

void from_json(const nlohmann::json& json, StringID& path)
{
	path = static_cast<StringID>(json.get<std::string>());
}
