#include "GameLogic/Systems/AiSystem.h"

#include "GameData/Components/NavMeshComponent.generated.h"
#include "GameData/Components/AiControllerComponent.generated.h"
#include "GameData/Components/CollisionComponent.generated.h"
#include "GameData/Components/TransformComponent.generated.h"
#include "GameData/Components/MovementComponent.generated.h"
#include "GameData/Components/CharacterStateComponent.generated.h"
#include "GameData/World.h"

#include "Utils/AI/NavMeshGenerator.h"

#include <DetourCommon.h>
#include <DetourNavMeshQuery.h>
#include <string.h>


inline bool inRange(const float* v1, const float* v2, const float r, const float h)
{
	const float dx = v2[0] - v1[0];
	const float dy = v2[1] - v1[1];
	const float dz = v2[2] - v1[2];
	return (dx*dx + dz*dz) < r*r && fabsf(dy) < h;
}

static bool getSteerTarget(dtNavMeshQuery* navQuery, const float* startPos, const float* endPos,
						   const float minTargetDist,
						   const dtPolyRef* path, const int pathSize,
						   float* steerPos, unsigned char& steerPosFlag, dtPolyRef& steerPosRef,
						   float* outPoints = nullptr, int* outPointCount = nullptr)
{
	// Find steer target.
	static const int MAX_STEER_POINTS = 3;
	float steerPath[MAX_STEER_POINTS*3];
	unsigned char steerPathFlags[MAX_STEER_POINTS];
	dtPolyRef steerPathPolys[MAX_STEER_POINTS];
	int nsteerPath = 0;
	navQuery->findStraightPath(startPos, endPos, path, pathSize,
							   steerPath, steerPathFlags, steerPathPolys, &nsteerPath, MAX_STEER_POINTS);
	if (!nsteerPath) {
		return false;
	}

	if (outPoints && outPointCount)
	{
		*outPointCount = nsteerPath;
		for (int i = 0; i < nsteerPath; ++i) {
			dtVcopy(&outPoints[i*3], &steerPath[i*3]);
		}
	}


	// Find vertex far enough to steer to.
	int ns = 0;
	while (ns < nsteerPath)
	{
		// Stop at Off-Mesh link or when point is further than slop away.
		if ((steerPathFlags[ns] & DT_STRAIGHTPATH_OFFMESH_CONNECTION) ||
			!inRange(&steerPath[ns*3], startPos, minTargetDist, 1000.0f)) {
			break;
		}
		ns++;
	}
	// Failed to find good point to steer to.
	if (ns >= nsteerPath) {
		return false;
	}

	dtVcopy(steerPos, &steerPath[ns*3]);
	steerPos[1] = startPos[1];
	steerPosFlag = steerPathFlags[ns];
	steerPosRef = steerPathPolys[ns];

	return true;
}

template<class T> inline T rcMin(T a, T b) { return a < b ? a : b; }
template<class T> inline T rcMax(T a, T b) { return a > b ? a : b; }

static int fixupCorridor(dtPolyRef* path, const int npath, const int maxPath,
						 const dtPolyRef* visited, const int nvisited)
{
	int furthestPath = -1;
	int furthestVisited = -1;

	// Find furthest common polygon.
	for (int i = npath-1; i >= 0; --i)
	{
		bool found = false;
		for (int j = nvisited-1; j >= 0; --j)
		{
			if (path[i] == visited[j])
			{
				furthestPath = i;
				furthestVisited = j;
				found = true;
			}
		}
		if (found) {
			break;
		}
	}

	// If no intersection found just return current path.
	if (furthestPath == -1 || furthestVisited == -1) {
		return npath;
	}

	// Concatenate paths.

	// Adjust beginning of the buffer to include the visited.
	const int req = nvisited - furthestVisited;
	const int orig = rcMin(furthestPath+1, npath);
	int size = rcMax(0, npath-orig);
	if (req+size > maxPath) {
		size = maxPath-req;
	}
	if (size) {
		memmove(path+req, path+orig, size*sizeof(dtPolyRef));
	}

	// Store visited
	for (int i = 0; i < req; ++i) {
		path[i] = visited[(nvisited-1)-i];
	}

	return req+size;
}

// This function checks if the path has a small U-turn, that is,
// a polygon further in the path is adjacent to the first polygon
// in the path. If that happens, a shortcut is taken.
// This can happen if the target (T) location is at tile boundary,
// and we're (S) approaching it parallel to the tile edge.
// The choice at the vertex can be arbitrary,
//  +---+---+
//  |:::|:::|
//  +-S-+-T-+
//  |:::|   | <-- the step can end up in here, resulting U-turn path.
//  +---+---+
static int fixupShortcuts(dtPolyRef* path, int npath, dtNavMeshQuery* navQuery)
{
	if (npath < 3)
		return npath;

	// Get connected polygons
	static const int maxNeis = 16;
	dtPolyRef neis[maxNeis];
	int nneis = 0;

	const dtMeshTile* tile = nullptr;
	const dtPoly* poly = nullptr;
	if (dtStatusFailed(navQuery->getAttachedNavMesh()->getTileAndPolyByRef(path[0], &tile, &poly))) {
		return npath;
	}

	for (unsigned int k = poly->firstLink; k != DT_NULL_LINK; k = tile->links[k].next)
	{
		const dtLink* link = &tile->links[k];
		if (link->ref != 0)
		{
			if (nneis < maxNeis) {
				neis[nneis++] = link->ref;
			}
		}
	}

	// If any of the neighbour polygons is within the next few polygons
	// in the path, short cut to that polygon directly.
	static const int maxLookAhead = 6;
	int cut = 0;
	for (int i = dtMin(maxLookAhead, npath) - 1; i > 1 && cut == 0; i--) {
		for (int j = 0; j < nneis; j++)
		{
			if (path[i] == neis[j]) {
				cut = i;
				break;
			}
		}
	}
	if (cut > 1)
	{
		int offset = cut-1;
		npath -= offset;
		for (int i = 1; i < npath; i++) {
			path[i] = path[i+offset];
		}
	}

	return npath;
}

constexpr int MAX_SMOOTH = 2048;

static void RecalcNavmesh(dtNavMesh* m_navMesh, dtNavMeshQuery* m_navQuery, float* m_spos, float* m_epos, float* m_polyPickExt, float* m_smoothPath, int& m_nsmoothPath)
{
	if (!m_navMesh) {
		return;
	}

	constexpr int MAX_POLYS = 2048;

	dtPolyRef m_startRef;
	dtPolyRef m_endRef;

	dtQueryFilter m_filter;

	m_navQuery->findNearestPoly(m_spos, m_polyPickExt, &m_filter, &m_startRef, nullptr);
	m_navQuery->findNearestPoly(m_epos, m_polyPickExt, &m_filter, &m_endRef, nullptr);

	if (m_startRef && m_endRef)
	{
		int m_npolys;
		dtPolyRef m_polys[MAX_POLYS];
		m_navQuery->findPath(m_startRef, m_endRef, m_spos, m_epos, &m_filter, m_polys, &m_npolys, MAX_POLYS);

		if (m_npolys)
		{
			// Iterate over the path to find smooth path on the detail mesh surface.
			dtPolyRef polys[MAX_POLYS];
			memcpy(polys, m_polys, sizeof(dtPolyRef)*m_npolys);
			int npolys = m_npolys;

			float iterPos[3], targetPos[3];
			m_navQuery->closestPointOnPoly(m_startRef, m_spos, iterPos, nullptr);
			m_navQuery->closestPointOnPoly(polys[npolys-1], m_epos, targetPos, nullptr);

			constexpr float STEP_SIZE = 50.0f;
			constexpr float SLOP = 0.0001f;

			dtVcopy(&m_smoothPath[m_nsmoothPath*3], iterPos);
			m_nsmoothPath++;

			// Move towards target a small advancement at a time until target reached or
			// when ran out of memory to store the path.
			while (npolys && m_nsmoothPath < MAX_SMOOTH)
			{
				// Find location to steer towards.
				float steerPos[3];
				unsigned char steerPosFlag;
				dtPolyRef steerPosRef;

				if (!getSteerTarget(m_navQuery, iterPos, targetPos, SLOP,
									polys, npolys, steerPos, steerPosFlag, steerPosRef)) {
					break;
				}

				bool endOfPath = (steerPosFlag & DT_STRAIGHTPATH_END) ? true : false;
				bool offMeshConnection = (steerPosFlag & DT_STRAIGHTPATH_OFFMESH_CONNECTION) ? true : false;

				// Find movement delta.
				float delta[3], len;
				dtVsub(delta, steerPos, iterPos);
				len = dtMathSqrtf(dtVdot(delta, delta));
				// If the steer target is end of path or off-mesh link, do not move past the location.
				if ((endOfPath || offMeshConnection) && len < STEP_SIZE) {
					len = 1;
				}
				else {
					len = STEP_SIZE / len;
				}
				float moveTgt[3];
				dtVmad(moveTgt, iterPos, delta, len);

				// Move
				float result[3];
				dtPolyRef visited[16];
				int nvisited = 0;
				m_navQuery->moveAlongSurface(polys[0], iterPos, moveTgt, &m_filter,
											 result, visited, &nvisited, 16);

				npolys = fixupCorridor(polys, npolys, MAX_POLYS, visited, nvisited);
				npolys = fixupShortcuts(polys, npolys, m_navQuery);

				float h = 0;
				m_navQuery->getPolyHeight(polys[0], result, &h);
				result[1] = h;
				dtVcopy(iterPos, result);

				// Handle end of path and off-mesh links when close enough.
				if (endOfPath && inRange(iterPos, steerPos, SLOP, 1.0f))
				{
					// Reached end of path.
					dtVcopy(iterPos, targetPos);
					if (m_nsmoothPath < MAX_SMOOTH)
					{
						dtVcopy(&m_smoothPath[m_nsmoothPath*3], iterPos);
						m_nsmoothPath++;
					}
					break;
				}
				else if (offMeshConnection && inRange(iterPos, steerPos, SLOP, 1.0f))
				{
					// Reached off-mesh connection.
					float startPos[3], endPos[3];

					// Advance the path up to and over the off-mesh connection.
					dtPolyRef prevRef = 0, polyRef = polys[0];
					int npos = 0;
					while (npos < npolys && polyRef != steerPosRef)
					{
						prevRef = polyRef;
						polyRef = polys[npos];
						npos++;
					}
					for (int i = npos; i < npolys; ++i) {
						polys[i-npos] = polys[i];
					}
					npolys -= npos;

					// Handle the connection.
					dtStatus status = m_navMesh->getOffMeshConnectionPolyEndPoints(prevRef, polyRef, startPos, endPos);
					if (dtStatusSucceed(status))
					{
						if (m_nsmoothPath < MAX_SMOOTH)
						{
							dtVcopy(&m_smoothPath[m_nsmoothPath*3], startPos);
							m_nsmoothPath++;
							// Hack to make the dotted path not visible during off-mesh connection.
							if (m_nsmoothPath & 1)
							{
								dtVcopy(&m_smoothPath[m_nsmoothPath*3], startPos);
								m_nsmoothPath++;
							}
						}
						// Move position at the other side of the off-mesh link.
						dtVcopy(iterPos, endPos);
						float eh = 0.0f;
						m_navQuery->getPolyHeight(polys[0], iterPos, &eh);
						iterPos[1] = eh;
					}
				}

				// Store results.
				if (m_nsmoothPath < MAX_SMOOTH)
				{
					dtVcopy(&m_smoothPath[m_nsmoothPath*3], iterPos);
					m_nsmoothPath++;
				}
			}
		}
	}
}


AiSystem::AiSystem(WorldHolder &worldHolder)
	: mWorldHolder(worldHolder)
{
}

void AiSystem::update()
{
	World* world = mWorldHolder.world;

	auto [navMeshComponent] = world->getWorldComponents().getComponents<NavMeshComponent>();

	if (navMeshComponent == nullptr)
	{
		return;
	}

	// ToDo: we recalculate navmesh every frame to be able to work with worst-case scenario as long as possible
	// optimizations such as dirty flag and spatial hash are on the way to be impelemnted
	NavMeshGenerator generator;
	std::vector<std::tuple<CollisionComponent*, TransformComponent*>> collisions = world->getEntityManager().getComponents<CollisionComponent, TransformComponent>();
	generator.generateNavMesh(navMeshComponent->getNavMeshRef(), collisions);

	OptionalEntity playerEntity = world->getPlayerControlledEntity();
	if (!playerEntity.isValid())
	{
		return;
	}

	auto [playerTransform] = world->getEntityManager().getEntityComponents<TransformComponent>(playerEntity.getEntity());
	if (playerTransform == nullptr)
	{
		return;
	}

	dtNavMesh* navMesh = navMeshComponent->getNavMeshRef().getMesh();
	if (navMesh == nullptr)
	{
		return;
	}

	Vector2D targetLocation = playerTransform->getLocation();

	world->getEntityManager().forEachComponentSet<AiControllerComponent, TransformComponent, MovementComponent, CharacterStateComponent>([targetLocation, navMesh](AiControllerComponent* aiController, TransformComponent* transform, MovementComponent* movement, CharacterStateComponent* characterState)
	{
		Vector2D startLocation = transform->getLocation();

		dtNavMeshQuery query;
		query.init(navMesh, 256);
		float startPos[3];
		startPos[0] = startLocation.x;
		startPos[1] = 0.0f;
		startPos[2] = startLocation.y;
		float endPos[3];
		endPos[0] = targetLocation.x;
		endPos[1] = 0.0f;
		endPos[2] = targetLocation.y;
		float halfExtents[3] = {20.0f, 5.0f, 20.0f};

		int m_nsmoothPath = 0;
		float m_smoothPath[MAX_SMOOTH*3];

		RecalcNavmesh(navMesh, &query, startPos, endPos, halfExtents, m_smoothPath, m_nsmoothPath);

		std::vector<Vector2D> &path = aiController->getPathRef().getSmoothPathRef();
		path.resize(static_cast<size_t>(m_nsmoothPath / 3));
		for (size_t i = 0; i < path.size(); ++i)
		{
			path[i].x = m_smoothPath[i*3];
			path[i].y = m_smoothPath[i*3 + 2];
		}

		if (path.size() > 1)
		{
			Vector2D diff = path[1] - startLocation;
			movement->setMoveDirection(diff);
		}

		characterState->getBlackboardRef().setValue<bool>(CharacterStateBlackboardKeys::TryingToMove, path.size() > 1);
	});
}