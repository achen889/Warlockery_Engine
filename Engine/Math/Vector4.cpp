//==============================================================================================================
//Vector4.cpp
//by Albert Chen Feb-27-2015.
//==============================================================================================================


//includes
#include <math.h>

#include "Engine\Math\Vector4.hpp"	
	
//-----------------------------------------------------------------------------------------------------------
//Normalize Methods
///----------------------------------------------------------------------------------------------------------
///Normalize converts Vector2 to a Length of 1.
float Vector4::Normalize(){
	float LengthSquared = x*x + y*y + z*z;
	if(LengthSquared != 0){
		float length = sqrt(LengthSquared);
		float invLength = 1.f / length;
		x *= invLength;
		y *= invLength;
		z *= invLength;
		return length;
	}else{
		return 0;
	}//end of if/else
}
//-----------------------------------------------------------------------------------------------------------
//Scaling Methods
///----------------------------------------------------------------------------------------------------------
///inverse scales the vector passing in the divisors
void Vector4::InverseScaleNonUniform(const Vector4& perAxisDivisors){
	//check if x divisor is 0
	if(perAxisDivisors.x != 0.0f){
		x = x / perAxisDivisors.x;
	}
	//check if y divisor is 0
	if(perAxisDivisors.y != 0.0f){
		y = y / perAxisDivisors.y;
	}
	//check if y divisor is 0
	if(perAxisDivisors.z != 0.0f){
		z = z / perAxisDivisors.z;
	}
}

//===========================================================================================================
//Friend Method Implementation
///calculate distance between two positions A and B
float CalcDistance( const Vector4& positionA, const Vector4& positionB ){ return sqrt( CalcDistanceSquared(positionA, positionB) ); }
///----------------------------------------------------------------------------------------------------------
///square of the distance between two vectors
float CalcDistanceSquared( const Vector4& positionA, const Vector4& positionB ){
	float LengthABX = abs ( (positionB.x - positionA.x) );
	float LengthABY = abs ( (positionB.y - positionA.y) );
	float LengthABZ = abs ( (positionB.z - positionA.z) );
	float LengthABW = abs ( (positionB.w - positionA.w) );

	return ( LengthABX * LengthABX + LengthABY * LengthABY + LengthABZ * LengthABZ + LengthABW * LengthABW);
}

const Vector4 Interpolate( const Vector4& startVec3, const Vector4& endVec3, float fractionFromStartToEnd ){
	Vector4 rangeFromSToE = endVec3 - startVec3;

	return startVec3 + ( rangeFromSToE * fractionFromStartToEnd);
}

