//==============================================================================================================
//PathNode.cpp
//by Albert Chen Sep-3-2015.
//==============================================================================================================

#include "PathNode.hpp"
//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///path node cost


PathNodeCost::PathNodeCost(const float& parent, const float& distance, const float& avoidance, const float& heuteristic):
distanceG(distance),
avoidanceG(avoidance),
h(heuteristic),
parentG(parent)

{
	localG = distanceG + avoidanceG;
	g = localG + parentG,
	f = g + h;
}

//===========================================================================================================

//tile has already been verified here
PathNode::PathNode(Tile* myTile, PathNode* parent, const IntVec2& goalPosition, const float& bestAvoidCost ) :
m_parent(parent),
m_position(myTile->m_mapPosition)
{
	float parentGCost = parent->m_nodeCost.g;

	float distanceGCost = GetAbsoluteDistanceBetweenMapPositions(m_position, parent->m_position);
	//float distanceGCost = GetManhattanDistanceBetweenMapPositions(m_position, parent->m_position);

	float avoidanceGCost = myTile->m_tileAvoidanceCost;

	float heuteristicCost = (float)GetManhattanDistanceBetweenMapPositions(m_position, goalPosition);
	heuteristicCost += heuteristicCost * bestAvoidCost;

	m_nodeCost = PathNodeCost(parentGCost, distanceGCost, avoidanceGCost, heuteristicCost);

}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///constructors

PathNode::PathNode(const IntVec2& position, const PathNodeCost& pathNodeCost, PathNode* parent):
m_position(position),
m_nodeCost(pathNodeCost),
m_parent(parent)
{

}


