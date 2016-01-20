//==============================================================================================================
//IntVec2.hpp
//by Albert Chen Feb-11-2015.
//==============================================================================================================

#pragma once
#include <vector>

#ifndef _included_IntVec2__
#define _included_IntVec2__

class IntVec2{
public:
	int x;
	int y;

	IntVec2();
	IntVec2(const IntVec2& copy);
	IntVec2(int initialX, int initialY );
	~IntVec2();

	static const IntVec2 ZERO;
	static const IntVec2 ONE;

	bool operator==(const IntVec2& vectorToEqual) const;
	bool operator!=(const IntVec2& vectorToNotEqual) const;
	bool operator<(const IntVec2& vectorToLessThan) const;
	//assignment operator
	const IntVec2& operator=(const IntVec2& vectorToAssign);
	//addition operators
	const IntVec2 operator+(const IntVec2& vectorToAdd) const;
	void operator+=(const IntVec2& vectorToAdd);
	//subtraction operators
	const IntVec2 operator-(const IntVec2& vectorToSubtract) const;
	void operator-=(const IntVec2& vectorToSubtract);
	//multiply scale operators
	const IntVec2 operator*(const int scale) const;
	void operator*=(const int scale);
	const IntVec2 operator*(const IntVec2& perAxisScaleFactors) const;
	void operator*=(const IntVec2& perAxisScaleFactors);
	const IntVec2 operator/(const int inverseScale) const ;

	std::string ToString();
	friend std::string ToString(const IntVec2& i);
	
	float CalcLength() const;
	int CalcLengthSquared() const;

	friend int CalcDistanceSquared(const IntVec2& positionA, const IntVec2& positionB);
	friend IntVec2 GetIntVec2FromString(const std::string& vec2iStr);
};

typedef std::vector<IntVec2> IntVec2s;

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///constructors
inline IntVec2::IntVec2(){
	//do nothing
}
inline IntVec2::~IntVec2(){
	//do nothing
}
inline IntVec2::IntVec2(const IntVec2& copy){
	x = copy.x;
	y = copy.y;
}
inline IntVec2::IntVec2(int initialX, int initialY ){
	x = initialX;
	y = initialY;
}
///----------------------------------------------------------------------------------------------------------
///Operators for Vec2
///----------------------------------------------------------------------------------------------------------
///Comparison operators
///----------------------------------------------------------------------------------------------------------
///Equals Operator
inline bool IntVec2::operator==(const IntVec2& vectorToEqual) const {
	if(x == vectorToEqual.x && y == vectorToEqual.y){
		return true;
	}else{
		return false;
	}//end of if/else
}
///----------------------------------------------------------------------------------------------------------
///Not Equals Operator
inline bool IntVec2::operator!=(const IntVec2& vectorToNotEqual) const {
	if(x != vectorToNotEqual.x || y != vectorToNotEqual.y){
		return true;
	}else{
		return false;
	}//end of if/else
}
inline bool IntVec2::operator<(const IntVec2& vectorToLessThan) const{
	if(y < vectorToLessThan.y ){
		return true;
	}
	else if(y > vectorToLessThan.y ){
		return false;
	}else{
		//y is equal so use x
		return (x < vectorToLessThan.x );
	}
}
///----------------------------------------------------------------------------------------------------------
///Assignment Operator
inline const IntVec2& IntVec2::operator=(const IntVec2& vectorToAssign){
	x = vectorToAssign.x;
	y = vectorToAssign.y;
	return *this;
}
///addition operator 
inline const IntVec2 IntVec2::operator+(const IntVec2& vectorToAdd) const { 
	return IntVec2( x + vectorToAdd.x, y + vectorToAdd.y );
}
inline void IntVec2::operator+=(const IntVec2& vectorToAdd){ 
	x += vectorToAdd.x; y += vectorToAdd.y; 
}
///----------------------------------------------------------------------------------------------------------
///subtraction operator
inline const IntVec2 IntVec2::operator-(const IntVec2& vectorToSubtract) const { 
	return IntVec2( x - vectorToSubtract.x, y - vectorToSubtract.y );
}
inline void IntVec2::operator-=(const IntVec2& vectorToSubtract){ 
	x -= vectorToSubtract.x; y -= vectorToSubtract.y;
}
///----------------------------------------------------------------------------------------------------------
///multiply operator with scalar val
inline const IntVec2 IntVec2::operator*(const int scale) const { 
	return IntVec2( x * scale, y * scale );
}
inline void IntVec2::operator*=(const int scale){ 
	x *= scale; y *= scale;
}
///----------------------------------------------------------------------------------------------------------
///multiply operator with two scale factors
inline const IntVec2 IntVec2::operator*(const IntVec2& perAxisScaleFactors) const { 
	return IntVec2( x*perAxisScaleFactors.x , y*perAxisScaleFactors.y );
}
inline void IntVec2::operator*=(const IntVec2& perAxisScaleFactors){ 
	x *= perAxisScaleFactors.x; y*= perAxisScaleFactors.y;
}
///----------------------------------------------------------------------------------------------------------
///inverse scale operator
inline const IntVec2 IntVec2::operator/(const int inverseScale) const {
	int ReciprocalOfScale = 1 / inverseScale;
	return IntVec2(x * ReciprocalOfScale, y * ReciprocalOfScale);
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///friend methods
///----------------------------------------------------------------------------------------------------------
///square of the distance between two vectors
inline int CalcDistanceSquared( const IntVec2& positionA, const IntVec2& positionB ){
	int LengthABX =  (positionB.x - positionA.x);
	int LengthABY =  (positionB.y - positionA.y);

	return ( LengthABX * LengthABX + LengthABY * LengthABY );
}



//===========================================================================================================

#endif