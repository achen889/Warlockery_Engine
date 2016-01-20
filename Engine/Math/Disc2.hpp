//==============================================================================================================
//Disc2.hpp
//by Albert Chen Jan-21-2015.
//==============================================================================================================

#pragma once

#ifndef _included_Disc2D__
#define _included_Disc2D__

#include "Vector2.hpp"

class Disc2{
public:
	Vector2 center;
	float radius;
	static const Disc2 UNIT_CIRCLE;
	//constructors
	Disc2();
	~Disc2();
	Disc2(const Disc2& copy );
	explicit Disc2(float initialX, float initialY, float initialRadius );
	explicit Disc2(const Vector2& initialCenter, float initialRadius );
	//mutators
	void StretchToIncludePoint(const Vector2& point );
	void AddPadding(float paddingRadius );
	void Translate(const Vector2& translation );
	//accessors/queries
	bool IsPointInside(const Vector2& point) const;
	//operators
	const Disc2 operator+( const Vector2& translation ) const;
	const Disc2 operator-( const Vector2& antiTranslation ) const;
	void operator+=( const Vector2& translation );
	void operator-=( const Vector2& antiTranslation );

	//friend methods
	friend const Disc2 Interpolate( const Disc2& start, const Disc2& end, float fractionFromStartToEnd );
};
//===========================================================================================================
//Constructors
inline Disc2::Disc2(){
	//do nothing
}
inline Disc2::~Disc2(){
	//do nothing
}
//add destructor
inline Disc2::Disc2(const Disc2& copy ){
	center = copy.center;
	radius = copy.radius;
}
inline Disc2::Disc2(float initialX, float initialY, float initialRadius ){
	center.x = initialX;
	center.y = initialY;
	radius = initialRadius;
}
inline Disc2::Disc2(const Vector2& initialCenter, float initialRadius ){
	center = initialCenter;
	radius = initialRadius;
}
///----------------------------------------------------------------------------------------------------------
///Mutators
///----------------------------------------------------------------------------------------------------------
///adds padding radius as padding for the radius
inline void Disc2::AddPadding(float paddingRadius){
	radius += paddingRadius;
}
///----------------------------------------------------------------------------------------------------------
///translate center to translation
inline void Disc2::Translate(const Vector2& translation ){
	center +=translation;
}
///----------------------------------------------------------------------------------------------------------
///Operator methods
inline const Disc2 Disc2::operator+( const Vector2& translation ) const{
	return Disc2( center + translation, radius);
}
inline const Disc2 Disc2::operator-( const Vector2& antiTranslation ) const{
	return Disc2( center - antiTranslation, radius);
}
inline void Disc2::operator+=( const Vector2& translation ){
	center +=translation;
}
inline void Disc2::operator-=( const Vector2& antiTranslation ){
	center -= antiTranslation;
}

#endif