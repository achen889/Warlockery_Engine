//==============================================================================================================
//LineSegment3.hpp
//by Albert Chen Feb-24-2015.
//==============================================================================================================

#ifndef _included_LineSegment3__
#define _included_LineSegment3__

//#include "EngineMath.hpp"
#include "Vector3.hpp"

class LineSegment3{
public:
	Vector3 startPoint;
	Vector3 endPoint;

	//constructors
	LineSegment3();
	LineSegment3(const Vector3& initialStart ,const Vector3& initialEnd );
	~LineSegment3();
	//mutators
	void Translate(Vector3 translation );

	//accessors
	const float CalcLength() const;
	const float CalcLengthSquared() const;
	Vector3 GetPointAtParametricValue( float parametricValue );
	//operators

	const LineSegment3 operator+( const Vector3& translation ) const;
	const LineSegment3 operator-( const Vector3& antiTranslation ) const;
	const LineSegment3 operator*( const float scale );
	void operator+=( const Vector3& translation );
	void operator-=( const Vector3& antiTranslation );
	//friend functions
	friend const LineSegment3 Interpolate( const LineSegment3& start, const LineSegment3& end, float fractionFromStartToEnd );
};
//===========================================================================================================

#endif