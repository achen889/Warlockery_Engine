//==============================================================================================================
//Sphere3.hpp
//by Albert Chen Feb-18-2015.
//==============================================================================================================

#pragma once

#ifndef _included_Sphere3__
#define _included_Sphere3__

#include "Vector3.hpp"

class Sphere3{
public:
	Vector3 center;
	float radius;
	static const Sphere3 UNIT_CIRCLE;

	//constructors
	Sphere3();
	~Sphere3();
	Sphere3(const Sphere3& copy );
	explicit Sphere3(float initialX, float initialY, float initialZ, float initialRadius );
	explicit Sphere3(const Vector3& initialCenter, float initialRadius );
	//mutators
	void StretchToIncludePoint(const Vector3& point );
	void AddPadding(float paddingRadius );
	void Translate(const Vector3& translation );
	//accessors/queries
	bool IsPointInside(const Vector3& point) const;
	//operators
	//assignment operator
	const Sphere3& operator=( const Sphere3& sphereToAssign);
	const Sphere3 operator+(  const Vector3& translation ) const;
	const Sphere3 operator-(  const Vector3& antiTranslation ) const;
	void operator+=( const Vector3& translation );
	void operator-=( const Vector3& antiTranslation );

	//friend methods
	friend const Sphere3 Interpolate( const Sphere3& start, const Sphere3& end, float fractionFromStartToEnd );
	friend const Sphere3 Union(const Sphere3& sphereA, const Sphere3& sphereB);
	
	
};
//===========================================================================================================
//Constructors
inline Sphere3::Sphere3(){
	//do nothing
}
inline Sphere3::~Sphere3(){
	//do nothing
}
//add destructor
inline Sphere3::Sphere3(const Sphere3& copy ){
	center = copy.center;
	radius = copy.radius;
}
inline Sphere3::Sphere3(float initialX, float initialY, float initialZ, float initialRadius ){
	center.x = initialX;
	center.y = initialY;
	center.z = initialZ;
	radius = initialRadius;
}
inline Sphere3::Sphere3(const Vector3& initialCenter, float initialRadius ){
	center = initialCenter;
	radius = initialRadius;
}
///----------------------------------------------------------------------------------------------------------
///Mutators
///----------------------------------------------------------------------------------------------------------
///if the point is outside disc, increase radius to fit
inline void Sphere3::StretchToIncludePoint(const Vector3& point ){
	float distanceToPoint = CalcDistance(center, point);
	if(distanceToPoint > radius){
		radius = distanceToPoint;
	}
}
///adds padding radius as padding for the radius
inline void Sphere3::AddPadding(float paddingRadius){
	radius += paddingRadius;
}
///----------------------------------------------------------------------------------------------------------
///translate center to translation
inline void Sphere3::Translate(const Vector3& translation ){
	center += translation;
}
//accessors/queries
///----------------------------------------------------------------------------------------------------------
///checks distance squared to see if a point is inside
inline bool Sphere3::IsPointInside(const Vector3& point) const{
	bool IsInside =  CalcDistanceSquared(center, point) < (radius*radius);
	return IsInside;
}
///----------------------------------------------------------------------------------------------------------
///Operator methods
///----------------------------------------------------------------------------------------------------------
///Assignment Operator
inline const Sphere3& Sphere3::operator=(const Sphere3& sphereToAssign){
	center = sphereToAssign.center;
	radius = sphereToAssign.radius;
	return *this;
}
inline const Sphere3 Sphere3::operator+( const Vector3& translation ) const{
	return Sphere3( center + translation, radius);
}
inline const Sphere3 Sphere3::operator-( const Vector3& antiTranslation ) const{
	return Sphere3( center - antiTranslation, radius);
}
inline void Sphere3::operator+=( const Vector3& translation ){
	center += translation;
}
inline void Sphere3::operator-=( const Vector3& antiTranslation ){
	center -= antiTranslation;
}



#endif