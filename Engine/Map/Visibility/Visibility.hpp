#include "Controller.hpp"
//==============================================================================================================
//Visibility.hpp
//by Albert Chen Sep-8-2015.
//==============================================================================================================

class Actor;
class Map;
struct TraceResult2D;


///----------------------------------------------------------------------------------------------------------
///field of view
void ResetTileVisibility(Map* map);

void CalcBruteForceVisiblityForActor(Actor* actor, Map* map);

void CalcBruteForceFieldOfViewForActor(Actor* actor, Map* map, const float& overrideRadius = 0.0f);

void CalcBruteForceVisibilityViewForActor(Actor* actor, Map* map, const float& overrideRadius = 0.0f, Controller* myController = NULL );

///----------------------------------------------------------------------------------------------------------
///visibility helpers
//TraceResult2D TileMapRayCast2D(LineSegment2 rayToTrace, Raycast2DIsImpactFunction* isImpact2DCallback, const float& tStep, Map* map, Tile* tileToTraceTo);