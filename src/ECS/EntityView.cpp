#include "Base/precomp.h"

#include "ECS/EntityView.h"

#include "Base/Debug/Assert.h"

EntityView::EntityView(Entity entity, EntityManager& manager)
	: mEntity(entity)
	, mManager(manager)
{
}

Entity EntityView::getEntity() const
{
	return mEntity;
}
