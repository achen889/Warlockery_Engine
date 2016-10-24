//==============================================================================================================
//Disc2D.cpp
//by Albert Chen Jan-21-2015.
//==============================================================================================================

#include "Disc2.hpp"

///----------------------------------------------------------------------------------------------------------
///if the point is outside disc, increase radius to fit
void Disc2::StretchToIncludePoint(const Vector2& point ){
	float distanceToPoint = CalcDistance(center, point);
	if(distanceToPoint > radius){
		radius = distanceToPoint;
	}
}
///----------------------------------------------------------------------------------------------------------
///checks distance squared to see if a point is inside
bool Disc2::IsPointInside(const Vector2& point) const{
	bool IsInside =  CalcDistanceSquared(center, point) < (radius*radius);
	return IsInside;
}
//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///friend functions
const Disc2 Interpolate( const Disc2& start, const Disc2& end, float fractionFromStartToEnd ){
	Vector2 displacementCenterStartToEnd = end.center - start.center;
	float displacementRadiusStartToEnd = end.radius - start.radius;

	Disc2 interpolatedDisc ( (displacementCenterStartToEnd * fractionFromStartToEnd) , ( displacementRadiusStartToEnd * fractionFromStartToEnd) );
	interpolatedDisc.center +=start.center;
	interpolatedDisc.radius +=start.radius;

	return interpolatedDisc;
}

//===========================================================================================================
