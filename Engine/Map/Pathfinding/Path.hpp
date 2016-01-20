//==============================================================================================================
//Path.hpp
//by Albert Chen Sep-3-2015.
//==============================================================================================================

#pragma once

#ifndef _included_Path__
#define _included_Path__

#include <map>
#include <unordered_map>
#include <list>
#include "PathNode.hpp"
#include "Engine/Multithreading/CriticalSection.hpp"

//===========================================================================================================

//multi map
typedef std::multimap<float, PathNode*> OpenListPathMap;
//typedef std::unordered_multimap<float, PathNode*> OpenListPathMap;

typedef std::pair<float, PathNode*> OpenListEntry;

typedef std::multimap<float, PathNode*>::iterator OpenListPathMapIterator;
//typedef std::unordered_multimap<float, PathNode*>::iterator OpenListPathMapIterator;

typedef std::vector<PathNode> ClosedList;
typedef std::vector<PathNode>::iterator ClosedListIterator;

typedef std::vector<PathNode*> PathNodePtrList;
typedef std::vector<PathNode*>::iterator PathNodePtrIterator;

//===========================================================================================================

static int currentPathID = 0;

class Path{
public:
	//vars
	OpenListPathMap m_openList;
	ClosedList m_closedList;

	MapPositions m_pathFound;

	IntVec2 m_startPosition;
	IntVec2 m_goalPosition;

	Map* m_map;
	PathNode* m_activeNode;
	PathNode* m_startNode;

	int m_id;

	float m_bestAvoidCost;

	bool m_isComplete;
	bool m_initialized;
	bool m_isImpossible;

	unsigned int m_iterations = 0;//num times process one step was called
	//methods
	//constructors
	Path();
	~Path();
	Path(Map* map, const IntVec2& startPosition, const IntVec2& goalPosition, const float& bestAvoidCost = 1.0f );
	
	//queries
	bool IsOpenListEmpty();
	
	bool IsPathNodeInOpenList(PathNode* nodeToAdd);
	
	bool IsMapPositionInOpenList(const IntVec2& posToAdd);
	PathNode* GetPathNodeAtMapPositionFromOpenList(const IntVec2& posToAdd);
	bool IsMapPositionInClosedList(const IntVec2& nodeToFind);
	
	void SetPathFound();

	std::string GetDebugString();

	//interaction
	PathNode* AddToOpenList(PathNode* nodeToAdd);

	PathNode* AddToClosedList(PathNode* nodeToAdd);

	PathNode* RemoveFromOpenList(PathNode* nodeToRemove);

	PathNode* PopLowestFCostInOpenList();

	PathNode* GetLowestFCostInOpenList();
};

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods

///----------------------------------------------------------------------------------------------------------
///constructors

inline Path::Path(){
	//do nothing
	m_initialized = false;
	m_isComplete = false;
	m_isImpossible = false;
	m_activeNode = NULL;
	m_map = NULL;
}

//-----------------------------------------------------------------------------------------------------------

inline Path::~Path(){
	//do nothing;
	m_openList.clear();
	m_closedList.clear();
	m_pathFound.clear();

// 	if (m_startNode){
// 		delete m_startNode;
// 		m_startNode = NULL;
// 	}
// 
// 	if (m_activeNode){
// 		delete m_activeNode;
// 		m_activeNode = NULL;
// 	}
}

//-----------------------------------------------------------------------------------------------------------

inline bool Path::IsOpenListEmpty(){ 
	return m_openList.empty();
}



///----------------------------------------------------------------------------------------------------------
///interaction

//-----------------------------------------------------------------------------------------------------------

inline PathNode* Path::AddToClosedList(PathNode* nodeToAdd){
	m_closedList.push_back(*nodeToAdd);

	return nodeToAdd;
}

//-----------------------------------------------------------------------------------------------------------

//===========================================================================================================

#endif
