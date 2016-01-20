//==============================================================================================================
//Visibility.cpp
//by Albert Chen Sep-8-2015.
//==============================================================================================================

#include "Visibility.hpp"
#include "Tile.hpp"
#include "Actor.hpp"
#include "Engine\Math\Math2D.hpp"
#include <algorithm>
#include "GameCommon.hpp"

///----------------------------------------------------------------------------------------------------------
///helpful methods

void ResetTileVisibility(Map* map){
	//reset tile visibility
	for (TilesIterator it = map->m_tiles.begin(); it != map->m_tiles.end(); ++it){
		Tile& tile = (*it);

		if (tile.m_tileVisibility.m_isVisible)
			tile.m_tileVisibility.m_hasBeenSeen = true;

		tile.m_tileVisibility.m_isVisible = false;

	}
}

//-----------------------------------------------------------------------------------------------------------

void CalcBruteForceVisiblityForActor(Actor* actor, Map* map){
	if (map && actor){
		
		//normally have reset visiblity here

		float actorSightRadius = actor->m_sightRadius;

		Tile* actorCurrentTile = actor->GetTileAtMyMapPosition();

		if (actorCurrentTile){
			TilePtrs tilesInActorSightRadius = map->GetAllTilesWithinRadiusOfTile(*actorCurrentTile, actorSightRadius);

			for (TilePtrsIterator it = tilesInActorSightRadius.begin(); it != tilesInActorSightRadius.end(); ++it){
				Tile* tile = (*it);

				//raytrace to tile here
				LineSegment2 rayToTraceToTile = LineSegment2(ToVector2(actor->m_position), ToVector2(tile->m_mapPosition));
				const float tStep = 0.1f;
				for (float t = 0.0f; t < 1.0f; t += tStep){

					Vector2 rayToTracePointAtT = rayToTraceToTile.GetPointAtParametricValue(t);
					
					Tile* tileAtRayTrace = map->GetTileAtMapPosition(ToIntVec2(rayToTracePointAtT));
					//does the visiblity flags stuff
					if (tileAtRayTrace){

						if (tileAtRayTrace->m_tileVisibility.m_isVisible)
							tileAtRayTrace->m_tileVisibility.m_hasBeenSeen = true;
						
						tileAtRayTrace->m_tileVisibility.m_isVisible = true;

						if (tileAtRayTrace->m_type == TILE_TYPE_SOLID ){
							tileAtRayTrace->m_tileVisibility.m_isVisible = true;
							break;
						}//end of innermost if

					}//end of is tile at ray trace valid

				}//end of raytrace for

			}

		}

	}
	
}

//-----------------------------------------------------------------------------------------------------------

void CalcBruteForceFieldOfViewForActor(Actor* actor, Map* map, const float& overrideRadius){
	if (map && actor){

		IntVec2s mapPositionsChecked;
		
		float actorSightRadius = actor->m_sightRadius;
		if (overrideRadius > 0.0f)
			actorSightRadius = overrideRadius;

		Tile* actorCurrentTile = actor->GetTileAtMyMapPosition();

		if (actorCurrentTile){
			TilePtrs tilesInActorSightRadius = map->GetAllTilesWithinRadiusOfTile(*actorCurrentTile, actorSightRadius);

			for (TilePtrsIterator it = tilesInActorSightRadius.begin(); it != tilesInActorSightRadius.end(); ++it){
				Tile* tile = (*it);

				IntVec2s::iterator positionsCheckedIter = std::find(mapPositionsChecked.begin(), mapPositionsChecked.end(), tile->m_mapPosition);
				//if found, continue
				if (positionsCheckedIter != mapPositionsChecked.end()){
					continue;
				}

				//raytrace to tile here
				LineSegment2 rayToTraceToTile = LineSegment2(ToVector2(actor->m_position), ToVector2(tile->m_mapPosition));
				const float tStep = 0.1f;
				for (float t = 0.0f; t < 1.0f; t += tStep){

					Vector2 rayToTracePointAtT = rayToTraceToTile.GetPointAtParametricValue(t);

					IntVec2 mapPositionAtT = ToIntVec2(rayToTracePointAtT);
					IntVec2s::iterator positionsCheckedIter = std::find(mapPositionsChecked.begin(), mapPositionsChecked.end(), mapPositionAtT);
					//if not found, add it and check it
					if (positionsCheckedIter == mapPositionsChecked.end()){
						mapPositionsChecked.push_back(mapPositionAtT);
						
						Tile* tileAtRayTrace = map->GetTileAtMapPosition(mapPositionAtT);
						if (tileAtRayTrace){

							//get the visible actors at this tile somehow
							
							//this is possibly super slow
							for (EntityIterator it = theEntities.begin(); it != theEntities.end(); ++it){
								 Entity* entity = (*it);

								if (!entity->m_isStructure && entity->m_id != actor->m_id && DoAABBAndPointOverlap(entity->m_renderBounds, tileAtRayTrace->m_worldCoordinates ) ){
									actor->AddToVisibleActors((Actor*)entity);
								}
							}

// 							if (tileAtRayTrace->m_tileContent.m_entity){
// 								actor->AddToVisibleActors((Actor*)tileAtRayTrace->m_tileContent.m_entity);
// 
// 								continue;
// 							}

							if (tileAtRayTrace->m_type == TILE_TYPE_SOLID){
								break;
							}//end of innermost if


						}//end of is tile at ray trace valid
					}//end of is found
					

				}//end of raytrace for

			}

		}

	}
}

//-----------------------------------------------------------------------------------------------------------

void CalcBruteForceVisibilityViewForActor(Actor* actor, Map* map, const float& overrideRadius, Controller* myController ){
	if (map && actor){
		
		IntVec2s mapPositionsChecked;

		float actorSightRadius = actor->m_sightRadius;
		if (overrideRadius > 0.0f)
			actorSightRadius = overrideRadius;

		Tile* actorCurrentTile = actor->GetTileAtMyMapPosition();

		if (actorCurrentTile){
			TilePtrs tilesInActorSightRadius = map->GetAllTilesWithinRadiusOfTile(*actorCurrentTile, actorSightRadius);
			
			for (TilePtrsIterator it = tilesInActorSightRadius.begin(); it != tilesInActorSightRadius.end(); ++it){
				Tile* tile = (*it);

				IntVec2s::iterator positionsCheckedIter = std::find(mapPositionsChecked.begin(), mapPositionsChecked.end(), tile->m_mapPosition);
				//if found, continue
				if (positionsCheckedIter != mapPositionsChecked.end()){
					continue;
				}

				//raytrace to tile here //commenting this out means it runs at 30 fps
				LineSegment2 rayToTraceToTile = LineSegment2(ToVector2(actor->m_position), ToVector2(tile->m_mapPosition));
				const float tStep = 0.1f;
				for (float t = 0.0f; t < 1.0f; t += tStep){

					Vector2 rayToTracePointAtT = rayToTraceToTile.GetPointAtParametricValue(t);

					Tile* tileAtRayTrace = map->GetTileAtMapPosition(ToIntVec2(rayToTracePointAtT));

					IntVec2 mapPositionAtT = ToIntVec2(rayToTracePointAtT);
					IntVec2s::iterator positionsCheckedIter = std::find(mapPositionsChecked.begin(), mapPositionsChecked.end(), mapPositionAtT);
					//if not found, add it and check it
					if (positionsCheckedIter == mapPositionsChecked.end()){
						mapPositionsChecked.push_back(mapPositionAtT);

						Tile* tileAtRayTrace = map->GetTileAtMapPosition(mapPositionAtT);
						if (tileAtRayTrace){
							if ((myController && myController->IsActive())){
								if (tileAtRayTrace->m_tileVisibility.m_isVisible)
									tileAtRayTrace->m_tileVisibility.m_hasBeenSeen = true;

								tileAtRayTrace->m_tileVisibility.m_isVisible = true;
							}
							else{ //what to do for not player
								actor->m_visibleTiles.insert(tileAtRayTrace);
							}
							//get the visible actors at this tile somehow

							//this is possibly super slow
							for (EntityIterator it = theEntities.begin(); it != theEntities.end(); ++it){
								 Entity* entity = (*it);

								if (entity->m_id != actor->m_id && DoAABBAndPointOverlap(entity->m_renderBounds, ToVector2(tileAtRayTrace->m_mapPosition))){
									actor->AddToVisibleActors((Actor*)entity);
								}
							}

							//do stuff
							if (tileAtRayTrace->m_type == TILE_TYPE_SOLID){
								if ((myController && myController->IsActive())){// /*|| (actor->m_isPlayerOwned)*/){
									tileAtRayTrace->m_tileVisibility.m_isVisible = true;
								}
								else{
									actor->m_visibleTiles.insert(tileAtRayTrace);
								}
								break;
							}//end of innermost if

						}//end of is tile at ray trace valid
					}//end of is found


				}//end of raytrace for


			}

		}//end of actorCurrentTile


	}//end of has map and actor

}

//===========================================================================================================