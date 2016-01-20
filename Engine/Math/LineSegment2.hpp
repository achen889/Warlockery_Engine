//==============================================================================================================
//LineSegment2.hpp
//by Albert Chen Jan-25-2015.
//==============================================================================================================

#ifndef _included_LineSegment2__
#define _included_LineSegment2__

#include "EngineMath.hpp"

class LineSegment2{
public:
	Vector2 startPoint;
	Vector2 endPoint;

	//constructors
	LineSegment2();
	LineSegment2(const Vector2& initialStart ,const Vector2& initialEnd );
	~LineSegment2();
	//mutators
	void Translate(Vector2 translation );

	//accessors
	const float CalcLength() const;
	const float CalcLengthSquared() const;
	const float CalcHeadingDegrees();
	const float CalcHeadingRadians();
	Vector2 GetPointAtParametricValue( float parametricValue );
	//operators
	const LineSegment2 operator+( const Vector2& translation ) const;
	const LineSegment2 operator-( const Vector2& antiTranslation ) const;
	const LineSegment2 operator*( const float scale );
	void operator+=( const Vector2& translation );
	void operator-=( const Vector2& antiTranslation );
	//friend functions
	friend const LineSegment2 Interpolate( const LineSegment2& start, const LineSegment2& end, float fractionFromStartToEnd );
};
//===========================================================================================================

#endif