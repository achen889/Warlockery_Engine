//==============================================================================================================
//AABB3.hpp
//by Albert Chen Feb-18-2015.
//==============================================================================================================

#pragma once

#ifndef _included_AABB3__
#define _included_AABB3__

#include "EngineMath.hpp"
#include "Vector3.hpp"

class AABB3{
public:
	Vector3 mins;
	Vector3 maxs;
	static const AABB3 ZERO_TO_ONE;
	
	//constructors
	AABB3();
	~AABB3();
	AABB3(const AABB3& copy );
	explicit AABB3(float initialX, float initialY, float initialZ );
	explicit AABB3(const Vector3& mins, const Vector3& maxs );
	explicit AABB3(const Vector3& center, float radiusX, float radiusY, float radiusZ );
	//mutators
	void StretchToIncludePoint(const Vector3& point );
	void AddPadding(float xPaddingRadius, float yPaddingRadius, float zPaddingRadius );
	void Translate(const Vector3& translation );
	//accessors/queries
	bool isPointInside(const Vector3& point );
	const Vector3 CalcSize() const;
	const Vector3 CalcCenter() const;
	const Vector3 GetPointAtNormalizedPositionWithinBox( const Vector3& normalizedPosition ) const;
	const Vector3 GetNormalizedPositionForPointWithinBox( const Vector3& point ) const;
	//operators
	const AABB3 operator+( const Vector3& translation ) const;
	const AABB3 operator-( const Vector3& antiTranslation ) const;
	void operator+=( const Vector3& translation );
	void operator-=( const Vector3& antiTranslation );
	//friend methods
	friend const AABB3 Interpolate( const AABB3& start, const AABB3& end, float fractionFromStartToEnd );
};
//===========================================================================================================
//Constructors
inline AABB3::AABB3(){
	//do nothing
}
inline AABB3::~AABB3(){
	//do nothing
}
inline AABB3::AABB3(const AABB3& copy ){
	mins = copy.mins;
	maxs = copy.maxs;
}
inline AABB3::AABB3(float initialX, float initialY, float initialZ ){
	mins = Vector3(initialX, initialY, initialZ  );
	maxs = Vector3(initialX, initialY, initialZ  );
}
inline AABB3::AABB3(const Vector3& initialMins, const Vector3& initialMaxs ){
	mins = initialMins;
	maxs = initialMaxs;
}
inline AABB3::AABB3(const Vector3& center, float radiusX, float radiusY, float radiusZ ){
	//do something
	mins.x = center.x - radiusX;
	mins.y = center.y - radiusY;
	mins.z = center.z - radiusZ;
	maxs.x = center.x + radiusX;
	maxs.y = center.y + radiusY;
	maxs.z = center.z + radiusZ;
}
//mutators
///----------------------------------------------------------------------------------------------------------
///add padding to each side of the box
inline void AABB3::AddPadding(float xPaddingRadius, float yPaddingRadius, float zPaddingRadius ){
	mins -= Vector3(xPaddingRadius, yPaddingRadius, zPaddingRadius );
	maxs += Vector3(xPaddingRadius, yPaddingRadius, zPaddingRadius );
}
///----------------------------------------------------------------------------------------------------------
///move box by this translation
inline void AABB3::Translate(const Vector3& translation ){
	mins += translation;
	maxs += translation;
}
//queries
///----------------------------------------------------------------------------------------------------------
///checks if point is inside AABB2
inline bool AABB3::isPointInside(const Vector3& point ){
	if( point.x > mins.x &&
		point.y > mins.y && 
		point.z > mins.z && 
		point.x < maxs.x && 
		point.y < maxs.y &&
		point.z < maxs.z ){
			return true;
	}
	return false;
}
///----------------------------------------------------------------------------------------------------------
///returns width and height of box
inline const Vector3 AABB3::CalcSize() const{
	float BoxXLength = maxs.x - mins.x;
	float BoxYWidth = maxs.y - mins.y;
	float BoxZHeight = maxs.z - mins.z;
	return Vector3(BoxXLength, BoxYWidth, BoxZHeight );
}
///----------------------------------------------------------------------------------------------------------
///returns center of AABB
inline const Vector3 AABB3::CalcCenter() const{
	float BoxXLength = maxs.x - mins.x;
	float BoxYWidth = maxs.y - mins.y;
	float BoxZHeight = maxs.z - mins.z;

	float CenterX = mins.x + (BoxXLength * 0.5f );
	float CenterY = mins.y + (BoxYWidth * 0.5f );
	float CenterZ = mins.z + (BoxZHeight * 0.5f );
	return Vector3(CenterX, CenterY, CenterZ );
}
///----------------------------------------------------------------------------------------------------------
///gets the point for the normalized pos
inline const Vector3 AABB3::GetPointAtNormalizedPositionWithinBox( const Vector3& normalizedPosition ) const{
	float width = maxs.x - mins.x;
	float length = maxs.y - mins.y;
	float height = maxs.z - mins.z;

	float PointAtNormalizedPositionX = mins.x + ( width * normalizedPosition.x );
	float PointAtNormalizedPositionY = mins.y + ( length * normalizedPosition.y );
	float PointAtNormalizedPositionZ = mins.z + ( height * normalizedPosition.z );

	return Vector3(PointAtNormalizedPositionX, PointAtNormalizedPositionY, PointAtNormalizedPositionZ);
}

inline void ConsolePrintAABB3(AABB3 consoleAABB3 ){
	ConsolePrintf("AABB3<mins< ");
	ConsolePrintVector3(consoleAABB3.mins);
	ConsolePrintf(", maxs< ");
	ConsolePrintVector3(consoleAABB3.maxs);
	ConsolePrintf("> >");
}

//-----------------------------------------------------------------------------------------------------------

typedef std::vector<AABB3>AABB3s;

#endif

