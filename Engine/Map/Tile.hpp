//==============================================================================================================
//Tile.hpp
//by Albert Chen Aug-18-2015.
//==============================================================================================================

#pragma once

#ifndef _included_Tile__
#define _included_Tile__

#include <vector>
#include <set>
#include "Engine/Math/IntVec2.hpp"
#include "Engine/Math/AABB2.hpp"

#include "Engine/Renderer/Vertex3D.hpp"
#include "Engine/Core/XML/XMLUtils.hpp"
#include "Engine/Input/GUI/UIButton.hpp"

#include "Engine/Map/TileState.hpp"
#include "Engine/Components/RenderComponent.hpp"

const int   TILE_X_WIDTH = 64;
const int   TILE_Y_HEIGHT = 32;
const int   NUMBER_OF_TILE_NODES = TILE_X_WIDTH * TILE_Y_HEIGHT;// +TILE_X_WIDTH;

const float TILE_SCALE_2D = 32.0f;
const float ONE_OVER_TILE_NODE_SCALE = 1.0f / TILE_SCALE_2D;
const float TILE_RADIUS_2D = TILE_SCALE_2D * 0.5f;
const Vector2 TILE_SCALE_2D_RADIUS_VEC2 = Vector2(TILE_RADIUS_2D, TILE_RADIUS_2D);
const Vector2 TILE_START_OFFSET = Vector2(TILE_SCALE_2D, TILE_SCALE_2D * 6.0f);

const Vector2 TILE_SCALE_2D_VEC2 = Vector2(TILE_SCALE_2D, TILE_SCALE_2D);
const IntVec2 TILE_SCALE_2D_INTVEC2 = IntVec2((int)TILE_SCALE_2D, (int)TILE_SCALE_2D);

const int   TILE_SCALE_2D_OFFSET = (int)(TILE_SCALE_2D * 1.0f);

//texture consts
const float TILE_ATLAS_WIDTH = 192.0f;
const float TILE_ATLAS_HEIGHT = 48.0f;

//specific to that netmage dfs
const AABB2 TILE_AIR_TEX_COORDS = AABB2(Vector2(48.0f / TILE_ATLAS_WIDTH, 0.0f), Vector2(96.0f / TILE_ATLAS_WIDTH, 144.0f / TILE_ATLAS_WIDTH));
const AABB2 TILE_SOLID_TEX_COORDS = AABB2(Vector2(0.0f, 0.0f), Vector2(48.0f / TILE_ATLAS_WIDTH, 144.0f / TILE_ATLAS_WIDTH));
const AABB2 TILE_HIGHLIGHT_TEX_COORDS = AABB2(Vector2(96.0f / TILE_ATLAS_WIDTH, 0.0f), Vector2(144.0f / TILE_ATLAS_WIDTH, 192.0f / TILE_ATLAS_WIDTH));

struct MeshRenderer;
class OpenGLRenderer;

enum TileType{
	TILE_TYPE_AIR = 0,
	TILE_TYPE_SOLID = 1, 
	NUMBER_OF_TILE_TYPES,
	TILE_TYPE_INVALID
};

//===========================================================================================================

class Tile{
public:

	Tile(){
		//do nothing
	}
	~Tile(){
		//do nothing

	}

	Tile(const int& mapIndex, const IntVec2& loc, const TileType& tileType = TILE_TYPE_SOLID);

	//construction helpers
	const Vertex3Ds GetTileVertex3Ds(bool clearVerts = true);

	void SetRenderBounds(){
		m_renderBounds = GetRenderBounds();
	}
	AABB2 GetRenderBounds();

	void SetRenderComponent2D();
	void SetRenderComponent3D();

	const char GetTileDetailsForTileType();

	void ChangeTileType(const TileType& newType){ m_type = newType; }

	bool IsEmpty(){ return m_tileState.contents.m_isEmpty; }
	void SetIsEmpty(bool newIsEmpty){ m_tileState.contents.m_isEmpty = newIsEmpty; }
	
	//update
	void Update(double deltaSeconds);
	
	//render
	void Render2D(OpenGLRenderer* renderer, ModelViewMatrix* modelView = NULL, Light* light = NULL);
	void Render3D(OpenGLRenderer* renderer, Camera3D& camera, bool isPerspective = true, Lights* lights = NULL, ModelViewMatrix* modelView = NULL);

	//xml helpers
	std::string WriteTileDataToXML(XMLNode& xmlNode);
	std::string WriteTileStateDataToXML(XMLNode& xmlNode);
	
	//pathing helpers
	friend bool VerifyTileIsValidForMovement(Tile* tile, bool ignoreEntities = true);

	//vars

	TileType m_type;
	Rgba m_color;

	TileState m_tileState;

	IntVec2 m_mapPosition; //x,y pos in map from 0,0 to mapH,mapW
	float m_mapHeight = 0.0f; //height of tile in map.

	AABB2 m_renderBounds;

	int m_mapIndex;

	RenderComponent m_renderComponent;

	float m_tileAvoidanceCost = 1.0f;
	
	int m_inOpenListOfPathID = -1;
	int m_inClosedListOfPathID = -1;

};

typedef std::vector<Tile> Tiles;
typedef std::vector<Tile*> TilePtrs;
typedef std::vector<Tile>::iterator TilesIterator;
typedef std::vector<Tile*>::iterator TilePtrsIterator;

typedef std::set<Tile*> TileSet;
typedef std::set<Tile*>::iterator TileSetIterator;

///----------------------------------------------------------------------------------------------------------
///inline methods

inline std::string Tile::WriteTileDataToXML(XMLNode& xmlNode){
	UNUSED(xmlNode);
	std::string tileDetailString;
	tileDetailString += CharToString(GetTileDetailsForTileType());

	return tileDetailString;
}

//-----------------------------------------------------------------------------------------------------------

inline std::string Tile::WriteTileStateDataToXML(XMLNode& xmlNode){
	UNUSED(xmlNode);
	std::string tileDetailString;
	if (m_tileState.visibility.m_hasBeenSeen)
		tileDetailString = CharToString(GetTileDetailsForTileType());
	else
		tileDetailString = "?";

	return tileDetailString;
}

//===========================================================================================================

#endif