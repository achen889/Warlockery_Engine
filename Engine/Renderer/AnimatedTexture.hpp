//==============================================================================================================
//AnimatedTexture.hpp
//by Albert Chen Feb-5-2015.
//==============================================================================================================

#ifndef _included_AnimatedTexture__
#define _included_AnimatedTexture__

#include "Texture.hpp"

/*
struct AnimatedTextureData{
	unsigned int cellWidth;
	unsigned int cellHeight;
	unsigned int totalNumberOfSprites;
};
*/


class AnimatedTexture : public Texture{
typedef std::map< std::string, AnimatedTexture* >AnimatedTextureMap;


public:
	virtual ~AnimatedTexture();
	static AnimatedTexture* CreateOrGetAnimatedTexture(const std::string& texturePath, unsigned int numberOfSprites, const Vector2& cellSize );
	void CalcTextureCoordinatesAtPercentComplete(float percentComplete, Vector2& minCoordinatesOut, Vector2& maxCoordinatesOut );

protected:
	AnimatedTexture( const std::string& pathName, int totalNumberOfSprites, const Vector2& initializedCellSize );

	static AnimatedTextureMap s_global_Animated_Textures;
	Vector2 cellSize;
	unsigned int m_TotalNumberOfSprites;
	unsigned int m_cellsPerRow;
	unsigned int m_cellsPerColumn;

};

inline AnimatedTexture::AnimatedTexture( const std::string& pathName, int totalNumberOfSprites, const Vector2& initializedCellSize ):Texture(pathName){
	
	m_TotalNumberOfSprites = totalNumberOfSprites;
	cellSize = initializedCellSize;

	m_cellsPerRow    = (unsigned int) floor(  m_size.x / cellSize.x  ); 
	m_cellsPerColumn = (unsigned int) floor(  m_size.y / cellSize.y );

}

inline AnimatedTexture::~AnimatedTexture(){
	//do nothing
}



#endif