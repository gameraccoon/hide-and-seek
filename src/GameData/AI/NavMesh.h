#pragma once

class dtNavMesh;

class NavMesh
{
public:
	~NavMesh();

	void setMesh(dtNavMesh* newMesh);
	dtNavMesh* getMesh();

private:
	dtNavMesh* mMesh = nullptr;
};
