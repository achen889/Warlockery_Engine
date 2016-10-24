
//==============================================================================================================
//Pathfinder.cpp
//by Albert Chen Sep-3-2015.
//==============================================================================================================

#include "Pathfinder.hpp"
#include "Engine/Core/Performance.hpp"
#include "Engine/Map/Map.hpp"

//===========================================================================================================

//A* Pathfinding Algorithm
//pass in a blank myPath to be filled
Path& CalcPath(Path& thePath, Map* map, const MapPosition& start, const MapPosition& goal, bool ignoreEntities){
	PROFILE_SECTION();

	if (!thePath.m_initialized && map){
		thePath = Path(map, start, goal); //this adds a start node to open list
	}
	
	//find the path and stuff
	while (!thePath.IsOpenListEmpty() && !thePath.m_isComplete){

		ProcessOneStepOfPath(thePath, map, start, goal, ignoreEntities);

	}//end of while
	
	if (thePath.IsOpenListEmpty() && !thePath.m_isComplete){
		thePath.m_isImpossible = true;
	}
	
	//17 return the path
	return thePath;

}//end of calc path

//===========================================================================================================

//much slower than the regular one
void RecursiveCalcPath(Path& thePath, Map* map, const IntVec2& start, const IntVec2& goal, bool ignoreEntities){
	PROFILE_SECTION();
	if (!thePath.m_initialized && map){
		thePath = Path(map, start, goal); //this adds a start node to open list
	}
	
	if (!thePath.IsOpenListEmpty() && !thePath.m_isComplete){
		thePath = ProcessOneStepOfPath(thePath, map, start, goal, ignoreEntities);

		RecursiveCalcPath(thePath, map, start, goal, ignoreEntities);

	}
	else{

		return;

	}


}

//===========================================================================================================

Path& ProcessOneStepOfPath(Path& thePath, Map* map, const IntVec2& start, const IntVec2& goal, bool ignoreEntities ){
	PROFILE_SECTION();
	if (!thePath.m_initialized && map){
		thePath = Path(map, start, goal);
	}
	
	if (!thePath.IsOpenListEmpty() && !thePath.m_isComplete){

		thePath.m_iterations++;

		PROFILE_START(steps 1 to 4);
		//1 pick lowest f on openList check
		//2 remove from open list check
		PathNode* lowestFCostInThePath = thePath.PopLowestFCostInOpenList();

		//3 add lowest cost to closed list check
		Tile* tileToCheckID = map->GetTileAtMapPosition(lowestFCostInThePath->m_position);
		tilesPathIDOpenCritSec.Enter();
		tileToCheckID->m_inOpenListOfPathID = -1;
		tilesPathIDOpenCritSec.Exit();

		//tilesPathIDCritSec.Enter();
		if (tileToCheckID->m_inClosedListOfPathID != thePath.m_id){
			
			thePath.AddToClosedList(lowestFCostInThePath);
			CRITICAL_SECTION_SCOPE(tilesPathIDClosedCritSec);
			tileToCheckID->m_inClosedListOfPathID = thePath.m_id;
		}
		//tilesPathIDCritSec.Exit();

// 		if (!thePath.IsMapPositionInClosedList(lowestFCostInThePath->m_position)){
// 
// 			thePath.AddToClosedList(lowestFCostInThePath);
// 
// 		}

		//4 set lowest f node to active
		thePath.m_activeNode = lowestFCostInThePath;
		PROFILE_STOP();

		//5 check if node is goal, break;
		if (thePath.m_activeNode->m_position == goal){
			thePath.m_isComplete = true;
			//ConsoleGenericMessageBox("YAY Path is found.");

			thePath.m_openList.clear();

			return thePath;
		}
		else{
			TilePtrs adjacentTilePtrs;
			PROFILE_START(step 6);
			//6 ask "map" for adjacent positions of active node
			Tile* activeTile = map->GetTileAtMapPosition(thePath.m_activeNode->m_position);
			if (activeTile){
				adjacentTilePtrs = map->GetValidAdjacentTiles(activeTile, ignoreEntities);
			}
			PROFILE_STOP();

			//this is the longest part of pathing
			ProcessAdjacentMapTiles(thePath, map, start, goal, adjacentTilePtrs);

		}

	}//end of final outer if
	
// 	else if (thePath.IsOpenListEmpty()){
// 		//ConsoleGenericMessageBox("No Path =(");
// 
// 		//the path is impossible
// 		thePath.m_isImpossible = true;
// 
// 		return thePath;
// 	}
	
	return thePath;
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///pathfinding wrappers

void ProcessAdjacentMapTiles(Path& thePath, Map* map, const IntVec2& start, const IntVec2& goal, TilePtrs& adjacentTilePtrs){
	PROFILE_SECTION();
	//7 for (adjacent List of PathNodes)
	for (TilePtrsIterator it = adjacentTilePtrs.begin(); it != adjacentTilePtrs.end(); ++it){
		Tile* tile = (*it);

		ProcessAdjacentMapTile(thePath, tile, map, start, goal);
		
	}//end of for
	
}

//===========================================================================================================

void ProcessAdjacentMapTile(Path& thePath, Tile* tile, Map* map, const IntVec2& start, const IntVec2& goal ){
	UNUSED(start);

	PROFILE_SECTION();
	if (tile == NULL)
		return;

	//8 Get adjacent pos
	MapPosition& adjacentNodePos = tile->m_mapPosition;

	//float newFinalCostCheck = newPathNode->m_nodeCost.h;
	//if (newFinalCostCheck < 2.0f * map->m_size.x * map->m_size.y){

	//9 if (adjacent pos on closed list) //continue;

	Tile* tileToCheckID = map->GetTileAtMapPosition(adjacentNodePos);
	bool tileCheckID;
	//tilesPathIDCritSec.Enter();
	tileCheckID = (tileToCheckID->m_inClosedListOfPathID == thePath.m_id);
	//tilesPathIDCritSec.Exit();
	if (tileCheckID){
	//if (thePath.IsMapPositionInClosedList(adjacentNodePos)){
		return;
	}
	else{
		//10 & 11 decently fast
		// 10 if (not on open list){ 
		if(tileToCheckID->m_inOpenListOfPathID != thePath.m_id){
		//if (!thePath.IsMapPositionInOpenList(adjacentNodePos)){
			PROFILE_START(step 10 and 11);
			//11 build new pathnode(local G, parentG = ActiveNode.G) //	Add to OpenList
			PathNode* newPathNode = new PathNode(tile, thePath.m_activeNode, goal, thePath.m_bestAvoidCost); //this bit of path node has a memory leak
	
			thePath.AddToOpenList(newPathNode);

			tilesPathIDOpenCritSec.Enter();
			tile->m_inOpenListOfPathID = thePath.m_id;
			tilesPathIDOpenCritSec.Exit();

			PROFILE_STOP();
			return;
		}
		else{
			//is on openList
			float altTotalG;
			float tileAvoidCost;
			float currentG;
			PathNode* nodeInOpenList;

			PROFILE_START(step 12 and 13); //doesn't seem to ever get called
			tileAvoidCost = tile->m_tileAvoidanceCost;
			nodeInOpenList = thePath.GetPathNodeAtMapPositionFromOpenList(adjacentNodePos);
			if (!nodeInOpenList){
				return;
			}
			//12 calc newG as calc hypothetical G to this node
			altTotalG = CalcHypotheticalGCostToAdjacentNodePosition(thePath.m_activeNode, adjacentNodePos, tileAvoidCost);
			//13 compare to G in openList for node holding this position	
			currentG = nodeInOpenList->m_nodeCost.g;
			PROFILE_STOP();
			
			//14 if better <
			if (altTotalG < currentG){
				//15 update openListNode, update localG, parent, 
				PROFILE_START(step 15);
				ReplaceOpenListPathNode(thePath, nodeInOpenList, tileAvoidCost);
				PROFILE_STOP();
				return;
			}
			else{
				//16 if worse or equal >= //throw out
				return;
			}//end of inside if/else

		}//end of if/else

	}//end of outer if/else

}

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///if the adjacent node is in the openList and the hypothetical cost is better replace the node
void ReplaceOpenListPathNode(Path& thePath, PathNode* nodeInOpenList, const float& avoidCost){
	ASSERT_RECOVERABLE(nodeInOpenList != NULL, "");
	//PROFILE_SECTION();
	nodeInOpenList->m_nodeCost.avoidanceG = avoidCost;

	//nodeInOpenList->m_nodeCost.distanceG = GetManhattanDistanceBetweenMapPositions(thePath.m_activeNode->m_position, nodeInOpenList->m_position);
	nodeInOpenList->m_nodeCost.distanceG = GetAbsoluteDistanceBetweenMapPositions(thePath.m_activeNode->m_position, nodeInOpenList->m_position);

	nodeInOpenList->m_nodeCost.SetLocalG();

	nodeInOpenList->m_parent = thePath.m_activeNode;

	nodeInOpenList->SetParentGFromParentNode(nodeInOpenList->m_parent);

	nodeInOpenList->m_nodeCost.SetTotalG();

	nodeInOpenList->m_nodeCost.SetFinalCost();
}

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///calculate hypothetical G cost from the current active node to the adjacent node
const float CalcHypotheticalGCostToAdjacentNodePosition(PathNode* currentActiveNode, const IntVec2& adjacentNodePos, const float& avoidCost){
	IntVec2 activeNodePosition = currentActiveNode->m_position;

	float altAvoidG = avoidCost;

	float altDistanceG = GetAbsoluteDistanceBetweenMapPositions(activeNodePosition, adjacentNodePos);
	//float altDistanceG = GetManhattanDistanceBetweenMapPositions(activeNodePosition, adjacentNodePos);

	//float altParentG = currentActiveNode->GetFinalCost();
	float altParentG = currentActiveNode->m_nodeCost.g;

	float altLocalG = altDistanceG + altAvoidG;

	float altTotalG = altParentG + altLocalG;

	return altTotalG;
}

//===========================================================================================================

//
void RenderDebugPathMeshOnMap2D(OpenGLRenderer* renderer, MeshRenderer& pathMeshRenderer, Path& pathToRender, Map* map){
	UNUSED(renderer);
	//renderer->SetTextureViewTransparent();

	ModelViewMatrix& mapToWorldTransform = map->m_mapToWorldTransformMatrix;

	//set mesh renderer
	Vertex3Ds inPathVerts;
	inPathVerts.clear();

	Tile* pathingTile = NULL;
	
	//render start/goal/open tiles
	if (map && !pathToRender.IsOpenListEmpty()){

		//render start tile
		pathingTile = map->GetTileAtMapPosition(pathToRender.m_startPosition);
		GenerateVertexArrayTextureQuad(inPathVerts, pathingTile->m_renderBounds, AABB2::ZERO_TO_ONE, Rgba::SILVER, false);

		
		//render goal tile
		pathingTile = map->GetTileAtMapPosition(pathToRender.m_goalPosition);
		GenerateVertexArrayTextureQuad(inPathVerts, pathingTile->m_renderBounds, AABB2::ZERO_TO_ONE, Rgba::GOLD, false);

		if (pathToRender.m_activeNode){
			pathingTile = map->GetTileAtMapPosition(pathToRender.m_activeNode->m_position);
			GenerateVertexArrayTextureQuad(inPathVerts, pathingTile->m_renderBounds, AABB2::ZERO_TO_ONE, Rgba::MAGENTA, false);

			//OUTPUT_COLOR_STRING_TO_SCREEN(IntToString(pathToRender.m_activeNode->m_nodeCost.f), pathingTile->m_renderBounds.mins.x, pathingTile->m_renderBounds.maxs.y, Rgba::GOLD);
		}
	
		static Rgba openListColor = Rgba::BLUE;
		openListColor.a = 127;

		for (OpenListPathMapIterator it = pathToRender.m_openList.begin(); it != pathToRender.m_openList.end(); ++it){
			PathNode& pathnode = *(it->second);

			pathingTile = map->GetTileAtMapPosition(pathnode.m_position);

			GenerateVertexArrayTextureQuad(inPathVerts, pathingTile->m_renderBounds, AABB2::ZERO_TO_ONE, openListColor , false);

			//OUTPUT_COLOR_STRING_TO_SCREEN(IntToString(pathnode.m_nodeCost.f), pathingTile->m_renderBounds.mins.x, pathingTile->m_renderBounds.maxs.y, Rgba::GOLD);
		}

	}


	//create path mesh for all of closed list
	if (map && pathToRender.m_closedList.size() > 1 ){

		static Rgba closedListColor = Rgba::RED;
		closedListColor.a = 127;

		for (ClosedListIterator it = pathToRender.m_closedList.begin() + 1; it != pathToRender.m_closedList.end() - 1; ++it){
			PathNode& pathnode = (*it);

			pathingTile = map->GetTileAtMapPosition(pathnode.m_position);

			GenerateVertexArrayTextureQuad(inPathVerts, pathingTile->m_renderBounds, AABB2::ZERO_TO_ONE, closedListColor, false);
			
			//OUTPUT_COLOR_STRING_TO_SCREEN(IntToString(pathnode.m_nodeCost.f), pathingTile->m_renderBounds.mins.x, pathingTile->m_renderBounds.maxs.y, Rgba::GOLD);
			
		}//end of for

		if (pathToRender.m_isImpossible){
			return;
		}
	
		static Rgba pathColor = Rgba::GREEN;
		pathColor.a = 210;

		//Create path mesh for only path
		ClosedListIterator mypathIterator = pathToRender.m_closedList.end() - 1;
		PathNode& myPathGoalNode = (*mypathIterator);
		//start traversing from goal back to start
		PathNode* traversalPathNode = &myPathGoalNode;
		//traversalPathNode = traversalPathNode->m_parent;

		//create last closed node added
		pathingTile = map->GetTileAtMapPosition(traversalPathNode->m_position);
		GenerateVertexArrayTextureQuad(inPathVerts, pathingTile->m_renderBounds, AABB2::ZERO_TO_ONE, pathColor, false);

		if (traversalPathNode->m_parent != NULL)
			traversalPathNode = traversalPathNode->m_parent;

		//create path
		while (traversalPathNode->m_parent != NULL){
			   
			   pathingTile = map->GetTileAtMapPosition(traversalPathNode->m_position); //has an infinite loop in certain cases
			   GenerateVertexArrayTextureQuad(inPathVerts, pathingTile->m_renderBounds, AABB2::ZERO_TO_ONE, pathColor, false);
		
				//OUTPUT_COLOR_STRING_TO_SCREEN(IntToString(traversalPathNode->m_nodeCost.f), pathingTile->m_renderBounds.mins.x, pathingTile->m_renderBounds.maxs.y, Rgba::WHITE);
			
				traversalPathNode = traversalPathNode->m_parent;
		}//end of while

	
	

	}//end of outer if

	//copy all verts to mesh
	pathMeshRenderer.m_mesh->CopyMeshVertexData(inPathVerts);
	pathMeshRenderer.RenderMesh2D(&mapToWorldTransform);
	inPathVerts.clear();
	
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///pathfinding job helpers

void CalcPathJob::Execute(){

	if (myPath){

		CalcPath(*myPath, myMap, start, goal, ignoreEntities);
	
		if (myPath->m_isComplete){
			myPath->SetPathFound();
			if (myPath->m_pathFound.empty()){
				myPath->m_isImpossible = true;
			}
		}
	}

}

void OnCalcPathJobComplete(void* data){
	UNUSED(data);
//	ASSERT_PTR_VALID(data);

// 	Path* myPath = (Path*)data; //fancy ptr cast
// 	if (myPath){
// 		if (myPath->m_isComplete){
// 			myPath->SetPathFound();
// 			if (myPath->m_pathFound.empty()){
// 				myPath->m_isImpossible = true;
// 			}
// 		}
// 	}
	


	ConsoleGenericMessageBox("Calc Path Job Completed!");
}

void AddCalcPathJob(Path& thePath, Map* map, const IntVec2& start, const IntVec2& goal, bool ignoreEntities){

	if (theJobManager){
		CalcPathJob* newJob = new CalcPathJob(&thePath, map, start, goal, ignoreEntities);
		newJob->SetJobCompleteCallBack(OnCalcPathJobComplete, &thePath);

		theJobManager->AddPendingJob(newJob, PRIORITY_HIGH);
	}

}


