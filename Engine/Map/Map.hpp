//==============================================================================================================
//Map.hpp
//by Albert Chen Aug-18-2015.
//==============================================================================================================

#pragma once

#ifndef _included_Map__
#define _included_Map__

#include "Tile.hpp"
#include "Engine/Core/XML/XMLUtils.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Multithreading/CriticalSection.hpp"

//debug vars

static bool DebugDrawVisiblilityMask = true;

extern CriticalSection tilesPathIDClosedCritSec;
extern CriticalSection tilesPathIDOpenCritSec;

enum MapDirection{
	MAP_DIRECTION_NORTH,
	MAP_DIRECTION_SOUTH,
	MAP_DIRECTION_EAST,
	MAP_DIRECTION_WEST,
	MAP_DIRECTION_NORTHEAST,
	MAP_DIRECTION_NORTHWEST,
	MAP_DIRECTION_SOUTHEAST,
	MAP_DIRECTION_SOUTHWEST,
	TOTAL_MAP_DIRECTIONS,
	INVALID_MAP_DIRECTION

};

typedef IntVec2 MapPosition;
typedef std::vector<MapPosition> MapPositions;
typedef std::vector<MapPosition> MapPositionsIterator;

class View;

//2D map made up of square tiles
class Map{
public:
	//vars
	Tiles m_tiles;

	IntVec2 m_size;
	
	MeshRenderer m_visibilityMaskRenderer;

	int m_randomSeed;

	Tile* m_playerStartTile;

	std::string m_cellDataString;

	std::vector<std::string> m_CellDataTokens;

	Camera3D* m_mapViewCamera;
	ModelViewMatrix m_mapToWorldTransformMatrix;

	//methods
	 Map();
	
	 ~Map();
	
	Map(MapPosition mapSize, bool is3DMode = false);

	Map(const std::string& filePath){
		GenerateMapFromData(filePath);

		//CreateTileMapMesh();
	}

	Map(const XMLNode& mapNode){
		GenerateMapFromData(mapNode);
	}

	//init helpers
	void SetMapToWorldTransformMatrixFor2D(const Vector2& scale2D = TILE_SCALE_2D_VEC2, const Vector2& translation2D = TILE_START_OFFSET);

	void GenerateMap(const int& width, const int& height, bool is3DMode = false );
	
	void GenerateMapWithCellTokens(const int& width, const int& height);
	void GenerateMapFromData(const std::string& filePath);
	void GenerateMapFromData(const XMLNode& mapNode);

	void InitializeVisibilityMaskRenderer();

	void CreateTileMapMesh();

	void GenerateDefaultMap();

	//xml queries
	void SaveMapTileDataToXML(XMLNode& mapDataNode);
	
	void SaveMapKnownDataToXML(XMLNode& mapDataNode);
	
	//queries
	TilePtrs GetAllTilesWithinBoxOfTile(const Tile& tileToCheck, const int& width, const int& height);

	TilePtrs GetAllTilesWithinRadiusOfTile(const Tile& tileToCheck, const float& radius);
	
	const unsigned int GetIndexFromMapPosition(const MapPosition& mapLocation);
	
	bool IsTileIndexValid(const unsigned int& index);

	Tile* GetTileAtMapPosition(const MapPosition& mapLocation);
	
	Tile* GetTileNearMapPositionInMapDirection(const MapPosition& position, const MapDirection& mapDirection, const int& tileStep = 1);
	
	TilePtrs GetValidAdjacentTiles(Tile* tileToCheck, bool ignoreEntities = true);

	TilePtrs GetAdjacentTiles(Tile* tileToCheck);

	Tile* GetRandomTile();

	IntVec2 GetRandomMapPosition();

	Tile* GetRandomTileOfType(const TileType& type);

	Tile* GetRandomOpenTile(){ return GetRandomTileOfType(TILE_TYPE_AIR); }

	//update
	void Update(double deltaSeconds);

	//render
	void Render(OpenGLRenderer* renderer, bool is3DMode = false);

	void RenderTileVisibility(OpenGLRenderer* renderer, bool is3DMode = false);
	//render helpers
	void RenderTiles(OpenGLRenderer* renderer, bool is3DMode = false, bool renderInWorldSpace = false);

	//friend methods
	friend float GetManhattanDistanceBetweenMapPositions(const MapPosition& positionA, const MapPosition& positionB);
	friend float GetAbsoluteDistanceBetweenMapPositions(const MapPosition& positionA, const MapPosition& positionB);
	friend MapPosition GetMapPositionAtScreenCoordinates(const Vector2& screenCoords);	

};

///----------------------------------------------------------------------------------------------------------
///inline methods

inline Map::Map(){

	
}

inline Map::~Map(){
	//do nothing
}

inline Tile* Map::GetRandomTile(){
	int randomTileIndex = GetRandomIntInRange(1, m_tiles.size()-1);

	return &(m_tiles[randomTileIndex]);

}

inline IntVec2 Map::GetRandomMapPosition(){
	return IntVec2(GetRandomIntInRange(0, m_size.x-1), GetRandomIntInRange(0, m_size.y-1));
}

inline MapPosition GetMapPositionAtScreenCoordinates(const Vector2& screenCoords){
	MapPosition mapPosFromScreenPos = ToIntVec2( (screenCoords - TILE_START_OFFSET) * ONE_OVER_TILE_NODE_SCALE);

	return mapPosFromScreenPos;
}

inline MapDirection GetRandomDirection(){
	return (MapDirection)GetRandomIntInRange(0, TOTAL_MAP_DIRECTIONS - 1);
}

//-----------------------------------------------------------------------------------------------------------

//===========================================================================================================


#endif