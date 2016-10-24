//==============================================================================================================
//Math2D.hpp
//by Albert Chen Jan-21-2015.
//==============================================================================================================
#pragma once

#ifndef _included_Math2D__
#define _included_Math2D__

#include "AABB2.hpp"
#include "Disc2.hpp"
#include "LineSegment2.hpp"
#include "EngineMath.hpp"
#include "IntVec2.hpp"

//world const directions for vec2
static const Vector2 WORLD_UP_VEC2(0, 1);
static const Vector2 WORLD_DOWN_VEC2(0, -1);
static const Vector2 WORLD_LEFT_VEC2(-1, 0);
static const Vector2 WORLD_RIGHT_VEC2(1, 0);

struct TraceResult2D{
	bool didImpact;

	Vector2 impactPoint;

	Vector2 impactNormal;

};

//From Squirrel Eiserloh
// Typedef for a function type which takes a const-reference Vector2 and returns bool
typedef bool (Raycast2DIsImpactFunction)( const Vector2& targetCoords ); 

bool DoAABBsOverlap( const AABB2& first, const AABB2& second );
bool DoDiscsOverlap( const Disc2& first, const Disc2& second );

bool IsPointInsideDisc2(const Disc2& disc, const Vector2& point);

bool DoAABBAndPointOverlap(const Vector2& pointA, const AABB2& boxB);

bool DoAABBAndPointOverlap(const AABB2& boxA, const Vector2& pointB);

bool DoShapesOverlap(const AABB2& box, const Disc2& disc);
bool DoShapesOverlap( const Disc2& disc, const AABB2& box );

Vector2 ToVector2(const IntVec2& vectorToConvert );
IntVec2 ToIntVec2(const Vector2& vectorToConvert );

TraceResult2D Raycast2D(LineSegment2 rayToTrace, Raycast2DIsImpactFunction* isImpact2DCallback, const float& tStep = 0.01f );

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods

inline Vector2 ToVector2(const IntVec2& vectorToConvert ){
	return Vector2( (float)vectorToConvert.x, (float)vectorToConvert.y );
}

inline IntVec2 ToIntVec2(const Vector2& vectorToConvert ){
	return IntVec2( (int)vectorToConvert.x, (int)vectorToConvert.y );
}

#endif