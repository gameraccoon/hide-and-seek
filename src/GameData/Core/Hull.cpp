#include "GameData/Core/Hull.h"

#include <nlohmann/json.hpp>

void Hull::generateBorders()
{
    borders.clear();
    for (size_t i = 0, count = points.size(); i < count; i++)
	{
		borders.insert(borders.end(), Border(points[i], points[(i + 1) % count]));
	}
}

bool operator==(const Hull& left, const Hull& right)
{
	return left.type == right.type &&
	((left.type == HullType::Angular && left.points == right.points)
	 ||
	 (left.type == HullType::Circular && left.radius == right.radius));
}

bool operator!=(const Hull& left, const Hull& right)
{
	return !(left == right);
}

float Hull::getQRadius() const
{
	return qRadius;
}

float Hull::getRadius() const
{
	return radius;
}

void Hull::setRadius(float newRadius)
{
	radius = newRadius;
	qRadius = newRadius * newRadius;
}

void to_json(nlohmann::json& outJson, const Hull& hull)
{
	outJson = nlohmann::json{
		{"points", hull.points},
		{"radius", hull.radius},
		{"type", hull.type}
	};
}

void from_json(const nlohmann::json& json, Hull& hull)
{
	json.at("points").get_to(hull.points);
	hull.setRadius(json.at("radius").get<float>());
	json.at("type").get_to(hull.type);
	hull.generateBorders();
}
