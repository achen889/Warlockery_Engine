//==============================================================================================================
//MathUtils.hpp
//by Albert Chen Jan-14-2015.
//==============================================================================================================

#ifndef _included_MathUtils__
#define _included_MathUtils__

#include "RandomUtils.hpp"

//===========================================================================================================

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


float SplitFloat(float inVal, double& outIntPart);
float Round(float inVal);
int RoundFloatToNearestInt(float inVal);

//degree and radian conversions
float ConvertRadiansToDegrees(float radians);
float ConvertDegreesToRadians(float degrees);

//range and asymptotic 0-1 methods
float AsymptoticAdd(float normalizedValueA, float normalizedValueB);
float RangeMap(const float& inValue, const float& inStart, const float& inEnd, float outStart, float outEnd);

//interpolate and easing methods
const float Interpolate(float start, float end, float fractionFromStartToEnd);
float GetLoopedParametric(float t, float maxParametric = 1.0f);

//blend functions

//logarithm helpers
float NaturalLogOf(float x);
float LogOf(float x, float base);

//lerp
template<typename T>
const T Interpolate(const T& start, const T& end, float fractionFromStartToEnd);

float SmoothStart(float normalizedValue);
float SmoothStop(float normalizedValue);
float SmoothStep(float normalizedValue);

//bit flag methods and bit magic
bool IsPowerOfTwo(unsigned int x);
//bool AreBitsSet(unsigned int& bitfield, const unsigned int& bitNum);
bool IsBitSet(unsigned int& bitfield, const unsigned char& bitNums);
void SetBit(unsigned int& bitfield,   const unsigned char& bitNums);
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
inline float SplitFloat(float inVal, double& outIntPart){
	return (float)std::modf((double)inVal, (double*)&outIntPart);
}

//-----------------------------------------------------------------------------------------------------------

inline float Round(float inVal) {
	return (float)floor(inVal + 0.5);
}

inline int RoundFloatToNearestInt(float inVal){
	return (int)floor(inVal + 0.5);
}

inline float Floor(float inVal) {
	return (float)floor(inVal);
}

//-----------------------------------------------------------------------------------------------------------

//for when the parametric value is less than or greater than 1.0
inline float GetLoopedParametric(float t, float maxParametric) {
	
	while (t > maxParametric) {
		t -= maxParametric;
	}

	while (t < 0.0f) {
		t += 1.0f;
	}

	return t;
}


//-----------------------------------------------------------------------------------------------------------

template<typename T>
inline const T Interpolate(const T& start, const T& end, float fractionFromStartToEnd) {
	T lerpedValue = Interpolate(start, end, fractionFromStartToEnd);

	return lerpedValue;
}

//-----------------------------------------------------------------------------------------------------------

//get natural base-e logarithm of x
inline float NaturalLogOf(float x) {
	return log(x);
}

//-----------------------------------------------------------------------------------------------------------

inline float LogOf(float x, float base) {
	return NaturalLogOf(x) / NaturalLogOf(base);
}

//===========================================================================================================

#endif