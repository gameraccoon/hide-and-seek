#pragma once

#include <typeindex>

#include "ECS/Component.h"


class JsonComponentSerializer
{
public:
	virtual void toJson(nlohmann::json& outJson, const BaseComponent* component) const = 0;
	virtual void fromJson(const nlohmann::json& json, BaseComponent* outComponent) const = 0;
};

class JsonComponentSerializationHolder
{
public:
	JsonComponentSerializationHolder() = default;
	JsonComponentSerializationHolder(JsonComponentSerializationHolder&) = delete;
	JsonComponentSerializationHolder& operator=(JsonComponentSerializationHolder&) = delete;

	const JsonComponentSerializer* getComponentSerializerFromTypeID(std::type_index typeID) const
	{
		const auto& it = mTypeIDToSerializer.find(typeID);
		if (it != mTypeIDToSerializer.end())
		{
			return mSerializers[it->second].get();
		}

		ReportFatalError("Unknown component type: '%s'", typeID.name());
		return nullptr;
	}

	const JsonComponentSerializer* getComponentSerializerFromClassName(StringID className) const
	{
		const auto& it = mClassNameToSerializer.find(className);
		if (it != mClassNameToSerializer.end())
		{
			return mSerializers[it->second].get();
		}

		ReportFatalError("Unknown component type: '%s'", className);
		return nullptr;
	}

	template<typename T>
	void registerSerializer(StringID className, std::unique_ptr<JsonComponentSerializer>&& serializer)
	{
		mSerializers.push_back(std::move(serializer));
		mTypeIDToSerializer.emplace(typeid(T), mSerializers.size() - 1);
		mClassNameToSerializer.emplace(className, mSerializers.size() - 1);
	}

private:
	std::vector<std::unique_ptr<JsonComponentSerializer>> mSerializers;
	std::unordered_map<std::type_index, unsigned int> mTypeIDToSerializer;
	std::unordered_map<StringID, unsigned int> mClassNameToSerializer;
};
