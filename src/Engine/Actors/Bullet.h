#ifndef BULLET_H
#define BULLET_H

#include "../Core/World.h"
#include "../Actors/Actor.h"
#include "../Modules/RayTrace.h"

class Bullet : public Actor
{
public:
	Bullet(World *world, Vector2D location, Vector2D scale, Rotator rotation);
	~Bullet(void);
	/** Process moving and other actions of the Actor */
	virtual void update(float deltatime) override;
	/** Take some damage to the bullet */
	virtual void takeDamage(float damageValue, Vector2D impulse) override;
protected:
	/** Update classId and actorId */
	virtual void updateActorId(std::string classId) override;
	virtual void updateCollision() override;
	float speed;
};

#endif
