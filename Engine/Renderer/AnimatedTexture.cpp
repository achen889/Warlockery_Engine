//==============================================================================================================
//AnimatedTexture.cpp
//by Albert Chen Feb-5-2015.
//==============================================================================================================

#include "AnimatedTexture.hpp"

std::map< std::string, AnimatedTexture* >	AnimatedTexture::s_global_Animated_Textures;

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///Create or get animated texture
AnimatedTexture* AnimatedTexture::CreateOrGetAnimatedTexture(const std::string& texturePath, unsigned int numberOfSprites, const Vector2& cellSize ){

	AnimatedTextureMap::iterator animatedTextureIter = s_global_Animated_Textures.find( texturePath );
	if ( animatedTextureIter != s_global_Animated_Textures.end() )
		return animatedTextureIter->second ;

	AnimatedTexture*	newAnimatedTexture	= new AnimatedTexture(texturePath,  numberOfSprites, cellSize );
	s_global_Animated_Textures[texturePath ]	= newAnimatedTexture;
	return newAnimatedTexture;
}

void AnimatedTexture::CalcTextureCoordinatesAtPercentComplete(float percentComplete, Vector2& minCoordinatesOut, Vector2& maxCoordinatesOut ){
	
	unsigned int animatedTextureIDAtPercentComplete = (int) floor( m_TotalNumberOfSprites * percentComplete );

	int coordinateRowofTextureID = 0;
	int coordinateColumnofTextureID = 0;

	while(animatedTextureIDAtPercentComplete > m_cellsPerRow-1){
		animatedTextureIDAtPercentComplete-= m_cellsPerRow;
		coordinateColumnofTextureID++;
		
	}
	coordinateRowofTextureID = animatedTextureIDAtPercentComplete;

	
	
	minCoordinatesOut = Vector2((float)coordinateRowofTextureID, (float)coordinateColumnofTextureID );
	minCoordinatesOut.ScaleNonUniform(cellSize );
	minCoordinatesOut.InverseScaleNonUniform(m_size );
	//minCoordinatesOut.Normalize();

	maxCoordinatesOut = Vector2(coordinateRowofTextureID + 1.0f , coordinateColumnofTextureID + 1.0f );
	maxCoordinatesOut.ScaleNonUniform(cellSize );
	maxCoordinatesOut.InverseScaleNonUniform(m_size );
	//maxCoordinatesOut.Normalize();
	

}