#pragma once

#include "Core/World.h"
#include "Core/Actor.h"
#include "Modules/RayTrace.h"

class Bullet : public Actor
{
public:
	Bullet(World *world, Vector2D location, Vector2D scale, Rotator rotation);
	~Bullet() override;
	/** Process moving and other actions of the Actor */
	virtual void update(float deltatime) override;

protected:
	/** Update classId and actorId */
	virtual void updateActorId(std::string classId) override;
	float mSpeed;
};
