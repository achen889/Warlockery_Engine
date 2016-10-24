//==============================================================================================================
//Vector2.hpp
//by Albert Chen Jan-14-2015.
//==============================================================================================================
#pragma once

#ifndef _included_Vector2__
#define _included_Vector2__

#include "MathUtils.hpp"
#include "Engine/Core/Utilities.hpp"
#include <vector>

class Vector2{
public:
	float x;
	float y;
	static const Vector2 ZERO;
	static const Vector2 ONE;
	
	//constructors
	Vector2();
	~Vector2();
	Vector2(const Vector2& copy);
	Vector2(float initialAll): 
		x(initialAll),
		y(initialAll){
		
	}
	explicit Vector2(float initialX, float initialY);
	//accessors
	void GetXY(float& out_x, float& out_y );
	const float* GetAsFloatArray() const;
	float* GetAsFloatArray();
	float CalcLength() const ;
	float CalcLengthSquared() const ;
	float CalcHeadingDegrees() const;
	float CalcHeadingRadians() const;
	//operators
	//comparison operators
	bool operator==(const Vector2& vectorToEqual) const;
	bool operator!=(const Vector2& vectorToNotEqual) const;
	//assignment operator
	const Vector2& operator=(const Vector2& vectorToAssign);
	const Vector2& operator=(const float& floatToAssign);
	//addition operators
	const Vector2 operator+(const Vector2& vectorToAdd) const;
	void operator+=(const Vector2& vectorToAdd);
	//subtraction operators
	const Vector2 operator-(const Vector2& vectorToSubtract) const;
	void operator-=(const Vector2& vectorToSubtract);
	//multiply scale operators
	const Vector2 operator*(const float scale) const;
	void operator*=(const float scale);
	const Vector2 operator*(const Vector2& perAxisScaleFactors) const;
	void operator*=(const Vector2& perAxisScaleFactors);
	const Vector2 operator/(const float inverseScale) const ;
	//mutators
	void SetXY( float newX, float newY );
	//Rotate methods
	void Rotate90Degrees();
	void RotateNeg90Degrees();
	inline void SetHeadingDegrees(float headingDegrees);

	void RotateDegrees(float degrees);
	void RotateRadians( float radians );
	//Normalize methods
	float Normalize();
	float SetLength( float newLength );
	//additional scaling methods
	void ScaleUniform(float scale );
	void ScaleNonUniform(const Vector2& perAxisScaleFactors );
	void InverseScaleNonUniform(const Vector2& perAxisDivisors);
	//Length/Heading Set methods
	void SetUnitLengthAndHeadingDegrees(float headingDegrees);
	void SetUnitLengthAndHeadingRadians(float headingRadians);
	void SetLengthAndHeadingDegrees(float newLength, float headingDegrees);
	void SetLengthAndHeadingRadians(float newLength, float headingRadians);


	//utility methods
	std::string ToString();
	friend std::string ToString(const Vector2& i);
	//inline Vector2 ToVector2(const std::string inVec2Str);

	//friend methods
	friend float CalcDistance( const Vector2& positionA, const Vector2& positionB );
	friend float CalcDistanceSquared( const Vector2& positionA, const Vector2& positionB );
	friend const Vector2 operator*(float scale, const Vector2& vectorToScale);
	friend float DotProduct( const Vector2& a, const Vector2& b);
	friend const Vector2 Interpolate( const Vector2& start, const Vector2& end, float fractionFromStartToEnd );
	//Vector2 GetVec2FromString(const std::string& vec2Str);
	friend float SetOrientationTowards(const Vector2& myPosition, const Vector2& targetPosition);
	friend Vector2 GetRandomVector2InRange(const float& minRange, const float& maxRange);
	friend Vector2 Reflect(const Vector2& incomingVector, const Vector2& planeNormal);
	friend void ConsolePrintVector2(const Vector2& consoleVector2 );
	
};

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///Default Constructor
///----------------------------------------------------------------------------------------------------------

inline Vector2::~Vector2(){ 
	//do nothing for speed
}

inline Vector2::Vector2(const Vector2& copy):
	x(copy.x),
	y(copy.y){
	
}
///----------------------------------------------------------------------------------------------------------
///Explicit Constructor
inline Vector2::Vector2(float initialX, float initialY): 
	x(initialX),
	y(initialY){
	
}
//-----------------------------------------------------------------------------------------------------------

///----------------------------------------------------------------------------------------------------------
///utility methods

inline std::string Vector2::ToString(){
	return "" + FloatToString(x) + ", " + FloatToString(y) + "";
}

inline std::string ToString(const Vector2& i){
	return "" + FloatToString(i.x) + ", " + FloatToString(i.y) + "";
}

//splits on , to get vector2
inline Vector2 ToVector2(const std::string inVec2Str){
	Strings vec2Tokens = SplitString(inVec2Str, ",");

	float vec2X = StringToFloat(vec2Tokens[0]);
	float vec2Y = StringToFloat(vec2Tokens[1]);

	return Vector2(vec2X, vec2Y);
}

//-----------------------------------------------------------------------------------------------------------

//===========================================================================================================
//Accessors
///----------------------------------------------------------------------------------------------------------
///GetXY
inline void Vector2::GetXY(float& out_x, float& out_y ){
	out_x = x;
	out_y = y;
}
///----------------------------------------------------------------------------------------------------------
///get vector2 as const float array
inline const float* Vector2::GetAsFloatArray() const{
	return &x;
}
///----------------------------------------------------------------------------------------------------------
///get vector2 as float array
inline float* Vector2::GetAsFloatArray(){
	return &x;
}
///----------------------------------------------------------------------------------------------------------
///Faster Calculate Length Squared
inline float Vector2::CalcLengthSquared() const { 
	return x*x + y*y; 
}
//===========================================================================================================
//Operators for Vec2
///----------------------------------------------------------------------------------------------------------
///Comparison operators
///----------------------------------------------------------------------------------------------------------
///Equals Operator
inline bool Vector2::operator==(const Vector2& vectorToEqual) const {
	if(x == vectorToEqual.x && y == vectorToEqual.y){
		return true;
	}else{
		return false;
	}//end of if/else
}
///----------------------------------------------------------------------------------------------------------
///Not Equals Operator
inline bool Vector2::operator!=(const Vector2& vectorToNotEqual) const {
	if(x != vectorToNotEqual.x || y != vectorToNotEqual.y){
		return true;
	}else{
		return false;
	}//end of if/else
}
///----------------------------------------------------------------------------------------------------------
///Assignment Operator
inline const Vector2& Vector2::operator=(const Vector2& vectorToAssign){
	x = vectorToAssign.x;
	y = vectorToAssign.y;
	return *this;
}
inline const Vector2& Vector2::operator=(const float& floatToAssign) {
	x = floatToAssign;
	y = floatToAssign;
	return *this;
}
///addition operator 
inline const Vector2 Vector2::operator+(const Vector2& vectorToAdd) const { 
	return Vector2( x + vectorToAdd.x, y + vectorToAdd.y );
}
inline void Vector2::operator+=(const Vector2& vectorToAdd){ 
	x += vectorToAdd.x; y += vectorToAdd.y; 
}
///----------------------------------------------------------------------------------------------------------
///subtraction operator
inline const Vector2 Vector2::operator-(const Vector2& vectorToSubtract) const { 
	return Vector2( x - vectorToSubtract.x, y - vectorToSubtract.y );
}
inline void Vector2::operator-=(const Vector2& vectorToSubtract){ 
	x -= vectorToSubtract.x; y -= vectorToSubtract.y;
}
///----------------------------------------------------------------------------------------------------------
///multiply operator with scalar val
inline const Vector2 Vector2::operator*(const float scale) const { 
	return Vector2( x * scale, y * scale );
}
inline void Vector2::operator*=(const float scale){ 
	x *= scale; y *= scale;
}
///----------------------------------------------------------------------------------------------------------
///multiply operator with two scale factors
inline const Vector2 Vector2::operator*(const Vector2& perAxisScaleFactors) const { 
	return Vector2( x*perAxisScaleFactors.x , y*perAxisScaleFactors.y );
}
inline void Vector2::operator*=(const Vector2& perAxisScaleFactors){ 
	x *= perAxisScaleFactors.x; y*= perAxisScaleFactors.y;
}
///----------------------------------------------------------------------------------------------------------
///inverse scale operator
inline const Vector2 Vector2::operator/(const float inverseScale) const {
	float ReciprocalOfScale = 1 / inverseScale;
	return Vector2(x * ReciprocalOfScale, y * ReciprocalOfScale);
}
//==========================================================================================================
//Mutators
//-----------------------------------------------------------------------------------------------------------
///sets x and y
inline void Vector2::SetXY( float newX, float newY ){
	x = newX;
	y = newY;
}
///Rotate +90
inline void Vector2::Rotate90Degrees(){
	float px = -y;
	y = x;
	x = px;
}
///----------------------------------------------------------------------------------------------------------
///Rotate -90
inline void Vector2::RotateNeg90Degrees(){
	float px = y;
	y = -x;
	x = px;
}

///----------------------------------------------------------------------------------------------------------
///set heading based on current length and new rotation
inline void Vector2::SetHeadingDegrees(float headingDegrees) {
	x = CalcLength() * cos(headingDegrees * RadiansPerDegree);
	y = CalcLength() * sin(headingDegrees * RadiansPerDegree);
}

///----------------------------------------------------------------------------------------------------------
///Sets length of vec2 to specific length
inline float Vector2::SetLength( float newLength ){
	Normalize();
	x *=newLength;
	y *=newLength;
	return newLength;
}
///scales the vector uniformly
inline void Vector2::ScaleUniform(float scale ){
	x *= scale;
	y *= scale;
}
///----------------------------------------------------------------------------------------------------------
///scales the vector non uniform using a second vector to pass in the scale factors
inline void Vector2::ScaleNonUniform(const Vector2& perAxisScaleFactors ){
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
}
//===========================================================================================================
//friend method implementation
///----------------------------------------------------------------------------------------------------------
///scale multiplied by vector operator
inline const Vector2 operator*(float scale, const Vector2& vectorToScale){ 
	return Vector2( vectorToScale.x * scale, vectorToScale.y * scale );
}
///----------------------------------------------------------------------------------------------------------
///Calculates DotProduct of two vectors a and b
inline float DotProduct( const Vector2& a, const Vector2& b){ 
	return a.x * b.x + a.y * b.y;
}
///----------------------------------------------------------------------------------------------------------
///calculates the vector reflected off of a normalized plane with specific facing
inline Vector2 Reflect(const Vector2& incomingVector, const Vector2& planeNormal ){

	float projectionOfIncomingByPlaneNormal = DotProduct( incomingVector , planeNormal );

	Vector2 projectionVectorOfIncomingByPlaneNormal = planeNormal * projectionOfIncomingByPlaneNormal;
	
	Vector2 reflectionOfIncomingVector = incomingVector - ( 2.0f * projectionVectorOfIncomingByPlaneNormal);
	
	return reflectionOfIncomingVector;
}

inline void ConsolePrintVector2(const Vector2& consoleVector2 ){
	ConsolePrintf("Vector2<%.02f, ",consoleVector2.x );
	ConsolePrintf("%.02f>",consoleVector2.y );
}
//===========================================================================================================

typedef std::vector<Vector2>Vector2s;

#endif

