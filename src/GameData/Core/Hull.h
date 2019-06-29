#pragma once

#include <vector>

#include <nlohmann/json_fwd.hpp>

#include "GameData/Core/Vector2D.h"
#include "GameData/Core/Border.h"

class Hull
{
public:
	enum class Type
	{
		Angular
		,Circular
		,None
	};

	Hull();
	~Hull();

	/** Pre-calculated squared radius */
	float getQRadius() const;
	/** Get the radius */
	float getRadius() const;
	/** Set the radius */
	void setRadius(float newRadius);
	/** Calc borders from points */
	void generateBorders();

	friend bool operator==(const Hull& left, const Hull& right);
	friend bool operator!=(const Hull& left, const Hull& right);

	friend void to_json(nlohmann::json& outJson, const Hull& hull);
	friend void from_json(const nlohmann::json& json, Hull& hull);
	
	/** Corners of a hull's borders */
	std::vector<Vector2D> points;
	/** Borders of hull (calculates from Points) */
	std::vector<Border> borders;
	Type type;
private:
	/** Squared radius */
	float qRadius;
	/** Radius for the circular hull */
	float radius;
};
