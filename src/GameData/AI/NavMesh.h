#pragma once

#include <memory>

class dtNavMesh;

class NavMesh
{
public:
	NavMesh();
	~NavMesh();
	void setMesh(std::unique_ptr<dtNavMesh>&& newMesh);
	dtNavMesh* getMesh();

private:
	std::unique_ptr<dtNavMesh> mMesh;
};
