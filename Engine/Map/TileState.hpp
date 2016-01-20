//==============================================================================================================
//TileState.hpp
//by Albert Chen Dec-2-2015.
//==============================================================================================================

#pragma once

#include "Engine/Core/Utilities.hpp"

#ifndef _included_TileState__
#define _included_TileState__

//===========================================================================================================

class Tile;
class Entity;

//===========================================================================================================

struct TileVisibility{

	bool m_isVisible;
	bool m_hasBeenSeen;

	float m_visibility;

	TileVisibility(bool visible = true, bool hasBeenSeen = true, float visibility = 1.0f){
		m_isVisible = visible;
		m_hasBeenSeen = hasBeenSeen;
		m_visibility = visibility;
	}

};

//===========================================================================================================

struct TileContents{

	bool m_isEmpty;

	Entity* m_entity;

	TileContents(){
		m_isEmpty = true;

		m_entity = NULL;
	}



};

//===========================================================================================================

struct TileState{
	//vars
	TileVisibility visibility;
	TileContents contents;
	
	//methods
	TileState():
		visibility(TileVisibility(false, false)),
		contents(TileContents())
	{

	}
	
	~TileState(){
		//do nothing
	}
	TileState(const Tile& tile);

	//accessors
	void SetTileState(const Tile& tile);

	//queries
	bool IsVisible();
	bool HasBeenSeen();
	bool IsEmpty();

};

//===========================================================================================================

typedef std::vector<TileState> TileStates;
typedef std::vector<TileState>::iterator TileStatesIterator;

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline

inline bool TileState::IsVisible(){
	return visibility.m_isVisible;
}

//-----------------------------------------------------------------------------------------------------------

inline bool TileState::HasBeenSeen(){
	return visibility.m_hasBeenSeen;
}

//-----------------------------------------------------------------------------------------------------------

inline bool TileState::IsEmpty(){
	return contents.m_isEmpty;
}

//-----------------------------------------------------------------------------------------------------------

//===========================================================================================================

#endif //__includedTileState__

