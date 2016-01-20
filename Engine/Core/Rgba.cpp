//==============================================================================================================
//Rgba.cpp
//by Albert Chen Jan-30-2015.
//==============================================================================================================
#define STATIC
#include "Engine/Core/Rgba.hpp"
#include "Engine/Math/Vector4.hpp"

//===========================================================================================================

const Rgba Rgba::BLACK(0, 0, 0 );
const Rgba Rgba::WHITE (255, 255, 255 );
const Rgba Rgba::GRAY (192, 192, 192 );
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
	
}

///----------------------------------------------------------------------------------------------------------
///return as vec4 for mesh rendering

Vector4 Rgba::ToVector4(){
	return Vector4(fR(), fG(), fB(), fA());
}
