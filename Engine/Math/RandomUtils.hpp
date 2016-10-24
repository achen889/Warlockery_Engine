//==============================================================================================================
//RandomUtils.hpp
//by Albert Chen May-19-2016.
//==============================================================================================================

#pragma once

#ifndef _included_RandomUtils__
#define _included_RandomUtils__

//#include "Engine/Core/Utilities.hpp"
#include "Engine/Core/Rgba.hpp"
#include "Engine/Core/Memory.hpp"

#include "MathCommon.hpp"

//===========================================================================================================

extern unsigned int g_randomSeed;

//===========================================================================================================

//random number generators
void RandomSeed(unsigned int seedValue);
inline unsigned int GetRandomSeed() { return g_randomSeed; }
int GetRandomIntLessThan(int maxValNotInclusive);
int GetRandomIntInRange(int minValInclusive, int maxValInclusive);
float GetRandomFloatZeroToOne();
float GetRandomFloatInRange(float minValInclusive, float maxValInclusive);
Rgba GetRandomColor();
float GetRandomAngleDegrees();
float GetRandomAngleRadians();
const unsigned char GetRandomByte();
const unsigned char GetRandomUnsignedChar();
const char GetRandomAsciiChar();
const char GetRandomAsciiLetter();

//===========================================================================================================

#endif //__includedRandomUtils__

