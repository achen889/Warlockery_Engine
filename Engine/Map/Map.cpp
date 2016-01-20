//==============================================================================================================
//Map.cpp
//by Albert Chen Aug-18-2015.
//==============================================================================================================

#include "Map.hpp"
#include "Engine/Renderer/OpenGLRenderer.hpp"
#include "Engine/Math/Math2D.hpp"
#include "Engine/Renderer/Text/TextSystem.hpp"

//===========================================================================================================

CriticalSection tilesPathIDClosedCritSec;
CriticalSection tilesPathIDOpenCritSec;

//===========================================================================================================

//-----------------------------------------------------------------------------------------------------------

Map::Map(IntVec2 mapSize, bool is3DMode){

	m_size = mapSize;
	
	GenerateMap(mapSize.x, mapSize.y, is3DMode);

	//m_mapViewCamera = new Camera3D();
	
}

//-----------------------------------------------------------------------------------------------------------

void Map::SetMapToWorldTransformMatrixFor2D(const Vector2& scale2D, const Vector2& translation2D){
	m_mapToWorldTransformMatrix.SetScale(scale2D);
	m_mapToWorldTransformMatrix.SetTranslation2D(translation2D);
}

//-----------------------------------------------------------------------------------------------------------

//generators
void Map::GenerateMap(const int& width, const int& height, bool is3DMode){
	
	int currentIndex = 0;
	m_tiles.reserve(width * height);

	for (int x = 0; x < width; x++){
		for (int y = 0; y < height; y++){

			Tile newTile = Tile(currentIndex, MapPosition(x, y), TILE_TYPE_AIR);
			
			//debug give tile random color
			newTile.m_color = RANDOM_COLOR;

			m_tiles.push_back(newTile);
			if (is3DMode){
				m_tiles.back().SetRenderComponent3D();
			}
			else{
				m_tiles.back().SetRenderComponent2D();
			}
			currentIndex++;
		}
	}

}

//-----------------------------------------------------------------------------------------------------------

void Map::GenerateMapWithCellTokens(const int& width, const int& height){

	Vertex3Ds mapTileVerts;
	mapTileVerts.reserve(10000);

	int currentIndex = 0;

	for (int x = 0; x < width; x++){
		for (int y = 0; y < height; y++){
			//if (x % 2 == 0 || y % 2 == 0)m_tiles.push_back(Tile(currentIndex, IntVec2(x, y),TILE_TYPE_GRASS));
			//else if( x / 2 == 0 || y / 2 == 0) m_tiles.push_back(Tile(currentIndex, IntVec2(x, y), TILE_TYPE_WATER));
			//else 
			std::string cellRow = m_CellDataTokens[height-y-1];
			if (cellRow[x] == '#'){
				m_tiles.push_back(Tile(currentIndex, IntVec2(x, y), TILE_TYPE_SOLID));
			}
// 			else if (cellRow[x] == '~'){
// 				m_tiles.push_back(Tile(currentIndex, IntVec2(x, y), TILE_TYPE_WATER));
// 			}
// 			else if (cellRow[x] == '"'){
// 				m_tiles.push_back(Tile(currentIndex, IntVec2(x, y), TILE_TYPE_GRASS));
// 			}
// 			else if (cellRow[x] == '='){
// 				m_tiles.push_back(Tile(currentIndex, IntVec2(x, y), TILE_TYPE_LAVA));
// 			}
			else {
				m_tiles.push_back(Tile(currentIndex, IntVec2(x, y), TILE_TYPE_AIR));
			}

			if (cellRow[x] == '@'){
				m_playerStartTile = new Tile(m_tiles.back());
			}

			currentIndex++;
		}
	}

}

//-----------------------------------------------------------------------------------------------------------

void Map::GenerateMapFromData(const std::string& filePath){

	XMLFileParser mapParser;
	LoadXMLFileToXMLParser(mapParser, filePath);

	m_cellDataString = mapParser.m_xmlNode.getChildNode().getChildNode().getText();

	m_CellDataTokens = SplitString(m_cellDataString, "\r\n");

	m_size.x = m_CellDataTokens[0].size();
	m_size.y = m_CellDataTokens.size();
	
	GenerateMapWithCellTokens(m_size.x, m_size.y);
}

//-----------------------------------------------------------------------------------------------------------

void Map::GenerateMapFromData(const XMLNode& mapNode){
	XMLNode mapDataNode = mapNode.getChildNode().getChildNode();
	if (!mapDataNode.IsContentEmpty()){
		m_cellDataString = mapDataNode.getText();
	}
	else{
		m_cellDataString = mapNode.getText();
	}

	m_CellDataTokens = SplitString(m_cellDataString, "\n");

	m_size.x = m_CellDataTokens[0].size();
	m_size.y = m_CellDataTokens.size();

	GenerateMapWithCellTokens(m_size.x, m_size.y);
}

//-----------------------------------------------------------------------------------------------------------

void Map::InitializeVisibilityMaskRenderer(){
	m_visibilityMaskRenderer = MeshRenderer();
	
	m_visibilityMaskRenderer.m_material = new Material();
	
	m_visibilityMaskRenderer.m_material->m_samplerInUse = false;
	m_visibilityMaskRenderer.m_material->InitializeMaterial("Data/Shaders/basic.vert", "Data/Shaders/basic.frag");
	
	m_visibilityMaskRenderer.m_mesh = new Mesh();
	m_visibilityMaskRenderer.m_mesh->SetDrawMode(GL_QUADS);
	
	m_visibilityMaskRenderer.BindVertexArray();

	//-----------------------------------------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------------------------------------

void Map::GenerateDefaultMap(){
	GenerateMap(TILE_X_WIDTH, TILE_Y_HEIGHT);
}

//-----------------------------------------------------------------------------------------------------------

void Map::SaveMapTileDataToXML(XMLNode& mapDataNode){
	std::string mapDataString;
	//TilesIterator tileIt = m_tiles.begin();
	for (int y = m_size.y-1; y >= 0; y--){
		mapDataString += "\n";
		for (int x = 0; x < m_size.x; x++){
		
		Tile& tile = *(GetTileAtMapPosition(IntVec2(x, y)));
	
		mapDataString += tile.WriteTileDataToXML(mapDataNode);

			//++tileIt;
		}
	}
	mapDataString += "\n";

	mapDataNode.addText(mapDataString.c_str());

	

}

//-----------------------------------------------------------------------------------------------------------

void Map::SaveMapKnownDataToXML(XMLNode& mapDataNode){
	std::string mapDataString;
	//TilesIterator tileIt = m_tiles.begin();
	for (int y = m_size.y - 1; y >= 0; y--){
		mapDataString += "\n";
		for (int x = 0; x < m_size.x; x++){


			Tile& tile = *(GetTileAtMapPosition(IntVec2(x, y)));
			//for (int y = 0; y < m_size.y; y++){


			mapDataString += tile.WriteTileStateDataToXML(mapDataNode);

			//++tileIt;
		}
	}
	// 	for (TilesIterator it = m_tiles.begin(); it != m_tiles.end(); ++ it ){
	// 		Tile& tile = (*it);
	// 
	// 		mapDataString += tile.WriteTileToXML(mapDataNode);
	// 		
	// 	}
	mapDataString += "\n";

	mapDataNode.addText(mapDataString.c_str());



}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///queries

const unsigned int Map::GetIndexFromMapPosition(const MapPosition& mapPosition){
	unsigned int mapIndex = mapPosition.x * (m_size.y) + mapPosition.y;

	if(IsTileIndexValid(mapIndex)){
		return mapIndex;
	}
	//return invalid index
	return m_tiles.size()+1;
}

//-----------------------------------------------------------------------------------------------------------

bool Map::IsTileIndexValid(const unsigned int& index){
	if (index >= 0 && index < m_tiles.size() ){
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------------------------------------

Tile* Map::GetTileAtMapPosition(const IntVec2& mapLocation){

	if (m_tiles.size() > 0){
		unsigned int mapIndex = GetIndexFromMapPosition(mapLocation);
		//crashes with 0 size
		TilesIterator mapIt = m_tiles.begin();

		if (IsTileIndexValid(mapIndex)){
			return &(*(mapIt + mapIndex));
		}
	}

	return NULL;

}

//-----------------------------------------------------------------------------------------------------------
///----------------------------------------------------------------------------------------------------------
///tile to check is the center of the box
TilePtrs Map::GetAllTilesWithinBoxOfTile(const Tile& tileToCheck, const int& width, const int& height){
	MapPosition tileMapPos = tileToCheck.m_mapPosition;
	TilePtrs tilesToReturn;
	tilesToReturn.reserve(10000);

	float xRadius = (float)width * 0.5f;
	float yRadius = (float)height * 0.5f;
	AABB2 tileBox = AABB2(Vector2(tileMapPos.x - xRadius, tileMapPos.y - yRadius), Vector2(tileMapPos.x + xRadius, tileMapPos.y + yRadius));

	for (TilesIterator it = m_tiles.begin(); it != m_tiles.end(); ++it){
		Tile& tile = (*it);

		if (DoAABBAndPointOverlap(tileBox, ToVector2(tile.m_mapPosition))){
			tilesToReturn.push_back(&tile);
		}
	}

	return tilesToReturn;
}

//-----------------------------------------------------------------------------------------------------------

Tile* Map::GetTileNearMapPositionInMapDirection(const IntVec2& position, const MapDirection& mapDirection, const int& tileStep ){

	IntVec2 posInMap = position;

	switch (mapDirection){
	case MAP_DIRECTION_WEST:
		//do stuff
		posInMap.x -= tileStep;

		return GetTileAtMapPosition(posInMap);

		break;
	case MAP_DIRECTION_EAST:
		//do stuff
		posInMap.x += tileStep;

		return GetTileAtMapPosition(posInMap);

		break;
	case MAP_DIRECTION_NORTH:
		//do stuff
		posInMap.y += tileStep;

		return GetTileAtMapPosition(posInMap);

		break;
	case MAP_DIRECTION_SOUTH:
		//do stuff
		posInMap.y -= tileStep;

		return GetTileAtMapPosition(posInMap);

		break;
	case MAP_DIRECTION_NORTHEAST:
		//do stuff
		posInMap.y += tileStep;
		posInMap.x -= tileStep;

		return GetTileAtMapPosition(posInMap);

		break;
	case MAP_DIRECTION_NORTHWEST:
		//do stuff
		posInMap.y += tileStep;
		posInMap.x += tileStep;

		return GetTileAtMapPosition(posInMap);

		break;
	case MAP_DIRECTION_SOUTHEAST:
		//do stuff
		posInMap.y -= tileStep;
		posInMap.x -= tileStep;

		return GetTileAtMapPosition(posInMap);

		break;
	case MAP_DIRECTION_SOUTHWEST:
		//do stuff
		posInMap.y -= tileStep;
		posInMap.x += tileStep;

		return GetTileAtMapPosition(posInMap);

		break;
	}

	return NULL;
}

//-----------------------------------------------------------------------------------------------------------

TilePtrs Map::GetValidAdjacentTiles(Tile* tileToCheck, bool ignoreEntities){
	TilePtrs adjacentTiles;
	adjacentTiles.reserve(32);
	adjacentTiles.clear();

	Tile* northTile = GetTileNearMapPositionInMapDirection(tileToCheck->m_mapPosition, MAP_DIRECTION_NORTH, 1);
	if (VerifyTileIsValidForMovement(northTile, ignoreEntities)){
		adjacentTiles.push_back(northTile);
	}

	Tile* southTile = GetTileNearMapPositionInMapDirection(tileToCheck->m_mapPosition, MAP_DIRECTION_SOUTH, 1);
	if (VerifyTileIsValidForMovement(southTile, ignoreEntities)){
		adjacentTiles.push_back(southTile);
	}

	Tile* eastTile = GetTileNearMapPositionInMapDirection(tileToCheck->m_mapPosition, MAP_DIRECTION_EAST, 1);
	if (VerifyTileIsValidForMovement(eastTile, ignoreEntities)){
		adjacentTiles.push_back(eastTile);
	}

	Tile* westTile = GetTileNearMapPositionInMapDirection(tileToCheck->m_mapPosition, MAP_DIRECTION_WEST, 1);
	if (VerifyTileIsValidForMovement(westTile, ignoreEntities)){
		adjacentTiles.push_back(westTile);
	}

	Tile* northeastTile = GetTileNearMapPositionInMapDirection(tileToCheck->m_mapPosition, MAP_DIRECTION_NORTHEAST, 1);
	if (VerifyTileIsValidForMovement(northeastTile, ignoreEntities)){
		adjacentTiles.push_back(northeastTile);
	}

	Tile* southeastTile = GetTileNearMapPositionInMapDirection(tileToCheck->m_mapPosition, MAP_DIRECTION_SOUTHEAST, 1);
	if (VerifyTileIsValidForMovement(southeastTile, ignoreEntities)){
		adjacentTiles.push_back(southeastTile);
	}

	Tile* northwestTile = GetTileNearMapPositionInMapDirection(tileToCheck->m_mapPosition, MAP_DIRECTION_NORTHWEST, 1);
	if (VerifyTileIsValidForMovement(northwestTile, ignoreEntities)){
		adjacentTiles.push_back(northwestTile);
	}

	Tile* southwestTile = GetTileNearMapPositionInMapDirection(tileToCheck->m_mapPosition, MAP_DIRECTION_SOUTHWEST, 1);
	if (VerifyTileIsValidForMovement(southwestTile, ignoreEntities)){
		adjacentTiles.push_back(southwestTile);
	}
	
	return adjacentTiles;
}

//-----------------------------------------------------------------------------------------------------------

TilePtrs Map::GetAdjacentTiles(Tile* tileToCheck){
	TilePtrs adjacentTiles;
	adjacentTiles.reserve(32);
	adjacentTiles.clear();

	Tile* northTile = GetTileNearMapPositionInMapDirection(tileToCheck->m_mapPosition, MAP_DIRECTION_NORTH, 1);
	//if (VerifyTileIsValidForMovement(northTile)){
		adjacentTiles.push_back(northTile);
	//}

	Tile* southTile = GetTileNearMapPositionInMapDirection(tileToCheck->m_mapPosition, MAP_DIRECTION_SOUTH, 1);
	//if (VerifyTileIsValidForMovement(southTile)){
		adjacentTiles.push_back(southTile);
	//}

	Tile* eastTile = GetTileNearMapPositionInMapDirection(tileToCheck->m_mapPosition, MAP_DIRECTION_EAST, 1);
	//if (VerifyTileIsValidForMovement(eastTile)){
		adjacentTiles.push_back(eastTile);
	//}

	Tile* westTile = GetTileNearMapPositionInMapDirection(tileToCheck->m_mapPosition, MAP_DIRECTION_WEST, 1);
	//if (VerifyTileIsValidForMovement(westTile)){
		adjacentTiles.push_back(westTile);
	//}

	Tile* northeastTile = GetTileNearMapPositionInMapDirection(tileToCheck->m_mapPosition, MAP_DIRECTION_NORTHEAST, 1);
	//if (VerifyTileIsValidForMovement(northeastTile)){
		adjacentTiles.push_back(northeastTile);
	//}

	Tile* southeastTile = GetTileNearMapPositionInMapDirection(tileToCheck->m_mapPosition, MAP_DIRECTION_SOUTHEAST, 1);
	//if (VerifyTileIsValidForMovement(southeastTile)){
		adjacentTiles.push_back(southeastTile);
	//}

	Tile* northwestTile = GetTileNearMapPositionInMapDirection(tileToCheck->m_mapPosition, MAP_DIRECTION_NORTHWEST, 1);
	//if (VerifyTileIsValidForMovement(northwestTile)){
		adjacentTiles.push_back(northwestTile);
	//}

	Tile* southwestTile = GetTileNearMapPositionInMapDirection(tileToCheck->m_mapPosition, MAP_DIRECTION_SOUTHWEST, 1);
	//if (VerifyTileIsValidForMovement(southwestTile)){
		adjacentTiles.push_back(southwestTile);
	//}

	return adjacentTiles;
}

//-----------------------------------------------------------------------------------------------------------
///----------------------------------------------------------------------------------------------------------
///update

void Map::Update(double deltaSeconds){
	for (TilesIterator it = m_tiles.begin(); it != m_tiles.end(); ++it){
		Tile& tile = (*it);

		tile.Update(deltaSeconds);

	}
}

///----------------------------------------------------------------------------------------------------------
///render methods

void Map::Render(OpenGLRenderer* renderer, bool is3DMode){
	//PROFILE_SECTION();
	//UNUSED(renderer);
	renderer->SetTextureViewTransparent();
	//renderer->SetModelViewDepthTest();
	//if there is a view then render it

	RenderTiles(renderer, is3DMode);
	
}

//-----------------------------------------------------------------------------------------------------------

void Map::RenderTileVisibility(OpenGLRenderer* renderer, bool is3DMode ){
	UNUSED(is3DMode);
	UNUSED(renderer);

	Vertex3Ds invisibleMaskQuads;
	Vertex3Ds hasBeenSeenMaskQuads;

	Rgba maskColor = Rgba(8, 8, 8, 0);

	for (TilesIterator it = m_tiles.begin(); it != m_tiles.end(); ++it){
		Tile& tile = (*it);
		
		//tile.DrawTileContents();
		AABB2 mapBounds = AABB2(Vector2(ToVector2(tile.m_mapPosition)), Vector2((float)(tile.m_mapPosition.x + 1), (float)(tile.m_mapPosition.y + 1)));

		if (!tile.m_tileState.visibility.m_isVisible){
			if (tile.m_tileState.visibility.m_hasBeenSeen){
				//invisible and has been seen
				maskColor.a = 180;
				//maskColor = Rgba(10, 10, 10, 250);
				
				GenerateVertexArrayTextureQuad(invisibleMaskQuads, mapBounds, AABB2::ZERO_TO_ONE, maskColor, false);

			}
			else{
				//invisible and has not been seen
				maskColor.a = 240;
				//maskColor = Rgba(0, 0, 0, 255);

				GenerateVertexArrayTextureQuad(invisibleMaskQuads, mapBounds, AABB2::ZERO_TO_ONE, maskColor, false);

			}
		}
	}

	IF_INPUT_SYSTEM_EXISTS{
		IF_INPUT_IS_KEY_RELEASED('M'){
			DebugDrawVisiblilityMask = !DebugDrawVisiblilityMask;
		}
	}

	m_visibilityMaskRenderer.m_mesh->CopyMeshVertexData(invisibleMaskQuads);

	if (DebugDrawVisiblilityMask){

		//m_visibilityMaskRenderer.RenderMesh2D(&(controlView->m_worldToViewMatrix));

		m_visibilityMaskRenderer.RenderMesh2D();
	}
}

//-----------------------------------------------------------------------------------------------------------

//renders in map space usually
void Map::RenderTiles(OpenGLRenderer* renderer, bool is3DMode, bool renderInWorldSpace ){
	PROFILE_SECTION();
	UNUSED(renderInWorldSpace);
	
	for (TilesIterator it = m_tiles.begin(); it != m_tiles.end(); ++it){
		Tile& tile = (*it);
		if (is3DMode){
			tile.Render3D(renderer, *m_mapViewCamera);
		}
		else{
			tile.Render2D(renderer, &m_mapToWorldTransformMatrix);
		}
	}

}

//-----------------------------------------------------------------------------------------------------------

Tile* Map::GetRandomTileOfType(const TileType& type){

	TilePtrs tilesOfType;
	tilesOfType.reserve(10000);

	for (TilesIterator it = m_tiles.begin(); it != m_tiles.end(); ++it){
		Tile& tile = (*it);

		if (tile.m_type == type){
			tilesOfType.push_back(&tile);
		}
	}

	int randomTileIndex = GetRandomIntInRange(1, tilesOfType.size() - 1);

	TilePtrsIterator it = tilesOfType.begin() + randomTileIndex;

	return (*it);
}

///----------------------------------------------------------------------------------------------------------
///friend methods

float GetManhattanDistanceBetweenMapPositions(const MapPosition& positionA, const MapPosition& positionB){

	int distanceABX = abs((positionA.x - positionB.x));
	int distanceABY = abs((positionA.y - positionB.y));

	return (float)abs(distanceABX + distanceABY);

}

//-----------------------------------------------------------------------------------------------------------

float GetAbsoluteDistanceBetweenMapPositions(const MapPosition& positionA, const MapPosition& positionB){

	return sqrt( abs( (float) CalcDistanceSquared(positionA, positionB) ) );

}

//===========================================================================================================
