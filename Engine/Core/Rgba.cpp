//==============================================================================================================
//Rgba.cpp
//by Albert Chen Jan-30-2015.
//==============================================================================================================
#define STATIC
#include "Engine/Core/Rgba.hpp"
#include "Engine/Math/Vector4.hpp"
#include "Utilities.hpp"

//===========================================================================================================

const Rgba Rgba::BLACK(0, 0, 0 );
const Rgba Rgba::WHITE (255, 255, 255 );
const Rgba Rgba::GRAY (127, 127, 127 );
const Rgba Rgba::RED   (255, 0, 0 );
const Rgba Rgba::GREEN (0, 255 , 0 );
const Rgba Rgba::BLUE  (0, 0 , 255 );
const Rgba Rgba::YELLOW  (255, 255 , 0 );
const Rgba Rgba::PURPLE (155, 48, 255 );
const Rgba Rgba::GOLD (255, 215 , 0);
const Rgba Rgba::SILVER (192, 192, 192);
const Rgba Rgba::MAGENTA (255, 0 , 255);
const Rgba Rgba::TRON_BLUE (24, 202 , 230 );
const Rgba Rgba::NEON_GREEN (57, 255 , 20 );
const Rgba Rgba::ORANGE (255, 165, 0);
const Rgba Rgba::MINT_GREEN(0, 255, 127);
const Rgba Rgba::LAVENDER(230, 230, 250);
const Rgba Rgba::VIOLET(127, 0, 255);
const Rgba Rgba::INDIGO(75, 0, 130);

//===========================================================================================================

Rgba::Rgba(const std::string& colorString){
	UNUSED(colorString);
	//try to get the color data from the colorScheme
	
}

///----------------------------------------------------------------------------------------------------------
///return as vec4 for mesh rendering

Vector4 Rgba::ToVector4() const {
	return Vector4(fR(), fG(), fB(), fA());
}

//-----------------------------------------------------------------------------------------------------------

Vector4 ToVector4(const Rgba& colorToConvert){
	return colorToConvert.ToVector4();
}

///----------------------------------------------------------------------------------------------------------
///friend methods

Rgba StringToRgba(const std::string& s) {
	Rgba color;

	std::vector<std::string> colorComponents = SplitString(s, ",");

	color.r = (unsigned char)CStringToInt(colorComponents[0].c_str());
	color.g = (unsigned char)CStringToInt(colorComponents[1].c_str());
	color.b = (unsigned char)CStringToInt(colorComponents[2].c_str());
	color.a = 255;//CStringToInt(colorComponents[3].c_str());

	return color;
}

//-----------------------------------------------------------------------------------------------------------

const Rgba Interpolate(const Rgba& start, const Rgba& end, float fractionFromStartToEnd) {

	float lerpedR = Interpolate(start.fR(), end.fR(), fractionFromStartToEnd);
	float lerpedG = Interpolate(start.fG(), end.fG(), fractionFromStartToEnd);
	float lerpedB = Interpolate(start.fB(), end.fB(), fractionFromStartToEnd);
	float lerpedA = Interpolate(start.fA(), end.fA(), fractionFromStartToEnd);

	unsigned char lerpedCR = Rgba::cR(lerpedR);
	unsigned char lerpedCG = Rgba::cG(lerpedG);
	unsigned char lerpedCB = Rgba::cB(lerpedB);
	unsigned char lerpedCA = Rgba::cA(lerpedA);

	return Rgba(lerpedCR, lerpedCG, lerpedCB, lerpedCA);
}

//===========================================================================================================