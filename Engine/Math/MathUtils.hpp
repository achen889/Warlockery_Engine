//==============================================================================================================
//MathUtils.hpp
//by Albert Chen Jan-14-2015.
//==============================================================================================================

#ifndef _included_MathUtils__
#define _included_MathUtils__

#include "Engine\Core\Utilities.hpp"
#include "Engine\Core\Rgba.hpp"
#include "Engine\Core\Memory.hpp"

//===========================================================================================================

#define RANDOM_COLOR GetRandomColor()

//mathematical constant variables
const float PI = 3.14159265f;
const float TWO_PI = PI * 2.0f;
const float RadiansPerDegree = ( PI/180.0f );
const float DegreesPerRadian = ( 180.0f /PI );
static unsigned int g_bitFlags; //int is 4 bytes or 32 bits, hence 32 bit flags to work with

//generalized bitflag macros
#define BIT(x) (1 << x)
#define BIT_AND(x,y) (x & y)
#define BIT_OR(x,y) (x | y)
#define  BIT_XOR(x,y) (x ^ y)
#define BIT_NOT(x) ~(x)

//===========================================================================================================

//public math methods
//basic math
float CalcShortestAngularDisplacement(const float& startAngle, const float& destinationAngle);
float Clamp(float inValue, float min, float max);
int ClampInt(int inValue, int min, int max);

float SplitFloat(float inVal, int& outIntPart);
int RoundFloatToNearestInt(float inVal);

//degree and radian conversions
float ConvertRadiansToDegrees(float radians);
float ConvertDegreesToRadians(float degrees);

//random number generators
void RandomSeed(unsigned int seedValue);
int GetRandomIntLessThan(int maxValNotInclusive);
int GetRandomIntInRange(int minValInclusive, int maxValInclusive);
float GetRandomFloatZeroToOne();
float GetRandomFloatInRange(float minValInclusive, float maxValInclusive);
Rgba GetRandomColor();
float GetRandomAngleDegrees();
float GetRandomAngleRadians();
const unsigned char GetRandomByte();
const char GetRandomAsciiChar();
const char GetRandomAsciiLetter();

//range and asymptotic 0-1 methods
float AsymptoticAdd(float normalizedValueA, float normalizedValueB);
float RangeMap(const float& inValue, const float& inStart, const float& inEnd, float outStart, float outEnd);
//interpolate and easing methods
float Interpolate(float start, float end, float fractionFromStartToEnd);
float SmoothStart(float normalizedValue);
float SmoothStop(float normalizedValue);
float SmoothStep(float normalizedValue);
//bit flag methods and bit magic
bool IsPowerOfTwo(unsigned int x);
bool IsBitSet(unsigned int& bitfield, const unsigned char& bitNum);
void SetBit(unsigned int& bitfield,   const unsigned char& bitNum);
void ClearBit(unsigned int& bitfield, const unsigned char& bitNum);
void ToggleBit(unsigned int& bitfield,const unsigned char& bitNum);

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods

//===========================================================================================================
//bit flag methods
inline bool IsBitSet(unsigned int& bitfield, const unsigned char& bitNum ){
	return BIT_AND(bitfield , BIT(bitNum)) != 0;
}

//-----------------------------------------------------------------------------------------------------------

inline void SetBit(unsigned int& bitfield, const unsigned char& bitNum){
	bitfield = BIT_OR( bitfield , BIT(bitNum) ); //or in the num to set bit to 1
}

//-----------------------------------------------------------------------------------------------------------

inline void ClearBit(unsigned int& bitfield, const unsigned char& bitNum){
	bitfield = BIT_AND(bitfield, BIT_NOT( BIT(bitNum) ) );
	//bitfield = bitfield & ~BIT(bitNum); //and with not value to clear only that bit to 0
}

//-----------------------------------------------------------------------------------------------------------

inline void ToggleBit(unsigned int& bitfield, const unsigned char& bitNum){
	bitfield = BIT_XOR( bitfield , BIT(bitNum) ); //xor with 2 ^ bitNuM to swap bit to 0 or 1
}

//-----------------------------------------------------------------------------------------------------------
//splits float into int and dec parts, returns dec part
inline float SplitFloat(float inVal, int& outIntPart){
	return (float)std::modf((double)inVal, (double*)&outIntPart);
}

//-----------------------------------------------------------------------------------------------------------

inline int RoundFloatToNearestInt(float inVal){
	int intPart;
	float decPart = SplitFloat(inVal, intPart);
	if (decPart <= 0.5f){
		return intPart;
	}
	else{
		return intPart + 1;
	}
}

//===========================================================================================================

#endif