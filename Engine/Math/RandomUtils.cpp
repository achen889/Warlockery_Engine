//==============================================================================================================
//RandomUtils.cpp
//by Albert Chen May-19-2016.
//==============================================================================================================

#include "RandomUtils.hpp"

//===========================================================================================================

const float InverseRandMax = (1.0f / RAND_MAX);
unsigned int g_randomSeed;

//===========================================================================================================
//Random Number Generators

void RandomSeed(unsigned int seedValue) {
	srand(seedValue);

	g_randomSeed = seedValue;
	ConsoleLogPrintf("randomSeed: %d", seedValue);
}

///----------------------------------------------------------------------------------------------------------
///Returns a random int >0 less than specified value
int GetRandomIntLessThan(int maxValNotInclusive) {
	return rand() % maxValNotInclusive;
}

///----------------------------------------------------------------------------------------------------------
///Returns a random int between the two specified values
int GetRandomIntInRange(int minValInclusive, int maxValInclusive) {
	return (rand() % (maxValInclusive + 1 - minValInclusive)) + minValInclusive;
}

///----------------------------------------------------------------------------------------------------------
///Returns a random float between zero and one
float GetRandomFloatZeroToOne() {
	return (float)rand() * InverseRandMax;
}

///----------------------------------------------------------------------------------------------------------
///Returns a random float between the two specified values
float GetRandomFloatInRange(float minValInclusive, float maxValInclusive) {
	return (float)((rand() * InverseRandMax) * (maxValInclusive - minValInclusive)) + minValInclusive;
}

Rgba GetRandomColor() {
	return Rgba(GetRandomByte(), GetRandomByte(), GetRandomByte());
}

float GetRandomAngleDegrees() {
	return GetRandomFloatInRange(0.0f, 360.0f);
}

float GetRandomAngleRadians() {
	return GetRandomFloatInRange(0.0f, TWO_PI);
}

const unsigned char GetRandomByte() {
	return (unsigned char)GetRandomIntInRange(0, 255);
}

const unsigned char GetRandomUnsignedChar() {
	return (unsigned char)GetRandomIntInRange(0, 255);
}

const char GetRandomAsciiChar() {
	return (char)GetRandomIntInRange(33, 126);
}

const char GetRandomAsciiLetter() {
	return (char)GetRandomIntInRange(65, 122);
}


//===========================================================================================================

