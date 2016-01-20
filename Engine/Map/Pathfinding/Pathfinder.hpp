//==============================================================================================================
//Pathfinder.hpp
//by Albert Chen Sep-3-2015.
//==============================================================================================================

#include <map>
#include "Path.hpp"
#include "Engine/Multithreading/JobManager.hpp"

class Map;

///----------------------------------------------------------------------------------------------------------
///helpful pathfinding methods

//A* pathfinding algorithm
Path& CalcPath(Path& thePath, Map* map, const IntVec2& start, const IntVec2& goal, bool ignoreEntities = true);

void RecursiveCalcPath(Path& thePath, Map* map, const IntVec2& start, const IntVec2& goal, bool ignoreEntities = true);
//-----------------------------------------------------------------------------------------------------------

Path& ProcessOneStepOfPath(Path& thePath, Map* map, const IntVec2& start, const IntVec2& goal, bool ignoreEntities = true);

//-----------------------------------------------------------------------------------------------------------

void ProcessAdjacentMapTiles(Path& thePath, Map* map, const IntVec2& start, const IntVec2& goal, TilePtrs& adjacentTilePtrs);

//-----------------------------------------------------------------------------------------------------------

void ProcessAdjacentMapTile(Path& thePath, Tile* tile, Map* map, const IntVec2& start, const IntVec2& goal);

//-----------------------------------------------------------------------------------------------------------

void ReplaceOpenListPathNode(Path& thePath, PathNode* nodeInOpenList, const float& avoidCost = 1.0f);

//-----------------------------------------------------------------------------------------------------------

const float CalcHypotheticalGCostToAdjacentNodePosition(PathNode* currentActiveNode, const IntVec2& adjacentNodePos, const float& avoidCost = 1.0f );

//-----------------------------------------------------------------------------------------------------------

//render A* path
void RenderDebugPathMeshOnMap(OpenGLRenderer* renderer, MeshRenderer& pathMeshRenderer, Path& pathToRender, Map* map);

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///pathfinding job

struct CalcPathJob : Job{

	//vars
	MapPosition start;
	MapPosition goal;
	Map* myMap;
	bool ignoreEntities;
	Path* myPath;

	//constructor
	CalcPathJob(Path* newPath, Map* newMap, const MapPosition& newStart, const MapPosition& newGoal, bool doIgnoreEntities = true ) :
		start(newStart),
		goal(newGoal),
		myMap(newMap),
		myPath(newPath),
		ignoreEntities(doIgnoreEntities)
	{

	}
	//methods
	void Execute();

protected:
	//Assignment Operator
	const CalcPathJob& operator=(const CalcPathJob& jobToAssign){
		*this = jobToAssign;

		return *this;
	}

};

///----------------------------------------------------------------------------------------------------------
///pathfinding job helpers

void AddCalcPathJob(Path& thePath, Map* map, const IntVec2& start, const IntVec2& goal, bool ignoreEntities = true);

//===========================================================================================================