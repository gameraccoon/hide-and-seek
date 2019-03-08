#include <Modules/ActorFactory.h>


#include <Actors/Bullet.h>
IActor* CreateBullet(World *world, const Vector2D location, const Vector2D scale, const Rotator rotation)
{
	return new Bullet(world, location, scale, rotation);
}

#include <Actors/LightEmitter.h>
IActor* CreateLightEmitter(World *world, const Vector2D location, const Vector2D /*scale*/, const Rotator rotation)
{
	return new LightEmitter(world, location, rotation);
}

#include <Actors/Wall.h>
IActor* CreateWall(World *world, const Vector2D location, const Vector2D scale, const Rotator rotation)
{
	return new Wall(world, location, scale, rotation);
}

#include <Actors/Man.h>
IActor* CreateMan(World *world, const Vector2D location, const Vector2D /*scale*/, const Rotator /*rotation*/)
{
	Man *man = new Man(world, location);
	man->giveWeapon(new Weapon);
	return man;
}

#include <Actors/Hero.h>
IActor* CreateHero(World *world, const Vector2D location, const Vector2D /*scale*/, const Rotator /*rotation*/)
{
	Hero *hero = new Hero(world, location);
	hero->giveWeapon(new Weapon);
	return hero;
}

#include <Actors/Corpse.h>
IActor* CreateCorpse(World *world, const Vector2D location, const Vector2D /*scale*/, const Rotator rotation)
{
	return new Corpse(world, location, rotation);
}


namespace FactoryActors
{
	void RegisterAll()
	{
		ActorFactory::Factory().registerActor("Bullet", ::CreateBullet);
		ActorFactory::Factory().registerActor("LightEmitter", ::CreateLightEmitter);
		ActorFactory::Factory().registerActor("Wall", ::CreateWall);
		ActorFactory::Factory().registerActor("Man", ::CreateMan);
		ActorFactory::Factory().registerActor("Hero", ::CreateHero);
		ActorFactory::Factory().registerActor("Corpse", ::CreateCorpse);
	}
}
