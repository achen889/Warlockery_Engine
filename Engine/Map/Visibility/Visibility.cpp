//==============================================================================================================
//Visibility.cpp
//by Albert Chen Sep-8-2015.
//==============================================================================================================

#include "Visibility.hpp"
//#include "Actor.hpp"
#include "Engine/Math/Math2D.hpp"
#include <algorithm>
#include "../Map.hpp"
//#include "GameCommon.hpp"

///----------------------------------------------------------------------------------------------------------
///helpful methods

void ResetTileVisibility(Map* map){
	//reset tile visibility
	for (TilesIterator it = map->m_tiles.begin(); it != map->m_tiles.end(); ++it){
		Tile& tile = (*it);

		tile.ResetVisibility();
	}
}

//-----------------------------------------------------------------------------------------------------------

void CalcBruteForceLineTraceToTile(const Tile& startTile, Tile* tileToCheck, Map* map) {
	if (!tileToCheck)return;
	//ray trace to tile here
	LineSegment2 rayToTraceToTile = LineSegment2(startTile.GetMapPositionVec2(), tileToCheck->GetMapPositionVec2());
	const float tStep = 0.1f;
	for (float t = 0.0f; t < 1.0f; t += tStep) {

		Vector2 rayToTracePointAtT = rayToTraceToTile.GetPointAtParametricValue(t);

		Tile* tileAtRayTrace = map->GetTileAtMapPosition(ToIntVec2(rayToTracePointAtT));
		//does the visibility flags stuff
		if (tileAtRayTrace) {

			if (tileAtRayTrace->IsVisible())
				tileAtRayTrace->m_tileState.visibility.SetHasBeenSeen(true);// .m_hasBeenSeen = true;

			tileAtRayTrace->m_tileState.visibility.SetIsVisible(true);// Visibility.m_isVisible = true;

			if (tileAtRayTrace->m_type == TILE_TYPE_SOLID) {
				tileAtRayTrace->m_tileState.visibility.SetIsVisible(true);//m_tileVisibility.m_isVisible = true;
				break;
			}//end of innermost if

		}//end of is tile at ray trace valid

	}//end of raytrace for
}

//-----------------------------------------------------------------------------------------------------------

void CalcBruteForceVisiblityForTile(const Tile& startTile, const float& sightRadius, Map* map) {
	if (map) {
		TilePtrs tilesInSightRadius = map->GetAllTilesWithinRadiusOfTile(startTile, sightRadius);
		for (TilePtrsIterator it = tilesInSightRadius.begin(); it != tilesInSightRadius.end(); ++it) {
			Tile* tileInRadius = (*it);
			if (tileInRadius) {
				CalcBruteForceLineTraceToTile(startTile, tileInRadius, map);
			}//end of is tileInRadius Valid
		}//end of for
	}//end of map valid
}

//-----------------------------------------------------------------------------------------------------------

//===========================================================================================================