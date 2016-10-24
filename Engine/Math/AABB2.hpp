//==============================================================================================================
//AABB2.hpp
//by Albert Chen Feb-18-2015.
//==============================================================================================================

#pragma once

#ifndef _included_AABB2__
#define _included_AABB2__

#include "EngineMath.hpp"

class AABB2{
public:
	Vector2 mins;
	Vector2 maxs;
	static const AABB2 ZERO;
	static const AABB2 ZERO_TO_ONE;
	static const AABB2 NEG_ONE_TO_POS_ONE;
	//constructors
	AABB2();
	~AABB2();
	AABB2(const AABB2& copy, float paddingX = 0.0f, float paddingY = 0.0f);
	explicit AABB2(float initialX, float initialY );
	explicit AABB2(const Vector2& mins, const Vector2& maxs );
	explicit AABB2(const Vector2& center, float radiusX, float radiusY );
	explicit AABB2(const Vector2& center, float radius );
	//mutators
	void StretchToIncludePoint(const Vector2& point );
	void AddPadding(float xPaddingRadius, float yPaddingRadius );
	void Translate(const Vector2& translation );
	void RotateDegrees(float rotationDegrees);
	//void ScaleNonUniform(const Vector2& scaleFactor);
	//accessors/queries
	bool isPointInside(const Vector2& point );
	const Vector2 CalcSize() const; //this is the last thing to do for aabb2
	Vector2 CalcCenter() const;
	Vector2 CalcLengthOfSides() const {
		Vector2 tempVecLength;
		tempVecLength.x = abs(maxs.x - mins.x);
		tempVecLength.y = abs(maxs.y - mins.y);

		return tempVecLength;
	}
	float CalcArea() const {
		Vector2 boxSides = CalcLengthOfSides();
		return boxSides.x * boxSides.y;
	}
	Vector2s GetAABB2Vertices();
	const Vector2 GetPointAtNormalizedPositionWithinBox( const Vector2& normalizedPosition ) const;
	const Vector2 GetNormalizedPositionForPointWithinBox( const Vector2& point ) const;
	//const Vector2s GetVerticesOfAABB2();

	//operators
	bool operator==(const AABB2& boxToEqual) const;
	bool operator!=(const AABB2& boxToNotEqual) const;
	const AABB2 operator+( const Vector2& translation ) const;
	const AABB2 operator-( const Vector2& antiTranslation ) const;
	void operator+=( const Vector2& translation );
	void operator-=( const Vector2& antiTranslation );

	//friend methods
	friend const AABB2 Interpolate( const AABB2& start, const AABB2& end, float fractionFromStartToEnd ); 
	friend void ConsolePrintAABB2(AABB2 consoleAABB2 );
	friend Vector2s GetTextureCoordinatesFromAABB2(AABB2 boxWithTextureCoords );
	friend Vector2s GetDefaultTextureCoordinates();
};
//===========================================================================================================
//Constructors
inline AABB2::AABB2(){
	//do nothing
}
inline AABB2::~AABB2(){
	//do nothing
}
inline AABB2::AABB2(const AABB2& copy, float paddingX, float paddingY ){
	mins = copy.mins;
	maxs = copy.maxs;

	AddPadding(paddingX, paddingY);
}
inline AABB2::AABB2(float initialX, float initialY ){
	mins = Vector2(initialX, initialY);
	maxs = Vector2(initialX, initialY);
}
inline AABB2::AABB2(const Vector2& initialMins, const Vector2& initialMaxs ){
	mins = initialMins;
	maxs = initialMaxs;
}
inline AABB2::AABB2(const Vector2& center, float radiusX, float radiusY ){
	//do something
	mins.x = center.x - radiusX;
	mins.y = center.y - radiusY;
	maxs.x = center.x + radiusX;
	maxs.y = center.y + radiusY;
}
inline AABB2::AABB2(const Vector2& center, float radius) {
	//do something
	mins.x = center.x - radius;
	mins.y = center.y - radius;
	maxs.x = center.x + radius;
	maxs.y = center.y + radius;
}

//mutators
///----------------------------------------------------------------------------------------------------------
///add padding to each side of the box
inline void AABB2::AddPadding(float xPaddingRadius, float yPaddingRadius ){
	mins -= Vector2(xPaddingRadius, yPaddingRadius);
	maxs += Vector2(xPaddingRadius, yPaddingRadius);
}
///----------------------------------------------------------------------------------------------------------
///move box by this translation
inline void AABB2::Translate(const Vector2& translation ){
	mins += translation;
	maxs += translation;
}

///----------------------------------------------------------------------------------------------------------
///rotate box by this degrees
inline void AABB2::RotateDegrees(float rotationDegrees){

// 	Vector2 myCenter = CalcCenter();
// 	Vector2 myRadius = CalcLengthOfSides() * 0.5f;
// 	//rotate center
// 	myCenter.RotateDegrees(rotationDegrees);
// 	//*this = AABB2(myCenter, myRadius.x, myRadius.y);
// 
// 	mins = myCenter - myRadius;
// 	maxs = myCenter + myRadius;

  mins.RotateDegrees(rotationDegrees);
  maxs.RotateDegrees(rotationDegrees);

}

///----------------------------------------------------------------------------------------------------------
///returns width and height of box
inline const Vector2 AABB2::CalcSize() const{
	float BoxXWidth = maxs.x - mins.x;
	float BoxYHeight = maxs.y - mins.y;
	return Vector2(BoxXWidth, BoxYHeight );
}
///----------------------------------------------------------------------------------------------------------
///returns center of AABB
inline Vector2 AABB2::CalcCenter() const{
	float BoxXWidth = maxs.x - mins.x;
	float BoxYHeight = maxs.y - mins.y;
	float CenterX = mins.x + (BoxXWidth * 0.5f );
	float CenterY = mins.y + (BoxYHeight * 0.5f );
	return Vector2(CenterX, CenterY );
}
///----------------------------------------------------------------------------------------------------------
///get vertices of box in counterclockwise order
inline Vector2s AABB2::GetAABB2Vertices(){
	Vector2s boxVertices;

	boxVertices.push_back(mins );
	boxVertices.push_back(Vector2( maxs.x , mins.y) );
	boxVertices.push_back(maxs );
	boxVertices.push_back(Vector2( mins.x , maxs.y) );

	return boxVertices;
}

///----------------------------------------------------------------------------------------------------------
///gets the point for the normalized pos
inline const Vector2 AABB2::GetPointAtNormalizedPositionWithinBox( const Vector2& normalizedPosition ) const{
	float width = maxs.x - mins.x;
	float length = maxs.y - mins.y;

	float PointAtNormalizedPositionX = mins.x + ( width * normalizedPosition.x );
	float PointAtNormalizedPositionY = mins.y + ( length * normalizedPosition.y );

	return Vector2(PointAtNormalizedPositionX, PointAtNormalizedPositionY);
}

inline void ConsolePrintAABB2(AABB2 consoleAABB2 ){
	ConsolePrintf("AABB2< mins< ");
	ConsolePrintVector2(consoleAABB2.mins);
	ConsolePrintf(", maxs<");
	ConsolePrintVector2(consoleAABB2.maxs);
	ConsolePrintf("> >");
}


typedef std::vector<AABB2>AABB2s;

#endif

