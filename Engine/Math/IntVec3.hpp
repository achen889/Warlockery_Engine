//==============================================================================================================
//IntVec3.hpp
//by Albert Chen Mar-31-2015.
//==============================================================================================================

#pragma once
#ifndef _included_IntVec3__
#define _included_IntVec3__

class IntVec3{
public:
	int x;
	int y;
	int z;

	IntVec3();
	~IntVec3();
	IntVec3(const IntVec3& copy);
	explicit IntVec3(int initialX, int initialY, int initialZ);

	bool operator==(const IntVec3& vectorToEqual) const;
	bool operator!=(const IntVec3& vectorToNotEqual) const;
	//assignment operator
	const IntVec3& operator=(const IntVec3& vectorToAssign);
	//addition operators
	const IntVec3 operator+(const IntVec3& vectorToAdd) const;
	void operator+=(const IntVec3& vectorToAdd);
	//subtraction operators
	const IntVec3 operator-(const IntVec3& vectorToSubtract) const;
	void operator-=(const IntVec3& vectorToSubtract);
	//multiply scale operators
	const IntVec3 operator*(const int scale) const;
	void operator*=(const int scale);
	const IntVec3 operator*(const IntVec3& perAxisScaleFactors) const;
	void operator*=(const IntVec3& perAxisScaleFactors);
	const IntVec3 operator/(const int inverseScale) const ;
};
///----------------------------------------------------------------------------------------------------------
///inline methods
inline IntVec3::IntVec3(){
	//do nothing
}
inline IntVec3::~IntVec3(){
	//do nothing
}
inline IntVec3::IntVec3(const IntVec3& copy){
	x = copy.x;
	y = copy.y;
	z = copy.z;
}
inline IntVec3::IntVec3(int initialX, int initialY, int initialZ){
	x = initialX;
	y = initialY;
	z = initialZ;
}

//===========================================================================================================
//Operators for Vec3
///----------------------------------------------------------------------------------------------------------
///Comparison operators
///----------------------------------------------------------------------------------------------------------
///Equals Operator
inline bool IntVec3::operator==(const IntVec3& vectorToEqual) const {
	if(x == vectorToEqual.x && y == vectorToEqual.y){
		return true;
	}else{
		return false;
	}//end of if/else
}
///----------------------------------------------------------------------------------------------------------
///Not Equals Operator
inline bool IntVec3::operator!=(const IntVec3& vectorToNotEqual) const {
	if(x != vectorToNotEqual.x && y != vectorToNotEqual.y){
		return true;
	}else{
		return false;
	}//end of if/else
}
///----------------------------------------------------------------------------------------------------------
///Assignment Operator
inline const IntVec3& IntVec3::operator=(const IntVec3& vectorToAssign){
	x = vectorToAssign.x;
	y = vectorToAssign.y;
	z = vectorToAssign.z;
	return *this;
}
///addition operator 
inline const IntVec3 IntVec3::operator+(const IntVec3& vectorToAdd) const { 
	return IntVec3( x + vectorToAdd.x, y + vectorToAdd.y, z + vectorToAdd.z);
}
inline void IntVec3::operator+=(const IntVec3& vectorToAdd){ 
	x += vectorToAdd.x; y += vectorToAdd.y; z += vectorToAdd.z;
}
///----------------------------------------------------------------------------------------------------------
///subtraction operator
inline const IntVec3 IntVec3::operator-(const IntVec3& vectorToSubtract) const { 
	return IntVec3( x - vectorToSubtract.x, y - vectorToSubtract.y, z - vectorToSubtract.z);
}
inline void IntVec3::operator-=(const IntVec3& vectorToSubtract){ 
	x -= vectorToSubtract.x; y -= vectorToSubtract.y; z -= vectorToSubtract.z;
}
///----------------------------------------------------------------------------------------------------------
///multiply operator with scalar val
inline const IntVec3 IntVec3::operator*(const int scale) const { 
	return IntVec3( x * scale, y * scale, z * scale);
}
inline void IntVec3::operator*=(const int scale){ 
	x *= scale; y *= scale; z *= scale;
}
///----------------------------------------------------------------------------------------------------------
///multiply operator with two scale factors
inline const IntVec3 IntVec3::operator*(const IntVec3& perAxisScaleFactors) const { 
	return IntVec3( x*perAxisScaleFactors.x , y*perAxisScaleFactors.y, z*perAxisScaleFactors.z );
}
inline void IntVec3::operator*=(const IntVec3& perAxisScaleFactors){ 
	x *= perAxisScaleFactors.x; y*= perAxisScaleFactors.y; z*= perAxisScaleFactors.z;
}
///----------------------------------------------------------------------------------------------------------
///inverse scale operator
inline const IntVec3 IntVec3::operator/(const int inverseScale) const {
	int ReciprocalOfScale = 1 / inverseScale;
	return IntVec3(x * ReciprocalOfScale, y * ReciprocalOfScale, z * ReciprocalOfScale);
}
#endif