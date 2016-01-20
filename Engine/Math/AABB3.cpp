//==============================================================================================================
//AABB2.cpp
//by Albert Chen Jan-21-2015.
//==============================================================================================================
#define STATIC
#include "AABB3.hpp"

STATIC const AABB3 AABB3::ZERO_TO_ONE = AABB3( Vector3(0.0f,0.0f,0.0f), Vector3(1.0f,1.0f,1.0f) );

///----------------------------------------------------------------------------------------------------------
///stretches the box to include the point
void AABB3::StretchToIncludePoint(const Vector3& point ){
	if(point.x < mins.x){
		mins.x = point.x;
	}
	if(point.y < mins.y){
		mins.y = point.y;
	}
	if(point.z < mins.z){
		mins.z = point.z;
	}

	if(point.x > maxs.x){
		maxs.x = point.x;
	}
	if(point.y > maxs.y){
		maxs.y = point.y;
	}
	if(point.z > maxs.z){
		maxs.z = point.z;
	}
}
///----------------------------------------------------------------------------------------------------------
///gets normalized pos for point in box

const Vector3 AABB3::GetNormalizedPositionForPointWithinBox( const Vector3& point ) const{

	float width = maxs.x - mins.x;
	float length = maxs.y - mins.y;
	float height = maxs.z - mins.z;

	float NormalizedPositionX = 0.0f;
	float NormalizedPositionY = 0.0f;
	float NormalizedPositionZ = 0.0f;

	if(width != 0.0f)
		NormalizedPositionX = (point.x - mins.x ) / width;
	if(length != 0.0f)
		NormalizedPositionY = (point.y - mins.y ) / length;
	if(height != 0.0f)
		NormalizedPositionZ = (point.z - mins.z ) / height;

	return Vector3(NormalizedPositionX, NormalizedPositionY, NormalizedPositionZ);
}
///----------------------------------------------------------------------------------------------------------
///Operators
const AABB3 AABB3::operator+( const Vector3& translation ) const{
	Vector3 sumMins (mins + translation);
	Vector3 sumMaxs (maxs + translation);
	return AABB3(sumMins, sumMaxs);
}
const AABB3 AABB3::operator-( const Vector3& antiTranslation ) const{
	Vector3 differenceMins (mins - antiTranslation);
	Vector3 differenceMaxs (maxs - antiTranslation);
	return AABB3(differenceMins, differenceMaxs);
}
void AABB3::operator+=( const Vector3& translation ){
	mins += translation;
	maxs += translation;
}
void AABB3::operator-=( const Vector3& antiTranslation ){
	mins -= antiTranslation;
	maxs -= antiTranslation;
}
//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///friend methods
const AABB3 Interpolate( const AABB3& start, const AABB3& end, float fractionFromStartToEnd ){

	Vector3 displacementMinFromStartToEnd = end.mins - start.mins;
	Vector3 displacementMaxFromStartToEnd = end.maxs - start.maxs;

	Vector3 interpolatedMins = start.mins + ( displacementMinFromStartToEnd * fractionFromStartToEnd );
	Vector3 interpolatedMaxs = start.maxs + ( displacementMaxFromStartToEnd * fractionFromStartToEnd );

	return AABB3( interpolatedMins , interpolatedMaxs ); 
}




