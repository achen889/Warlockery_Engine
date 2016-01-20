//==============================================================================================================
//MathUtils.cpp
//by Albert Chen Jan-14-2015.
//==============================================================================================================
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <vector>
#include "MathUtils.hpp"

//#include "Engine\Console\DevConsole.hpp"

const float InverseRandMax = ( 1.0f / RAND_MAX );

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///basic math
///----------------------------------------------------------------------------------------------------------
///Calculate shortest angular displacement
float CalcShortestAngularDisplacement( const float& startAngle, const float& destinationAngle ){

	float angularDisplacement = destinationAngle - startAngle;
	
	while(angularDisplacement > 180.0f ){
		angularDisplacement -=360.0f;
	}

	while(angularDisplacement < -180.0f ){
		angularDisplacement +=360.0f;
	}
	

	return angularDisplacement;
}
///----------------------------------------------------------------------------------------------------------
///clamp
float Clamp( float inValue, float min, float max ){
	if(min > max){
		float temp = min;
		min = max;
		max = temp;
	}
	//clamp the value
	if(inValue < min){
		return min;
	}else if(inValue > max){
		return max;
	}else{
		return inValue;
	}
}
//-----------------------------------------------------------------------------------------------------------

int ClampInt(int inValue, int min, int max){
	if (min > max){
		int temp = min;
		min = max;
		max = temp;
	}
	//clamp the value
	if (inValue < min){
		return min;
	}
	else if (inValue > max){
		return max;
	}
	else{
		return inValue;
	}
}


///----------------------------------------------------------------------------------------------------------
///determines if int is power of 2 using black magic
bool IsPowerOfTwo( unsigned int x ){	
	//if x is 0 or negative, just return
	if(x <= 0){
		return false;
	}
	//and the binary of the int with it's opposite
	//any power of 2 in binary is 1 with 0's after it, 
	//one less than that is definitely a 0 with 1's after it, 
	//by anding the values you will always get 0 if it's a power of 2

	return ( x & (x - 1) ) == 0;

	//return true;

}
//===========================================================================================================
///-------------------------------------------------------------------------------------------------------------
///Converts radians to degrees
float ConvertRadiansToDegrees(float radians ){
	return radians * DegreesPerRadian;
}

//converts degrees to radians
float ConvertDegreesToRadians(float degrees ){
	return degrees * RadiansPerDegree;
}

//===========================================================================================================
//Random Number Generators

void RandomSeed(unsigned int seedValue){ 
	srand(seedValue);
}

///----------------------------------------------------------------------------------------------------------
///Returns a random int >0 less than specified value
int GetRandomIntLessThan(int maxValNotInclusive ){
	return rand() % maxValNotInclusive;
}

///----------------------------------------------------------------------------------------------------------
///Returns a random int between the two specified values
int GetRandomIntInRange(int minValInclusive, int maxValInclusive){
	return ( rand() % (maxValInclusive + 1 - minValInclusive) ) + minValInclusive;
}

///----------------------------------------------------------------------------------------------------------
///Returns a random float between zero and one
float GetRandomFloatZeroToOne(){
	return (float) rand() * InverseRandMax;
}

///----------------------------------------------------------------------------------------------------------
///Returns a random float between the two specified values
float GetRandomFloatInRange(float minValInclusive, float maxValInclusive){
	return (float) ( ( rand() * InverseRandMax ) * (maxValInclusive - minValInclusive) ) + minValInclusive;
}

Rgba GetRandomColor(){
	return Rgba(GetRandomByte(), GetRandomByte(), GetRandomByte());
}

float GetRandomAngleDegrees(){
	return GetRandomFloatInRange(0.0f, 360.0f);
}

float GetRandomAngleRadians(){
	return GetRandomFloatInRange(0.0f, TWO_PI );
}

const unsigned char GetRandomByte(){
	return (unsigned char)GetRandomIntInRange(0, 255);
}

const char GetRandomAsciiChar(){
	return (char)GetRandomIntInRange(33, 126);
}

const char GetRandomAsciiLetter(){
	return (char)GetRandomIntInRange(65, 122);
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///range and asymptotic methods
float AsymptoticAdd( float normalizedValueA, float normalizedValueB ){
	float asymtopeOfA =  1.0f - normalizedValueA;
	float asymtopeOfB =  1.0f - normalizedValueB;

	return 1.0f - (asymtopeOfA * asymtopeOfB);
}

float RangeMap( const float& inValue, const float& inStart, const float& inEnd, float outStart, float outEnd ){

	float inputRange = inEnd - inStart;
	float outputRange = outEnd - outStart;
	float outValue = inValue / inputRange; 
	float newValue = outValue * outputRange;

	return outStart + newValue;
}

//===========================================================================================================
//Interpolate and easing functions
float Interpolate( float start, float end, float fractionFromStartToEnd ){
	float rangeStoE = end - start;
	return rangeStoE * fractionFromStartToEnd;
}

float SmoothStart( float normalizedValue ){
	return normalizedValue * normalizedValue;
}

float SmoothStop( float normalizedValue ){
	float OneMinusNormalizedValue = 1.0f - normalizedValue;
	return 1.0f - ( OneMinusNormalizedValue * OneMinusNormalizedValue);
}

float SmoothStep( float normalizedValue ){
	float normalizedValueSquared = normalizedValue * normalizedValue;
	return ( 3.0f * normalizedValueSquared ) - ( 2.0f * normalizedValueSquared * normalizedValue );
}


//===========================================================================================================


