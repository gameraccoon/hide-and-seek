#include "Base/precomp.h"

#include "GameData/AI/NavMesh.h"

#include <DetourNavMesh.h>

NavMesh::NavMesh() {}
NavMesh::~NavMesh() {}

void NavMesh::setMesh(std::unique_ptr<dtNavMesh>&& newMesh)
{
	mMesh = std::move(newMesh);
}

dtNavMesh* NavMesh::getMesh()
{
	return mMesh.get();
}
