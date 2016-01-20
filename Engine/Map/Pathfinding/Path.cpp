//==============================================================================================================
//Path.cpp
//by Albert Chen Sep-3-2015.
//==============================================================================================================

#include "Path.hpp"


//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///constructors

Path::Path(Map* map, const IntVec2& startPosition, const IntVec2& goalPosition, const float& bestAvoidCost ) :
m_startPosition(startPosition),
m_goalPosition(goalPosition),
m_map(map),
m_isImpossible(false),
m_isComplete(false),
m_bestAvoidCost(bestAvoidCost)
{
	m_id = currentPathID;
	currentPathID++;

	m_openList.clear();
	m_closedList.clear();

	// 	Tile* goalTile = NULL;
	// 	goalTile = m_map->GetTileAtMapPosition(m_goalPosition);
	// 	if (goalTile == NULL)
	// 		m_isImpossible = true;
	
	Tile* startTile = m_map->GetTileAtMapPosition((MapPosition)startPosition);

	float distance = 0.0f;//GetAbsoluteDistanceSquaredBetweenMapPositions(startPosition, goalPosition);

	float avoid = startTile->m_tileAvoidanceCost;//1.0f;

	float parent = 0.0f;

	float heuteristic = (float)GetManhattanDistanceBetweenMapPositions(startPosition, goalPosition);
	heuteristic += heuteristic * m_bestAvoidCost;

	PathNodeCost startNodeCost = PathNodeCost(parent, distance, avoid, heuteristic);

	m_startNode = new PathNode(startPosition, startNodeCost, NULL);

	//AddToOpenList(new PathNode(*m_startNode));
	AddToOpenList(m_startNode);

	m_initialized = true;
	
}

///----------------------------------------------------------------------------------------------------------
///interaction

PathNode* Path::AddToOpenList(PathNode* nodeToAdd){
	//I think I have to lock the open list for multithreading
	//CRITICAL_SECTION_ENTER();
	float nodeCost = nodeToAdd->GetFinalCost();

	m_openList.insert(OpenListEntry(nodeCost, nodeToAdd));
	//CRITICAL_SECTION_EXIT();
	return nodeToAdd;

}

//-----------------------------------------------------------------------------------------------------------
///----------------------------------------------------------------------------------------------------------
///queries

bool Path::IsPathNodeInOpenList(PathNode* nodeToAdd){
	for (OpenListPathMapIterator it = m_openList.begin(); it != m_openList.end(); ++it){
		PathNode& pathNode = (*it->second);

		if (pathNode.m_position == nodeToAdd->m_position){
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------------------------------------

bool Path::IsMapPositionInOpenList(const IntVec2& posToFind){
	PROFILE_SECTION();
	int i = 0;
	for (OpenListPathMapIterator it = m_openList.begin(); it != m_openList.end(); ++it){
		PathNode& pathNode = (*it->second);
		i++;
		if (pathNode.m_position == posToFind){
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------------------------------------

PathNode* Path::GetPathNodeAtMapPositionFromOpenList(const IntVec2& posToAdd){
	PROFILE_SECTION();
	for (OpenListPathMapIterator it = m_openList.begin(); it != m_openList.end(); ++it){
		PathNode* pathNode = (it->second);

		if (pathNode->m_position == posToAdd){
			return  (it->second);
		}
	}
	return NULL;
}

//-----------------------------------------------------------------------------------------------------------

bool Path::IsMapPositionInClosedList(const IntVec2& posToFind){
	PROFILE_SECTION(); //takes a bit longer than the others

	//size_t size = m_closedList.size();

	//int i = 0;
 	for (ClosedListIterator it = m_closedList.begin(); it != m_closedList.end(); ++it){
		 PathNode& closedListNode = (*it);
		 //i++;
		 if (closedListNode.m_position == posToFind){
			 return true;
		 }
// 		if (closedListNode.m_position != posToFind){
// 			//return true;
// 			continue;
// 		}
// 		else{
// 			return true;
// 		}

	}
	return false;
}

//-----------------------------------------------------------------------------------------------------------

void Path::SetPathFound(){

	if (m_closedList.empty()){
		return;
	}
	
	//m_pathFound.clear();

	ClosedListIterator mypathIterator = m_closedList.end() - 1;
	PathNode& myPathGoalNode = (*mypathIterator);
	//start traversing from goal back to start
	PathNode* traversalPathNode = &myPathGoalNode;
	//traversalPathNode = traversalPathNode->m_parent;



	if (traversalPathNode->m_parent != NULL){
		m_pathFound.push_back(traversalPathNode->m_position);

		traversalPathNode = traversalPathNode->m_parent;
	}
		

	//create path
	while (traversalPathNode->m_parent != NULL){

		m_pathFound.push_back(traversalPathNode->m_position); //crashes here when the program goes pretty dang fast! std::bad_alloc at memory location 0x06DCF89C.
		//something somewhere is null

		traversalPathNode = traversalPathNode->m_parent;
	}//end of while

}

//-----------------------------------------------------------------------------------------------------------

std::string Path::GetDebugString(){
	std::string pathOverviewStr = "Calc Path from (" + m_startPosition.ToString() + ") to (";
	pathOverviewStr += m_goalPosition.ToString() + ")\n";

	int distA = (int)GetAbsoluteDistanceBetweenMapPositions(m_startPosition, m_goalPosition);
	int distM = (int)GetManhattanDistanceBetweenMapPositions(m_startPosition, m_goalPosition);

	std::string pathBasicDatStr = "Abs Dist to goal: " + IntToString(distA) + "| Manhat Dist to goal: ";
	pathBasicDatStr += IntToString(distM) + "| \n";

	std::string pathNodesDatStr = "open nodes: " + IntToString(m_openList.size());
	pathNodesDatStr += "| closed nodes: " + IntToString(m_closedList.size());
	if (m_isComplete)
		pathNodesDatStr += "| nodes in found path: " + IntToString(m_pathFound.size()) + " |";

	pathNodesDatStr += "\n";
	pathNodesDatStr += "iterations: " + IntToString(m_iterations);

	std::string fullPathDatStr = pathOverviewStr + pathBasicDatStr + pathNodesDatStr;

	return fullPathDatStr;
}

//-----------------------------------------------------------------------------------------------------------
///----------------------------------------------------------------------------------------------------------
///interaction

PathNode* Path::RemoveFromOpenList(PathNode* nodeToRemove){
	//CRITICAL_SECTION_ENTER();
	OpenListPathMapIterator openListIt;// = m_openList.begin();

	for (openListIt = m_openList.begin(); openListIt != m_openList.end(); ++openListIt){
		PathNode& pathNode = (*openListIt->second);

		if (pathNode.m_position == nodeToRemove->m_position){
			m_openList.erase(openListIt);

			return nodeToRemove;
		}

	}//end of for
	//CRITICAL_SECTION_EXIT();

	return NULL;

}

//-----------------------------------------------------------------------------------------------------------

PathNode* Path::PopLowestFCostInOpenList(){
	OpenListPathMapIterator lowestFCostPathNode = m_openList.begin();
	float lowestKeyVal = lowestFCostPathNode->first;
	UNUSED(lowestKeyVal);
	//std::pair<OpenListPathMapIterator, OpenListPathMapIterator> lowestRangeOfNodes;
	//lowestRangeOfNodes = m_openList.equal_range(lowestKeyVal);

	//PathNode* bestPathNode = NULL;

// 	for (OpenListPathMapIterator it = lowestRangeOfNodes.first; it != lowestRangeOfNodes.second; ++it){
// 
// 	}

	return RemoveFromOpenList(lowestFCostPathNode->second);
}

PathNode* Path::GetLowestFCostInOpenList(){
	OpenListPathMapIterator lowestFCostPathNode = m_openList.begin();

	return (lowestFCostPathNode->second);
}

//===========================================================================================================