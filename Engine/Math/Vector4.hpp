//==============================================================================================================
//Vector4.hpp
//by Albert Chen Feb-27-2015.
//==============================================================================================================

#pragma once

#ifndef _included_Vector4__
#define _included_Vector4__

//#include "MathUtils.hpp"
#include "Vector3.hpp"
#include <vector>

class Vector4{
public:
	float x;
	float y;
	float z;
	float w;
	//constructors
	Vector4();
	~Vector4();
	Vector4(const Vector4& copy);
	explicit Vector4(float initialX, float initialY, float initialZ, float initialW );
	//accessors
	void GetXYZW(float& out_x, float& out_y, float& out_z, float& out_w );
	const float* GetAsFloatArray() const;
	float* GetAsFloatArray();
	float CalcLength() const ;
	float CalcLengthSquared() const ;
	std::string ToString();
	//mutators
	void SetXYZW( float newX, float newY, float newZ, float newW);
	//operators
	//comparison operators
	bool operator==(const Vector4& vectorToEqual) const;
	bool operator!=(const Vector4& vectorToNotEqual) const;
	//assignment operator
	const Vector4& operator=(const Vector4& vectorToAssign);
	//addition operators
	const Vector4 operator+(const Vector4& vectorToAdd) const;
	void operator+=(const Vector4& vectorToAdd);
	//subtraction operators
	const Vector4 operator-(const Vector4& vectorToSubtract) const;
	void operator-=(const Vector4& vectorToSubtract);
	//multiply scale operators
	const Vector4 operator*(const float scale) const;
	void operator*=(const float scale);
	//multiply peraxisScale operators
	const Vector4 operator*(const Vector4& perAxisScaleFactors) const;
	void operator*=(const Vector4& perAxisScaleFactors);
	//inverse scale operator
	const Vector4 operator/(const float inverseScale) const ;
	//Normalize methods
	float Normalize();
	float SetLength( float newLength );
	//additional scaling methods
	void ScaleUniform(float scale );
	void ScaleNonUniform(const Vector4& perAxisScaleFactors );
	void InverseScaleNonUniform(const Vector4& perAxisDivisors);
	//friend methods
	friend float CalcDistance( const Vector4& positionA, const Vector4& positionB );
	friend float CalcDistanceSquared( const Vector4& positionA, const Vector4& positionB );
	friend const Vector4 operator*(float scale, const Vector4& vectorToScale);
	friend float DotProduct( const Vector4& a, const Vector4& b);
	//these two are questionable
	friend const Vector4 Interpolate( const Vector4& start, const Vector4& end, float fractionFromStartToEnd );
	friend Vector4 Reflect(const Vector4& incomingVector, const Vector4& planeNormal );	
};
//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///Default Constructor
///----------------------------------------------------------------------------------------------------------
inline Vector4::Vector4(){ 
	//do nothing for speed
}
inline Vector4::~Vector4(){ 
	//do nothing for speed
}
inline Vector4::Vector4(const Vector4& copy){
	x = copy.x;
	y = copy.y;
	z = copy.z;
	w = copy.w;
}
///----------------------------------------------------------------------------------------------------------
///Explicit Constructor
inline Vector4::Vector4(float initialX, float initialY, float initialZ, float initialW){
	x = initialX;
	y = initialY;
	z = initialZ;
	w = initialW;
}
//===========================================================================================================
//Accessors
///----------------------------------------------------------------------------------------------------------
///GetXYZ
inline void Vector4::GetXYZW(float& out_x, float& out_y, float& out_z, float& out_w ){
	out_x = x;
	out_y = y;
	out_z = z;
	out_w = w;
}

inline std::string Vector4::ToString(){
	std::string vec4String = "";
	vec4String += FloatToString(x) + ", " + FloatToString(y) + ", " + FloatToString(z) + ", " + FloatToString(w);

	return vec4String;
}

///----------------------------------------------------------------------------------------------------------
///get vector3 as const float array
inline const float* Vector4::GetAsFloatArray() const{
	return &x;
}
///----------------------------------------------------------------------------------------------------------
///get vector3 as float array
inline float* Vector4::GetAsFloatArray(){
	return &x;
}
///----------------------------------------------------------------------------------------------------------
///Faster Calculate Length Squared
inline float Vector4::CalcLengthSquared() const { 
	return x*x + y*y + z*z + w*w; 
}
///Calculate Length
inline float Vector4::CalcLength() const { 
	return sqrt( CalcLengthSquared() );
}
//==========================================================================================================
//Mutators
//-----------------------------------------------------------------------------------------------------------
///sets x and y
inline void Vector4::SetXYZW( float newX, float newY, float newZ, float newW ){
	x = newX;
	y = newY;
	z = newZ;
	w = newW;
}
//===========================================================================================================
//Operators for Vec2
///----------------------------------------------------------------------------------------------------------
///Comparison operators
///----------------------------------------------------------------------------------------------------------
///Equals Operator
inline bool Vector4::operator==(const Vector4& vectorToEqual) const {
	if(x == vectorToEqual.x && y == vectorToEqual.y && z == vectorToEqual.z && w == vectorToEqual.w ){
		return true;
	}else{
		return false;
	}//end of if/else
}
///----------------------------------------------------------------------------------------------------------
///Not Equals Operator
inline bool Vector4::operator!=(const Vector4& vectorToNotEqual) const {
	if(x != vectorToNotEqual.x && y != vectorToNotEqual.y && z != vectorToNotEqual.z && w != vectorToNotEqual.w){
		return true;
	}else{
		return false;
	}//end of if/else
}
///----------------------------------------------------------------------------------------------------------
///Assignment Operator
inline const Vector4& Vector4::operator=(const Vector4& vectorToAssign){
	x = vectorToAssign.x;
	y = vectorToAssign.y;
	z = vectorToAssign.z;
	w = vectorToAssign.w;
	return *this;
}
///addition operator 
inline const Vector4 Vector4::operator+(const Vector4& vectorToAdd) const { 
	return Vector4( x + vectorToAdd.x, y + vectorToAdd.y, z + vectorToAdd.z, w + vectorToAdd.w );
}
inline void Vector4::operator+=(const Vector4& vectorToAdd){ 
	x += vectorToAdd.x; 
	y += vectorToAdd.y; 
	z += vectorToAdd.z;
}
///----------------------------------------------------------------------------------------------------------
///subtraction operator
inline const Vector4 Vector4::operator-(const Vector4& vectorToSubtract) const { 
	return Vector4( x - vectorToSubtract.x, y - vectorToSubtract.y, z - vectorToSubtract.z, w - vectorToSubtract.w);
}
inline void Vector4::operator-=(const Vector4& vectorToSubtract){ 
	x -= vectorToSubtract.x; 
	y -= vectorToSubtract.y;
	z -= vectorToSubtract.z;
}
///----------------------------------------------------------------------------------------------------------
///multiply operator with scalar val
inline const Vector4 Vector4::operator*(const float scale) const { 
	return Vector4( x * scale, y * scale, z * scale, w * scale );
}
inline void Vector4::operator*=(const float scale){ 
	x *= scale; 
	y *= scale; 
	z *= scale;
	w *= scale;
}
///----------------------------------------------------------------------------------------------------------
///multiply operator with two scale factors
inline const Vector4 Vector4::operator*(const Vector4& perAxisScaleFactors) const { 
	return Vector4( x*perAxisScaleFactors.x , y*perAxisScaleFactors.y, z*perAxisScaleFactors.z, w*perAxisScaleFactors.w);
}
inline void Vector4::operator*=(const Vector4& perAxisScaleFactors){ 
	x *= perAxisScaleFactors.x; 
	y *= perAxisScaleFactors.y; 
	z *= perAxisScaleFactors.z;
	w*= perAxisScaleFactors.w;
}
///----------------------------------------------------------------------------------------------------------
///inverse scale operator
inline const Vector4 Vector4::operator/(const float inverseScale) const {
	float ReciprocalOfScale = 1.0f /  inverseScale;
	return Vector4(x * ReciprocalOfScale, y * ReciprocalOfScale, z * ReciprocalOfScale, w *ReciprocalOfScale );
}
///----------------------------------------------------------------------------------------------------------
///Sets length of vec2 to specific length
inline float Vector4::SetLength( float newLength ){
	Normalize();
	x *=newLength;
	y *=newLength;
	z *=newLength;
	w *=newLength;
	return newLength;
}
///scales the vector uniformly
inline void Vector4::ScaleUniform(float scale ){
	x *= scale;
	y *= scale;
	z *= scale;
	w *= scale;
}
///----------------------------------------------------------------------------------------------------------
///scales the vector non uniform using a second vector to pass in the scale factors
inline void Vector4::ScaleNonUniform(const Vector4& perAxisScaleFactors ){
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
	z *= perAxisScaleFactors.z;
	w *= perAxisScaleFactors.w;
}
//===========================================================================================================
//friend method implementation
///----------------------------------------------------------------------------------------------------------
///scale multiplied by vector operator
inline const Vector4 operator*(float scale, const Vector4& vectorToScale){ 
	return Vector4( vectorToScale.x * scale, vectorToScale.y * scale, vectorToScale.z * scale, vectorToScale.w * scale );
}
///----------------------------------------------------------------------------------------------------------
///Calculates DotProduct of two vectors a and b
inline float DotProduct( const Vector4& a, const Vector4& b){ 
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w*b.w;
}
///----------------------------------------------------------------------------------------------------------
///calculates the vector reflected off of a normalized plane with specific facing
inline Vector4 Reflect(const Vector4& incomingVector, const Vector4& planeNormal ){

	float projectionOfIncomingByPlaneNormal = DotProduct( incomingVector , planeNormal );

	Vector4 projectionVectorOfIncomingByPlaneNormal = planeNormal * projectionOfIncomingByPlaneNormal;
	
	Vector4 reflectionOfIncomingVector = incomingVector - ( 2 * projectionVectorOfIncomingByPlaneNormal );

	return reflectionOfIncomingVector;
}
//===========================================================================================================

#endif

