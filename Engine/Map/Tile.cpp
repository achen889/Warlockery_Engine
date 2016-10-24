//==============================================================================================================
//Tile.cpp
//by Albert Chen Aug-18-2015.
//==============================================================================================================

#include "Tile.hpp"
#include "Engine\Renderer\Text\TextSystem.hpp"

//===========================================================================================================


//===========================================================================================================

//constructors

Tile::Tile(const int& mapIndex, const IntVec2& loc, const TileType& tileType):
m_mapIndex(mapIndex),
m_mapPosition(loc)
{

	SetRenderBounds();

	ChangeTileType(tileType);

	m_tileState.visibility = TileVisibility( false, false, 1.0f);
	m_tileState.contents = TileContents();

	//set render component
	m_renderComponent = RenderComponent("TileRenderer" + IntToString(m_mapIndex));

	//m_mapHeight = GetRandomFloatInRange(-0.5f, 0.5f);

	m_color = Rgba::WHITE;
	GetTileDetailsForTileType();
	
}

///----------------------------------------------------------------------------------------------------------
///construction helpers

const Vertex3Ds Tile::GetTileVertex3Ds(bool clearVerts){
	UNUSED(clearVerts);

	Vertex3Ds tileVerts;
	tileVerts.reserve(4);
	//tileVerts.clear();

	//flickering effect
	//m_color.a = 255 * ( 0.5f + GetRandomFloatZeroToOne() * 0.35f);
	//GetAABB2TextureCoords


	AABB2 tileCoords = AABB2::ZERO_TO_ONE;

	//specific to that netmage dfs
// 	AABB2 tileCoords = TILE_HIGHLIGHT_TEX_COORDS;
// 	if (m_type == TILE_TYPE_SOLID){
// 		tileCoords = TILE_SOLID_TEX_COORDS;
// 	}
	
	GenerateVertexArrayTextureQuad(tileVerts, GetRenderBounds(), tileCoords, m_color, false);

	return tileVerts;
}

//-----------------------------------------------------------------------------------------------------------

AABB2 Tile::GetRenderBounds(){

	return AABB2(Vector2(ToVector2(m_mapPosition)), Vector2((float)(m_mapPosition.x + 1), (float)(m_mapPosition.y + 1)));

}


//-----------------------------------------------------------------------------------------------------------

void Tile::SetRenderComponent2D(){

	m_renderComponent.SetRenderMesh2D(ToVector2(m_mapPosition), Vector2::ONE, m_color, "");

}

//-----------------------------------------------------------------------------------------------------------

void Tile::SetRenderComponent3D(){

	m_renderComponent.SetRenderMesh3D(ToVector3(ToVector2(m_mapPosition), 0.0f), ToVector3(Vector2::ONE, m_mapHeight), m_color, "");

}

//-----------------------------------------------------------------------------------------------------------
//expand this to deal with tile state
const char Tile::GetTileDetailsForTileType(){

	char m_symbol = ' ';

	if (m_type == TILE_TYPE_AIR){
	    m_color = Rgba::WHITE;
		m_symbol = '.';
	}
	else if (m_type == TILE_TYPE_SOLID){
		m_color = Rgba::GRAY;
		m_symbol = '#';
	}
	return m_symbol;

}


//-----------------------------------------------------------------------------------------------------------
///----------------------------------------------------------------------------------------------------------
///update

void Tile::Update(double deltaSeconds){
	UNUSED(deltaSeconds);
	


}



///----------------------------------------------------------------------------------------------------------
///render

void Tile::Render2D(OpenGLRenderer* renderer, ModelViewMatrix* modelView, Light* light){
	PROFILE_SECTION(); //0.4 - 0.6 ms
	
	m_renderComponent.Render2D(renderer, modelView, light);
	
}

//-----------------------------------------------------------------------------------------------------------

void Tile::Render3D(OpenGLRenderer* renderer, Camera3D& camera, bool isPerspective , Lights* lights, ModelViewMatrix* modelView){
	PROFILE_SECTION(); //0.6 - 0.8 ms
	
	m_renderComponent.Render3D(renderer, camera, isPerspective, lights, modelView);

}

//-----------------------------------------------------------------------------------------------------------

bool VerifyTileIsValidForMovement(Tile* tile, bool ignoreEntities){

	if (tile){
		bool isTileEmpty = (tile->IsEmpty() == true) || ignoreEntities;
// 		if (ignoreEntities){
// 			isTileEmpty = true;
// 		}

		if (tile->m_type != TILE_TYPE_SOLID && isTileEmpty){

			return true;

		}
	}

	return false;

}


