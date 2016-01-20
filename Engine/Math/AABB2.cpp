//==============================================================================================================
//AABB2.cpp
//by Albert Chen Jan-21-2015.
//==============================================================================================================
#define STATIC
#include "AABB2.hpp"

STATIC const AABB2 AABB2::ZERO = AABB2(Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f));
STATIC const AABB2 AABB2::ZERO_TO_ONE = AABB2( Vector2(0.0f,0.0f), Vector2(1.0f,1.0f) );
STATIC const AABB2 AABB2::NEG_ONE_TO_POS_ONE = AABB2( Vector2(-1.0f,-1.0f), Vector2(1.0f,1.0f) );
///----------------------------------------------------------------------------------------------------------
///stretches the box to include the point
void AABB2::StretchToIncludePoint(const Vector2& point ){
	if(point.x < mins.x){
		mins.x = point.x;
	}
	if(point.y < mins.y){
		mins.y = point.y;
	}
	if(point.x > maxs.x){
		maxs.x = point.x;
	}
	if(point.y > maxs.y){
		maxs.y = point.y;
	}
}
///----------------------------------------------------------------------------------------------------------
///checks if point is inside AABB2
bool AABB2::isPointInside(const Vector2& point ){
	if( point.x > mins.x &&
		point.y > mins.y && 
		point.x < maxs.x && 
		point.y < maxs.y ){
		return true;
	}
	return false;
}

const Vector2 AABB2::GetNormalizedPositionForPointWithinBox( const Vector2& point ) const{

	float width = maxs.x - mins.x;
	float length = maxs.y - mins.y;

	float NormalizedPositionX = 0.0f;
	float NormalizedPositionY = 0.0f;

	if(width != 0.0f)
		NormalizedPositionX = (point.x - mins.x ) / width;
	if(length != 0.0f)
		NormalizedPositionY = (point.y - mins.y ) / length;

	return Vector2(NormalizedPositionX, NormalizedPositionY);
}

// const Vector2s GetVerticesOfAABB2(){
// 	Vector2s verticesOfAABB2Corners;
// 	verticesOfAABB2Corners.push_back();
// }

///----------------------------------------------------------------------------------------------------------
///Operators

bool AABB2::operator==(const AABB2& boxToEqual) const{
	if (mins == boxToEqual.mins && maxs == boxToEqual.maxs){
		return true;
	}
	return false;
}
bool AABB2::operator!=(const AABB2& boxToNotEqual) const{
	if (mins != boxToNotEqual.mins || maxs != boxToNotEqual.maxs){
		return true;
	}
	return false;
}

const AABB2 AABB2::operator+( const Vector2& translation ) const{
	Vector2 sumMins (mins + translation);
	Vector2 sumMaxs (maxs + translation);
	return AABB2(sumMins, sumMaxs);
}
const AABB2 AABB2::operator-( const Vector2& antiTranslation ) const{
	Vector2 differenceMins (mins - antiTranslation);
	Vector2 differenceMaxs (maxs - antiTranslation);
	return AABB2(differenceMins, differenceMaxs);
}
void AABB2::operator+=( const Vector2& translation ){
	mins += translation;
	maxs += translation;
}
void AABB2::operator-=( const Vector2& antiTranslation ){
	mins -= antiTranslation;
	maxs -= antiTranslation;
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///friend methods
const AABB2 Interpolate( const AABB2& start, const AABB2& end, float fractionFromStartToEnd ){

	Vector2 displacementMinFromStartToEnd = end.mins - start.mins;
	Vector2 displacementMaxFromStartToEnd = end.maxs - start.maxs;

	Vector2 interpolatedMins = start.mins + ( displacementMinFromStartToEnd * fractionFromStartToEnd );
	Vector2 interpolatedMaxs = start.maxs + ( displacementMaxFromStartToEnd * fractionFromStartToEnd );

	return AABB2( interpolatedMins , interpolatedMaxs ); 
}

Vector2s GetDefaultTextureCoordinates(){
	return GetTextureCoordinatesFromAABB2(AABB2::ZERO_TO_ONE);
}

//bottom right, counterclockwise
Vector2s GetTextureCoordinatesFromAABB2(AABB2 boxWithTextureCoords ){
	Vector2s outBoxTextureCoordinates;
	outBoxTextureCoordinates.push_back(Vector2(boxWithTextureCoords.mins.x , boxWithTextureCoords.maxs.y ) ); 
	outBoxTextureCoordinates.push_back(Vector2(boxWithTextureCoords.maxs.x , boxWithTextureCoords.maxs.y ) ); 
	outBoxTextureCoordinates.push_back(Vector2(boxWithTextureCoords.maxs.x , boxWithTextureCoords.mins.y ) ); 
	outBoxTextureCoordinates.push_back(Vector2(boxWithTextureCoords.mins.x , boxWithTextureCoords.mins.y ) );

	return outBoxTextureCoordinates;
}



