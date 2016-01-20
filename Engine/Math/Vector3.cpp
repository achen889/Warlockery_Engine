//==============================================================================================================
//Vector3.cpp
//by Albert Chen Feb-18-2015.
//==============================================================================================================

//includes
#define STATIC
#include <math.h>
#include "Engine\Math\Vector3.hpp"	

STATIC const Vector3 Vector3::ZERO = Vector3(0.0f, 0.0f, 0.0f );

STATIC const Vector3 Vector3::UP = Vector3(0.0f, 0.0f, 1.0f );
STATIC const Vector3 Vector3::DOWN = Vector3(0.0f, 0.0f, -1.0f);

STATIC const Vector3 Vector3::RIGHT = Vector3(0.0f, 1.0f, 0.0f);
STATIC const Vector3 Vector3::LEFT = Vector3(0.0f, -1.0f, 0.0f);

STATIC const Vector3 Vector3::FORWARD = Vector3(1.0f, 0.0f, 0.0f );
STATIC const Vector3 Vector3::BACKWARD = Vector3(-1.0f, 0.0f, 0.0f);

STATIC const Vector3 Vector3::COMMON_WORLD_NORMAL= Vector3(-1.0f, 0.0f, 0.0f);

	//-----------------------------------------------------------------------------------------------------------
	//Normalize Methods
	///----------------------------------------------------------------------------------------------------------
	///Normalize converts Vector2 to a Length of 1.
	float Vector3::Normalize(){
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
	void Vector3::InverseScaleNonUniform(const Vector3& perAxisDivisors){
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
float CalcDistance( const Vector3& positionA, const Vector3& positionB ){ return sqrt( CalcDistanceSquared(positionA, positionB) ); }
///----------------------------------------------------------------------------------------------------------
///square of the distance between two vectors
float CalcDistanceSquared( const Vector3& positionA, const Vector3& positionB ){
	float LengthABX = abs ( (positionB.x - positionA.x) );
	float LengthABY = abs ( (positionB.y - positionA.y) );
	float LengthABZ = abs ( (positionB.z - positionA.z) );
	return ( LengthABX * LengthABX + LengthABY * LengthABY + LengthABZ * LengthABZ );
}

const Vector3 Interpolate( const Vector3& startVec3, const Vector3& endVec3, float fractionFromStartToEnd ){
	Vector3 rangeFromSToE = endVec3 - startVec3;

	return startVec3 + ( rangeFromSToE * fractionFromStartToEnd);
}

Vector3 CrossProduct(const Vector3& vectorU, const Vector3& vectorV ){
	Vector3 vectorS;
	vectorS.x = vectorU.y * vectorV.z - vectorU.z * vectorV.y;
	vectorS.y = vectorU.z * vectorV.x - vectorU.x * vectorV.z;
	vectorS.z = vectorU.x * vectorV.y - vectorU.y * vectorV.x;

	return vectorS;
}

Vector3 GetRandomVector3InRange(const float& minRange, const float& maxRange){
	return Vector3(GetRandomFloatInRange(minRange, maxRange), GetRandomFloatInRange(minRange, maxRange), GetRandomFloatInRange(minRange, maxRange));
}

