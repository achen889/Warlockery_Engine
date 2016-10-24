//==============================================================================================================
//Vector2.cpp
//by Albert Chen Jan-13-2015.
//==============================================================================================================
//includes
#define STATIC
#include <math.h>
#include "Engine\Math\Vector2.hpp"	

const Vector2 Vector2::ZERO = Vector2(0.0f, 0.0f);
const Vector2 Vector2::ONE = Vector2(1.0f, 1.0f);

	Vector2::Vector2(){
		//do nothing
		//register vector2 commands
		//REGISTER_CONSOLE_COMMAND(VEC2LENGTH);
	}
	//-----------------------------------------------------------------------------------------------------------
	//Calculation Methods
	///----------------------------------------------------------------------------------------------------------
	///Calculate Length
	float Vector2::CalcLength() const { 
		return sqrt( CalcLengthSquared() );
	}
	///----------------------------------------------------------------------------------------------------------
	///Calculate Heading angle in Degrees
	float Vector2::CalcHeadingDegrees() const { 
		return atan2(y,x) * DegreesPerRadian; 
	}
	///----------------------------------------------------------------------------------------------------------
	///Calculate Heading angle in Radians
	float Vector2::CalcHeadingRadians() const { 
		return atan2(y,x); 
	}
	//===========================================================================================================
	//Mutators
	///----------------------------------------------------------------------------------------------------------
	///Rotate vector a specified number of degrees
	void Vector2::RotateDegrees( float degrees ){
		//setting new heading in radians because cos and sin only use radians
		//adding 90 degrees in radians to current heading
		float newHeadingDegrees = CalcHeadingDegrees() + degrees;
		float currentLength = CalcLength();

		SetLengthAndHeadingDegrees(currentLength, newHeadingDegrees);
	}
	///----------------------------------------------------------------------------------------------------------
	///Rotate Radians
	void Vector2::RotateRadians( float radians ){
		//setting new heading in radians because cos and sin only use radians
		//adding 90 degrees in radians to current heading
		float newHeadingRadians = CalcHeadingRadians() + ( radians );
		float currentLength = CalcLength();
		x = currentLength * cos( newHeadingRadians );
		y = currentLength * sin( newHeadingRadians );
	}
	//-----------------------------------------------------------------------------------------------------------
	//Normalize Methods
	///----------------------------------------------------------------------------------------------------------
	///Normalize converts Vector2 to a Length of 1.
	float Vector2::Normalize(){
		float LengthSquared = x*x + y*y;
		if(LengthSquared != 0){
			float length = sqrt(LengthSquared);
			float invLength = 1.f / length;
			x *= invLength;
			y *= invLength;
			return length;
		}else{
			return 0;
		}//end of if/else
	}
	//-----------------------------------------------------------------------------------------------------------
	//Scaling Methods
	///----------------------------------------------------------------------------------------------------------
	///inverse scales the vector passing in the divisors
	void Vector2::InverseScaleNonUniform(const Vector2& perAxisDivisors){
		//check if x divisor is 0
		if(perAxisDivisors.x != 0.0f){
			x = x / perAxisDivisors.x;
		}
		//check if y divisor is 0
		if(perAxisDivisors.y != 0.0f){
			y = y / perAxisDivisors.y;
		}	
	}
	///----------------------------------------------------------------------------------------------------------
	///Set Unit Length and Header Methods
	void Vector2::SetUnitLengthAndHeadingDegrees(float HeadingDegrees){
		x = cos( HeadingDegrees * RadiansPerDegree );
		y = sin( HeadingDegrees * RadiansPerDegree );
	}

	void Vector2::SetUnitLengthAndHeadingRadians(float HeadingRadians){
		x = cos( HeadingRadians );
		y = sin( HeadingRadians );
	}

	void Vector2::SetLengthAndHeadingDegrees(float newLength, float HeadingDegrees){
		x = newLength * cos( HeadingDegrees * RadiansPerDegree);
		y = newLength * sin( HeadingDegrees * RadiansPerDegree );
	}

	void Vector2::SetLengthAndHeadingRadians(float newLength, float HeadingRadians){
		x = newLength * cos( HeadingRadians );
		y = newLength * sin( HeadingRadians );
	}

//===========================================================================================================
//Friend Method Implementation

///calculate distance between two positions A and B
float CalcDistance( const Vector2& positionA, const Vector2& positionB ){ 
	return sqrt( CalcDistanceSquared(positionA, positionB) ); 
}

///----------------------------------------------------------------------------------------------------------
///square of the distance between two vectors
float CalcDistanceSquared( const Vector2& positionA, const Vector2& positionB ){
	float LengthABX = abs ( (positionB.x - positionA.x) );
	float LengthABY = abs ( (positionB.y - positionA.y) );

	return ( LengthABX * LengthABX + LengthABY * LengthABY );
}

//-----------------------------------------------------------------------------------------------------------

const Vector2 Interpolate( const Vector2& start, const Vector2& end, float fractionFromStartToEnd ){
	Vector2 rangeFromSToE = end - start;

	return start + ( rangeFromSToE * fractionFromStartToEnd);
}

///----------------------------------------------------------------------------------------------------------
///assumes the format "x,y", will break otherwise
Vector2 GetVec2FromString(const std::string& vec2Str){
	Strings vec2Strs = SplitString(vec2Str, ",");

	Vector2 outVec2;
	outVec2.x = (float)StringToInt(vec2Strs[0]);
	outVec2.y = (float)StringToInt(vec2Strs[1]);

	return outVec2;
}

//-----------------------------------------------------------------------------------------------------------

float SetOrientationTowards(const Vector2& myPosition, const Vector2& targetPosition){
	Vector2 displacementTowardsPosition = targetPosition - myPosition;
	float headingDegreesTowardsPosition = displacementTowardsPosition.CalcHeadingDegrees();

	return headingDegreesTowardsPosition;

}

Vector2 GetRandomVector2InRange(const float& minRange, const float& maxRange) {
	return Vector2(GetRandomFloatInRange(minRange, maxRange), GetRandomFloatInRange(minRange, maxRange));
}

//===========================================================================================================


