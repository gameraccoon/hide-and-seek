#include "Base/String/LocalizableText.h"

#include <nlohmann/json.hpp>

void to_json(nlohmann::json& outJson, const TextString& path)
{
	outJson = nlohmann::json::object({path.c_str()});
}

void from_json(const nlohmann::json& json, TextString& path)
{
	path = static_cast<TextString>(json.get<std::string>());
}
