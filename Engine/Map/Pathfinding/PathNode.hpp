//==============================================================================================================
//PathNode.hpp
//by Albert Chen Sep-3-2015.
//==============================================================================================================

#pragma once

#ifndef _included_PathNode__
#define _included_PathNode__

#include "Engine/Map/Map.hpp"

struct PathNode;

//===========================================================================================================

struct PathNodeCost{
	float localG;
	float parentG;

	float distanceG;
	float avoidanceG;

	float g;
	
	float h;

	float f;

	PathNodeCost(){
		//do nothing
	}

	PathNodeCost(const float& parent, const float& distance, const float& avoidance, const float& heuteristic);
	
	void SetLocalG();

	void SetTotalG();
	
	void SetFinalCost();
};
///----------------------------------------------------------------------------------------------------------
///inline methods

inline void PathNodeCost::SetLocalG(){
	localG = avoidanceG + distanceG;
}

inline void PathNodeCost::SetTotalG(){
	g = parentG + localG;
}

inline void PathNodeCost::SetFinalCost(){
	f = g + h;
}

//===========================================================================================================

class Tile;

struct PathNode{

	IntVec2 m_position;

	PathNodeCost m_nodeCost;

	PathNode* m_parent = NULL;

	~PathNode(){
// 		if (m_parent){
// 			delete m_parent;
// 			m_parent = NULL;
// 		}
	}

	PathNode(const IntVec2& position, const PathNodeCost& pathNodeCost, PathNode* parent);

	PathNode(Tile* tile, PathNode* parent, const IntVec2& goalPosition, const float& bestAvoidCost = 1.0f );
	
	float GetFinalCost();

	void SetParentGFromParentNode(PathNode* parentNode);
};

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods

inline float PathNode::GetFinalCost(){
	return m_nodeCost.f;
}

inline void PathNode::SetParentGFromParentNode(PathNode* parentNode){
	m_nodeCost.parentG = parentNode->m_nodeCost.g;
}


//static PathNode* s_activeNode;

#endif

