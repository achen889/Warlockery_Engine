//==============================================================================================================
//LineSegment3.cpp
//by Albert Chen Feb-24-2015.
//==============================================================================================================

#include "Engine/Math/LineSegment3.hpp"

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///constructors
LineSegment3::LineSegment3(){
	//do nothing
}
LineSegment3::~LineSegment3(){
	//do nothing
}
LineSegment3::LineSegment3(const Vector3& initialStart ,const Vector3& initialEnd ){
	startPoint = initialStart;
	endPoint   = initialEnd;
}
//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///Mutators
void LineSegment3::Translate(Vector3 translation ){
	startPoint += translation;
	endPoint   += translation;
}
//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///Accessors
const float LineSegment3::CalcLength() const{
	return sqrt( CalcLengthSquared() );
}
const float LineSegment3::CalcLengthSquared() const{
	float LengthX= abs ( startPoint.x - endPoint.x );
	float LengthY = abs ( startPoint.y - endPoint.y );
	float LengthZ = abs ( startPoint.z - endPoint.z );
	return ( LengthX * LengthX + LengthY * LengthY + LengthZ * LengthZ );
}
// ===========================================================================================================
Vector3 LineSegment3::GetPointAtParametricValue( float parametricValue ){
	Vector3 displacementStoE = endPoint - startPoint;
	Vector3 displacementStoP = parametricValue * displacementStoE;

	return startPoint + displacementStoP;
}
//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///operators
const LineSegment3 LineSegment3::operator+( const Vector3& translation ) const{
	return LineSegment3(startPoint + translation, endPoint + translation );
}
const LineSegment3 LineSegment3::operator-( const Vector3& antiTranslation ) const{
	return LineSegment3(startPoint - antiTranslation, endPoint - antiTranslation );
}
const LineSegment3 LineSegment3::operator*( const float scalingValue ){
	return LineSegment3(startPoint * scalingValue, endPoint * scalingValue );
}
void LineSegment3::operator+=( const Vector3& translation ){
	startPoint += translation;
	endPoint   += translation;
}
void LineSegment3::operator-=( const Vector3& antiTranslation ){
	startPoint -= antiTranslation;
	endPoint   -= antiTranslation;
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///friend functions
const LineSegment3 Interpolate( const LineSegment3& start, const LineSegment3& end, float fractionFromStartToEnd ){
	LineSegment3 adjustedDisplacementFromStartToEnd = LineSegment3( Vector3( end.startPoint - start.startPoint ), Vector3( end.endPoint - start.endPoint));

	adjustedDisplacementFromStartToEnd.startPoint *= fractionFromStartToEnd;
	adjustedDisplacementFromStartToEnd.endPoint *= fractionFromStartToEnd;
	adjustedDisplacementFromStartToEnd.startPoint += start.startPoint;
	adjustedDisplacementFromStartToEnd.endPoint += start.endPoint;

	return adjustedDisplacementFromStartToEnd;
}