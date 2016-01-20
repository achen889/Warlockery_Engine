//==============================================================================================================
//IntVec2.cpp
//by Albert Chen Sep-8-2015.
//==============================================================================================================

#include "IntVec2.hpp"
#include <math.h>
#include "..\Core\Utilities.hpp"

const IntVec2 IntVec2::ZERO = IntVec2(0, 0);
const IntVec2 IntVec2::ONE =  IntVec2(1, 1);

//-----------------------------------------------------------------------------------------------------------

int IntVec2::CalcLengthSquared() const {
	return x*x + y*y;
}

//-----------------------------------------------------------------------------------------------------------

float IntVec2::CalcLength() const {
	return abs( sqrt((float)CalcLengthSquared()) );
}

//-----------------------------------------------------------------------------------------------------------

std::string IntVec2::ToString(){
	return "" + IntToString(x) + "," + IntToString(y) + "";
}

std::string ToString(const IntVec2& i ){
	return "" + IntToString(i.x) + "," + IntToString(i.y) + "";
}

//-----------------------------------------------------------------------------------------------------------

///----------------------------------------------------------------------------------------------------------
///assumes the format "x,y", will break otherwise
IntVec2 GetIntVec2FromString(const std::string& vec2iStr){
	Strings vec2iStrs = SplitString(vec2iStr, ",");

	IntVec2 outIntVec2;
	outIntVec2.x = StringToInt(vec2iStrs[0]);
	outIntVec2.y = StringToInt(vec2iStrs[1]);

	return outIntVec2;
}

//===========================================================================================================