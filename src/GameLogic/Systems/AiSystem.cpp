#include "Base/precomp.h"

#include "GameLogic/Systems/AiSystem.h"

#include "GameData/Components/NavMeshComponent.generated.h"
#include "GameData/Components/AiControllerComponent.generated.h"
#include "GameData/Components/CollisionComponent.generated.h"
#include "GameData/Components/TransformComponent.generated.h"
#include "GameData/Components/MovementComponent.generated.h"
#include "GameData/Components/CharacterStateComponent.generated.h"
#include "GameData/Components/TrackedSpatialEntitiesComponent.generated.h"
#include "GameData/Components/DebugDrawComponent.generated.h"
#include "GameData/World.h"
#include "GameData/GameData.h"

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
		memmove(path+req, path+orig, static_cast<size_t>(size)*sizeof(dtPolyRef));
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

static void RecalcNavmesh(dtNavMesh* navMesh, dtNavMeshQuery* navQuery, float* spos, float* epos, float* polyPickExt, float* smoothPath, int& nsmoothPath)
{
	if (!navMesh) {
		return;
	}

	constexpr int MAX_POLYS = 2048;

	dtPolyRef startRef;
	dtPolyRef endRef;

	dtQueryFilter filter;

	navQuery->findNearestPoly(spos, polyPickExt, &filter, &startRef, nullptr);
	navQuery->findNearestPoly(epos, polyPickExt, &filter, &endRef, nullptr);

	if (startRef && endRef)
	{
		int orig_npolys;
		dtPolyRef orig_polys[MAX_POLYS];
		navQuery->findPath(startRef, endRef, spos, epos, &filter, orig_polys, &orig_npolys, MAX_POLYS);

		if (orig_npolys)
		{
			// Iterate over the path to find smooth path on the detail mesh surface.
			dtPolyRef polys[MAX_POLYS];
			memcpy(polys, orig_polys, sizeof(dtPolyRef)*static_cast<size_t>(orig_npolys));
			int npolys = orig_npolys;

			float iterPos[3], targetPos[3];
			navQuery->closestPointOnPoly(startRef, spos, iterPos, nullptr);
			navQuery->closestPointOnPoly(polys[npolys-1], epos, targetPos, nullptr);

			constexpr float STEP_SIZE = 50.0f;
			constexpr float SLOP = 0.0001f;

			dtVcopy(&smoothPath[nsmoothPath*3], iterPos);
			nsmoothPath++;

			// Move towards target a small advancement at a time until target reached or
			// when ran out of memory to store the path.
			while (npolys && nsmoothPath < MAX_SMOOTH)
			{
				// Find location to steer towards.
				float steerPos[3];
				unsigned char steerPosFlag;
				dtPolyRef steerPosRef;

				if (!getSteerTarget(navQuery, iterPos, targetPos, SLOP,
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
				navQuery->moveAlongSurface(polys[0], iterPos, moveTgt, &filter,
											 result, visited, &nvisited, 16);

				npolys = fixupCorridor(polys, npolys, MAX_POLYS, visited, nvisited);
				npolys = fixupShortcuts(polys, npolys, navQuery);

				float h = 0;
				navQuery->getPolyHeight(polys[0], result, &h);
				result[1] = h;
				dtVcopy(iterPos, result);

				// Handle end of path and off-mesh links when close enough.
				if (endOfPath && inRange(iterPos, steerPos, SLOP, 1.0f))
				{
					// Reached end of path.
					dtVcopy(iterPos, targetPos);
					if (nsmoothPath < MAX_SMOOTH)
					{
						dtVcopy(&smoothPath[nsmoothPath*3], iterPos);
						nsmoothPath++;
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
					dtStatus status = navMesh->getOffMeshConnectionPolyEndPoints(prevRef, polyRef, startPos, endPos);
					if (dtStatusSucceed(status))
					{
						if (nsmoothPath < MAX_SMOOTH)
						{
							dtVcopy(&smoothPath[nsmoothPath*3], startPos);
							nsmoothPath++;
							// Hack to make the dotted path not visible during off-mesh connection.
							if (nsmoothPath & 1)
							{
								dtVcopy(&smoothPath[nsmoothPath*3], startPos);
								nsmoothPath++;
							}
						}
						// Move position at the other side of the off-mesh link.
						dtVcopy(iterPos, endPos);
						float eh = 0.0f;
						navQuery->getPolyHeight(polys[0], iterPos, &eh);
						iterPos[1] = eh;
					}
				}

				// Store results.
				if (nsmoothPath < MAX_SMOOTH)
				{
					dtVcopy(&smoothPath[nsmoothPath*3], iterPos);
					nsmoothPath++;
				}
			}
		}
	}
}


AiSystem::AiSystem(WorldHolder& worldHolder, const TimeData& timeData)
	: mWorldHolder(worldHolder)
	, mTime(timeData)
{
}

void AiSystem::update()
{
	World& world = mWorldHolder.getWorld();
	const GameplayTimestamp timestampNow = mTime.currentTimestamp;

	auto [navMeshComponent] = world.getWorldComponents().getComponents<NavMeshComponent>();

	if (navMeshComponent == nullptr)
	{
		return;
	}

	TupleVector<CollisionComponent*, TransformComponent*> collisions;
	world.getSpatialData().getAllCellManagers().getComponents<CollisionComponent, TransformComponent>(collisions);

	bool needUpdate = navMeshComponent->getNavMeshRef().getMesh() == nullptr;
	if (!needUpdate)
	{
		needUpdate = std::any_of(std::begin(collisions), std::end(collisions), [lastUpdateTimestamp = navMeshComponent->getUpdateTimestamp()](const std::tuple<CollisionComponent*, TransformComponent*>& set)
		{
			GameplayTimestamp objectUpdateTimestamp = std::get<1>(set)->getUpdateTimestamp();
			return objectUpdateTimestamp > lastUpdateTimestamp && std::get<0>(set)->getGeometry().type == HullType::Angular;
		});
	}

	if (needUpdate)
	{
		NavMeshGenerator generator;
		generator.generateNavMesh(navMeshComponent->getNavMeshRef(), collisions);
		navMeshComponent->setUpdateTimestamp(timestampNow);
	}

	std::optional<std::pair<EntityView, CellPos>> playerEntity = world.getTrackedSpatialEntity(STR_TO_ID("ControlledEntity"));

	if (!playerEntity.has_value())
	{
		return;
	}

	auto [playerTransform] = playerEntity->first.getComponents<TransformComponent>();
	if (playerTransform == nullptr)
	{
		return;
	}

	dtNavMesh* navMesh = navMeshComponent->getNavMeshRef().getMesh();
	if (navMesh == nullptr)
	{
		return;
	}

	auto [debugDraw] = mWorldHolder.getGameData().getGameComponents().getComponents<DebugDrawComponent>();

	Vector2D targetLocation = playerTransform->getLocation();

	world.getSpatialData().getAllCellManagers().forEachComponentSet<AiControllerComponent, TransformComponent, MovementComponent, CharacterStateComponent>([targetLocation, navMesh, debugDraw, timestampNow](AiControllerComponent* aiController, TransformComponent* transform, MovementComponent* movement, CharacterStateComponent* characterState)
	{
		Vector2D currentLocation = transform->getLocation();

		std::vector<Vector2D> &path = aiController->getPathRef().getSmoothPathRef();
		if (path.empty())
		{
			dtNavMeshQuery query;
			query.init(navMesh, 256);
			float startPos[3];
			startPos[0] = currentLocation.x;
			startPos[1] = 0.0f;
			startPos[2] = currentLocation.y;
			float endPos[3];
			endPos[0] = targetLocation.x;
			endPos[1] = 0.0f;
			endPos[2] = targetLocation.y;
			float halfExtents[3] = {40.0f, 5.0f, 40.0f};

			int nsmoothPath = 0;
			float smoothPath[MAX_SMOOTH*3];

			RecalcNavmesh(navMesh, &query, startPos, endPos, halfExtents, smoothPath, nsmoothPath);

			size_t pointsCount = static_cast<size_t>(nsmoothPath / 3);
			path.reserve(pointsCount);
			for (size_t i = 0; i < pointsCount; ++i)
			{
				Vector2D newPoint(smoothPath[i*3], smoothPath[i*3 + 2]);
				if (path.empty() || newPoint != path.back())
				{
					path.push_back(std::move(newPoint));
				}
			}

			for (size_t i = 1; i < path.size(); ++i)
			{
				debugDraw->getWorldLineSegmentsRef().emplace_back(path[i - 1], path[i], timestampNow.getIncreasedByFloatTime(10.0f));
			}

			characterState->getBlackboardRef().setValue<bool>(CharacterStateBlackboardKeys::TryingToMove, path.size() > 1);
		}

		if (!path.empty())
		{
			if ((path[0] - currentLocation).qSize() < 100.0f)
			{
				path.erase(path.begin());
			}
		}

		if (!path.empty())
		{
			Vector2D diff = path[1] - currentLocation;
			movement->setMoveDirection(diff);
		}
		else
		{
			movement->setMoveDirection(ZERO_VECTOR);
		}
	});
}
