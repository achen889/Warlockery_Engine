//==============================================================================================================
//Math3D.hpp
//by Albert Chen Feb-18-2015.
//==============================================================================================================

#pragma once

#ifndef _included_Math3D__
#define _included_Math3D__

#include "AABB3.hpp"
#include "Sphere3.hpp"
#include "LineSegment3.hpp"
#include "EngineMath.hpp"
#include "Vector4.hpp"
#include "AABB2.hpp"

struct RayCastResult3D{
	bool didImpact;
	Vector3 impactPoint;
};

bool DoAABB3sOverlap( const AABB3& first, const AABB3& second );
bool DoSpheresOverlap( const Sphere3& first, const Sphere3& second );

bool IsPointInsideSphere(const Vector3& point, const Sphere3& sphere );

const Sphere3 CalcBoundingSphere(const Vector3s& pointCloud );
// 
// RayCastResult3D Raycast3D(LineSegment3 rayToTrace, bool impactCondition , const float& tStep = 0.01f );

//conversions
Vector2 ToVector2(const Vector3& vector3ToConvert );

Vector3 ToVector3(const Vector2& vector2ToConvert, const float& zVal = 0.0f);
Vector3 ToVector3(const Vector4& vector4ToConvert );
Vector4 ToVector4(const Vector3& vector3ToConvert, const float& newW = 0.0f );
Vector4 ToVector4(const Vector2& vector2ToConvert, const float& zVal = 0.0f, const float& newW = 1.0f);

AABB2 ToAABB2(const AABB3& boxToConvert);

inline AABB2 ToAABB2(const AABB3& boxToConvert){
	Vector2 boxBMins = ToVector2(boxToConvert.mins);
	Vector2 boxBMaxs = ToVector2(boxToConvert.maxs);

	return AABB2(boxBMins, boxBMaxs);
}
//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods
//returns the x,y components of the vector3
inline Vector2 ToVector2(const Vector3& vector3ToConvert ){
	return Vector2(vector3ToConvert.x,vector3ToConvert.y );
}

//returns a vector 3 of x,y,0
inline Vector3 ToVector3(const Vector2& vector2ToConvert, const float& zVal ){
	return Vector3(vector2ToConvert.x,vector2ToConvert.y, zVal );
}

inline Vector3 ToVector3(const Vector4& vector4ToConvert ){
	return Vector3(vector4ToConvert.x, vector4ToConvert.y, vector4ToConvert.z );
}

inline Vector4 ToVector4(const Vector3& vector3ToConvert, const float& wVal){
	return Vector4(vector3ToConvert.x, vector3ToConvert.y, vector3ToConvert.z, wVal);
}

inline Vector4 ToVector4(const Vector2& vector2ToConvert, const float& zVal , const float& wVal ){
	return Vector4(vector2ToConvert.x, vector2ToConvert.y, zVal, wVal);
}

inline Vector2 ToVector2(const Vector4& vector4ToConvert){
	return Vector2(vector4ToConvert.x, vector4ToConvert.y);
}


#endif