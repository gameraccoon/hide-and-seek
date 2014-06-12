#include <Engine/Modules/ActorFactory.h>


#include <Engine/Actors/Bullet.h>
IActor* CreateBullet(World *world, const Vector2D location, const Vector2D scale, const Rotator rotation)
{
	return new Bullet(world, location, scale, rotation);
}

#include <Engine/Actors/LightEmitter.h>
IActor* CreateLightEmitter(World *world, const Vector2D location, const Vector2D scale, const Rotator rotation)
{
	return new LightEmitter(world, location, scale, rotation);
}

#include <Game/Actors/Wall.h>
IActor* CreateWall(World *world, const Vector2D location, const Vector2D scale, const Rotator rotation)
{
	return new Wall(world, location, scale, rotation);
}

#include <Game/Actors/Man.h>
IActor* CreateMan(World *world, const Vector2D location, const Vector2D scale, const Rotator rotation)
{
	return new Man(world, location, scale, rotation);
}

#include <Game/Actors/Corpse.h>
IActor* CreateCorpse(World *world, const Vector2D location, const Vector2D scale, const Rotator rotation)
{
	return new Corpse(world, location, scale, rotation);
}


namespace FactoryActors
{
	void RegisterAll()
	{
		ActorFactory::Factory().registerActor("Bullet", ::CreateBullet);
		ActorFactory::Factory().registerActor("LightEmitter", ::CreateLightEmitter);
		ActorFactory::Factory().registerActor("Wall", ::CreateWall);
		ActorFactory::Factory().registerActor("Man", ::CreateMan);
		ActorFactory::Factory().registerActor("Corpse", ::CreateCorpse);
	}
}