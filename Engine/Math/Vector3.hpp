//==============================================================================================================
//Vector3.hpp
//by Albert Chen Feb-18-2015.
//==============================================================================================================

#pragma once

#ifndef _included_Vector3__
#define _included_Vector3__

//#include "MathUtils.hpp"
#include "Vector2.hpp"
#include <vector>


#define RANDOM_VEC3(minVal , maxVal) GetRandomVector3InRange((float) minVal, (float) maxVal )
#define RANDOM_UNIT_VEC3 GetRandomVector3InRange(0.0f, 1.0f )

class Vector3{
public:
	float x;
	float y;
	float z;

	static const Vector3 ZERO;

	static const Vector3 UP;
	static const Vector3 DOWN;

	static const Vector3 FORWARD;
	static const Vector3 BACKWARD;

	static const Vector3 RIGHT;
	static const Vector3 LEFT;

	static const Vector3 COMMON_WORLD_NORMAL;
	//constructors
	Vector3();
	~Vector3();
	Vector3(const Vector3& copy);
	Vector3(float initialAll) :
		x(initialAll),
		y(initialAll),
		z(initialAll){

	}
	explicit Vector3(float initialX, float initialY, float initialZ);
	//accessors
	void GetXYZ(float& out_x, float& out_y, float& out_z );
	const float* GetAsFloatArray() const;
	float* GetAsFloatArray();
	float CalcLength() const ;
	float CalcLengthSquared() const ;
	std::string ToString();
	//mutators
	void SetXYZ( float newX, float newY, float newZ);
	//operators
	//comparison operators
	bool operator==(const Vector3& vectorToEqual) const;
	bool operator!=(const Vector3& vectorToNotEqual) const;
	//assignment operator
	const Vector3& operator=(const Vector3& vectorToAssign);
	//addition operators
	const Vector3 operator+(const Vector3& vectorToAdd) const;
	void operator+=(const Vector3& vectorToAdd);
	//subtraction operators
	const Vector3 operator-(const Vector3& vectorToSubtract) const;
	void operator-=(const Vector3& vectorToSubtract);
	//multiply scale operators
	const Vector3 operator*(const float& scale) const;
	void operator*=(const float scale);
	//multiply per axisScale operators
	const Vector3 operator*(const Vector3& perAxisScaleFactors) const;
	void operator*=(const Vector3& perAxisScaleFactors);
	//inverse scale operator
	const Vector3 operator/(const float inverseScale) const ;
	//Normalize methods
	float Normalize();
	float SetLength( float newLength );
	//additional scaling methods
	void ScaleUniform(float scale );
	void ScaleNonUniform(const Vector3& perAxisScaleFactors );
	void InverseScaleNonUniform(const Vector3& perAxisDivisors);
	//friend methods
	friend float CalcDistance( const Vector3& positionA, const Vector3& positionB );
	friend float CalcDistanceSquared( const Vector3& positionA, const Vector3& positionB );
	friend const Vector3 operator*(float scale, const Vector3& vectorToScale);
	friend float DotProduct( const Vector3& a, const Vector3& b);
	friend Vector3 CrossProduct(const Vector3& vectorU, const Vector3& vectorV );
	friend const Vector3 Interpolate( const Vector3& start, const Vector3& end, float fractionFromStartToEnd );
	friend Vector3 Reflect(const Vector3& incomingVector, const Vector3& planeNormal );
	friend void ConsolePrintVector3(const Vector3& consoleVector3 );
	friend Vector3 GetRandomVector3InRange(const float& minRange, const float& maxRange);
};
//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///Default Constructor
///----------------------------------------------------------------------------------------------------------
inline Vector3::Vector3(){ 
	//do nothing for speed
}
inline Vector3::~Vector3(){ 
	//do nothing for speed
}
inline Vector3::Vector3(const Vector3& copy){
	x = copy.x;
	y = copy.y;
	z = copy.z;
}
///----------------------------------------------------------------------------------------------------------
///Explicit Constructor
inline Vector3::Vector3(float initialX, float initialY, float initialZ){
	x = initialX;
	y = initialY;
	z = initialZ;
}
//===========================================================================================================
//Accessors
///----------------------------------------------------------------------------------------------------------
///GetXYZ
inline void Vector3::GetXYZ(float& out_x, float& out_y, float& out_z ){
	out_x = x;
	out_y = y;
	out_z = z;
}
///----------------------------------------------------------------------------------------------------------
///get vector3 as const float array
inline const float* Vector3::GetAsFloatArray() const{
	return &x;
}
///----------------------------------------------------------------------------------------------------------
///get vector3 as float array
inline float* Vector3::GetAsFloatArray(){
	return &x;
}
///----------------------------------------------------------------------------------------------------------
///Faster Calculate Length Squared
inline float Vector3::CalcLengthSquared() const { 
	return x*x + y*y + z*z; 
}
///Calculate Length
inline float Vector3::CalcLength() const { 
	return sqrt( CalcLengthSquared() );
}
inline std::string Vector3::ToString(){
	std::string vectorString = "(" +FloatToString(x) +", "+ FloatToString(y) +", "+FloatToString(z) +")";
	return vectorString;
}
//==========================================================================================================
//Mutators
//-----------------------------------------------------------------------------------------------------------
///sets x and y
inline void Vector3::SetXYZ( float newX, float newY, float newZ ){
	x = newX;
	y = newY;
	z = newZ;
}
//===========================================================================================================
//Operators for Vec2
///----------------------------------------------------------------------------------------------------------
///Comparison operators
///----------------------------------------------------------------------------------------------------------
///Equals Operator
inline bool Vector3::operator==(const Vector3& vectorToEqual) const {
	if(x == vectorToEqual.x && y == vectorToEqual.y && z == vectorToEqual.z ){
		return true;
	}else{
		return false;
	}//end of if/else
}
///----------------------------------------------------------------------------------------------------------
///Not Equals Operator
inline bool Vector3::operator!=(const Vector3& vectorToNotEqual) const {
	if(x != vectorToNotEqual.x && y != vectorToNotEqual.y && z != vectorToNotEqual.z ){
		return true;
	}else{
		return false;
	}//end of if/else
}
///----------------------------------------------------------------------------------------------------------
///Assignment Operator
inline const Vector3& Vector3::operator=(const Vector3& vectorToAssign){
	x = vectorToAssign.x;
	y = vectorToAssign.y;
	z = vectorToAssign.z;
	return *this;
}
///addition operator 
inline const Vector3 Vector3::operator+(const Vector3& vectorToAdd) const { 
	return Vector3( x + vectorToAdd.x, y + vectorToAdd.y, z + vectorToAdd.z );
}

inline void Vector3::operator+=(const Vector3& vectorToAdd){ 
	x += vectorToAdd.x; 
	y += vectorToAdd.y; 
	z += vectorToAdd.z;
}
///----------------------------------------------------------------------------------------------------------
///subtraction operator
inline const Vector3 Vector3::operator-(const Vector3& vectorToSubtract) const { 
	return Vector3( x - vectorToSubtract.x, y - vectorToSubtract.y, z - vectorToSubtract.z );
}
inline void Vector3::operator-=(const Vector3& vectorToSubtract){ 
	x -= vectorToSubtract.x; 
	y -= vectorToSubtract.y;
	z -= vectorToSubtract.z;
}
///----------------------------------------------------------------------------------------------------------
///multiply operator with scalar val
inline const Vector3 Vector3::operator*(const float& scale) const { 
	return Vector3( x * scale, y * scale, z * scale );
}
inline void Vector3::operator*=(const float scale){ 
	x *= scale; 
	y *= scale; 
	z *= scale;
}
///----------------------------------------------------------------------------------------------------------
///multiply operator with two scale factors
inline const Vector3 Vector3::operator*(const Vector3& perAxisScaleFactors) const { 
	return Vector3( x*perAxisScaleFactors.x , y*perAxisScaleFactors.y, z*perAxisScaleFactors.z );
}
inline void Vector3::operator*=(const Vector3& perAxisScaleFactors){ 
	x *= perAxisScaleFactors.x; 
	y *= perAxisScaleFactors.y; 
	z *= perAxisScaleFactors.z;
}
///----------------------------------------------------------------------------------------------------------
///inverse scale operator
inline const Vector3 Vector3::operator/(const float inverseScale) const {
	float ReciprocalOfScale = 1 / inverseScale;
	return Vector3(x * ReciprocalOfScale, y * ReciprocalOfScale, z * ReciprocalOfScale );
}
///----------------------------------------------------------------------------------------------------------
///Sets length of vec2 to specific length
inline float Vector3::SetLength( float newLength ){
	Normalize();
	x *=newLength;
	y *=newLength;
	z *=newLength;
	return newLength;
}
///scales the vector uniformly
inline void Vector3::ScaleUniform(float scale ){
	x *= scale;
	y *= scale;
	z *= scale;
}
///----------------------------------------------------------------------------------------------------------
///scales the vector non uniform using a second vector to pass in the scale factors
inline void Vector3::ScaleNonUniform(const Vector3& perAxisScaleFactors ){
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
	z *= perAxisScaleFactors.z;
}
//===========================================================================================================
//friend method implementation
///----------------------------------------------------------------------------------------------------------
///scale multiplied by vector operator
inline const Vector3 operator*(float scale, const Vector3& vectorToScale){ 
	return Vector3( vectorToScale.x * scale, vectorToScale.y * scale, vectorToScale.z * scale );
}
///----------------------------------------------------------------------------------------------------------
///Calculates DotProduct of two vectors a and b
inline float DotProduct( const Vector3& a, const Vector3& b){ 
	return a.x * b.x + a.y * b.y + a.z * b.z;
}
///----------------------------------------------------------------------------------------------------------
///calculates the vector reflected off of a normalized plane with specific facing
inline Vector3 Reflect(const Vector3& incomingVector, const Vector3& planeNormal ){

	float projectionOfIncomingByPlaneNormal = DotProduct( incomingVector , planeNormal );

	Vector3 projectionVectorOfIncomingByPlaneNormal = planeNormal * projectionOfIncomingByPlaneNormal;
	
	Vector3 reflectionOfIncomingVector = incomingVector - ( 2 * projectionVectorOfIncomingByPlaneNormal );

	return reflectionOfIncomingVector;
}

inline void ConsolePrintVector3(const Vector3& consoleVector3 ){
	ConsolePrintf("Vector3<%.02f, ",consoleVector3.x );
	ConsolePrintf("%.02f, ",consoleVector3.y );
	ConsolePrintf("%.02f>\n",consoleVector3.z );
}
//===========================================================================================================

typedef std::vector<Vector3>Vector3s;

#endif

