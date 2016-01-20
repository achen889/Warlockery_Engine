//==============================================================================================================
//LineSegment2.cpp
//by Albert Chen Jan-26-2015.
//==============================================================================================================

#include "Engine/Math/LineSegment2.hpp"

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///constructors
LineSegment2::LineSegment2(){
	//do nothing
}
LineSegment2::~LineSegment2(){
	//do nothing
}
LineSegment2::LineSegment2(const Vector2& initialStart ,const Vector2& initialEnd ){
	startPoint = initialStart;
	endPoint   = initialEnd;
}
//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///Mutators
void LineSegment2::Translate(Vector2 translation ){
	startPoint += translation;
	endPoint   += translation;
}
//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///Accessors
const float LineSegment2::CalcLength() const{
	return sqrt( CalcLengthSquared() );
}
const float LineSegment2::CalcLengthSquared() const{
	float LengthX= abs ( startPoint.x - endPoint.x );
	float LengthY = abs ( startPoint.y - endPoint.y );
	return ( LengthX * LengthX + LengthY * LengthY );
}
//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
const float LineSegment2::CalcHeadingDegrees(){
	return CalcHeadingRadians() * DegreesPerRadian;
}

const float LineSegment2::CalcHeadingRadians(){
	float LengthX = endPoint.x - startPoint.x;
	float LengthY = endPoint.y - startPoint.y;
	return atan2(LengthY , LengthX);
}

Vector2 LineSegment2::GetPointAtParametricValue( float parametricValue ){
	Vector2 displacementStoE = endPoint - startPoint;
	Vector2 displacementStoP = parametricValue * displacementStoE;

	return startPoint + displacementStoP;
}
//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///operators
const LineSegment2 LineSegment2::operator+( const Vector2& translation ) const{
	return LineSegment2(startPoint + translation, endPoint + translation );
}
const LineSegment2 LineSegment2::operator-( const Vector2& antiTranslation ) const{
	return LineSegment2(startPoint - antiTranslation, endPoint - antiTranslation );
}
const LineSegment2 LineSegment2::operator*( const float scalingValue ){
	return LineSegment2(startPoint * scalingValue, endPoint * scalingValue );
}
void LineSegment2::operator+=( const Vector2& translation ){
	startPoint += translation;
	endPoint   += translation;
}
void LineSegment2::operator-=( const Vector2& antiTranslation ){
	startPoint -= antiTranslation;
	endPoint   -= antiTranslation;
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///friend functions
const LineSegment2 Interpolate( const LineSegment2& start, const LineSegment2& end, float fractionFromStartToEnd ){
	LineSegment2 adjustedDisplacementFromStartToEnd = LineSegment2( Vector2( end.startPoint - start.startPoint ), Vector2( end.endPoint - start.endPoint));

	adjustedDisplacementFromStartToEnd.startPoint *= fractionFromStartToEnd;
	adjustedDisplacementFromStartToEnd.endPoint *= fractionFromStartToEnd;
	adjustedDisplacementFromStartToEnd.startPoint += start.startPoint;
	adjustedDisplacementFromStartToEnd.endPoint += start.endPoint;

	return adjustedDisplacementFromStartToEnd;
}