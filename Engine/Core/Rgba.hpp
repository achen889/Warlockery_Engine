//==============================================================================================================
//Rgba.hpp
//by Albert Chen Jan-29-2015.
//==============================================================================================================

#pragma once
#include <string>

#ifndef _included_Rgba__
#define _included_Rgba__

static const float g_oneOverColorBits = 1.0f / 255.0f;

class Vector4;

class Rgba{
public:
	 Rgba();
	 Rgba(const Rgba& newColor);
	 Rgba(const Rgba& newColor, const unsigned char& newA );
	 Rgba(const unsigned char& newR, const unsigned char& newG, const unsigned char& newB, const unsigned char& newA = 255);
	 Rgba(const std::string& colorString);

	 //Rgba(float newR, float newG, float newB, float newA = 1.0f);
	~Rgba();

	const float fR();
	const float fG();
	const float fB();
	const float fA();
	
	Vector4 ToVector4();
	bool Rgba::operator==(const Rgba& rgbaToEqual) const;
	bool Rgba::operator!=(const Rgba& rgbaToEqual) const;
	
	//variables
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
	//static constant colors
	//common colors
	static const Rgba BLACK;
	static const Rgba WHITE;
	static const Rgba GRAY;
	static const Rgba RED;
	static const Rgba GREEN;
	static const Rgba BLUE;
	static const Rgba YELLOW;
	static const Rgba PURPLE;
	static const Rgba GOLD;
	static const Rgba SILVER;
	static const Rgba MAGENTA;
	static const Rgba TRON_BLUE;
	static const Rgba NEON_GREEN;
	static const Rgba ORANGE;
	static const Rgba MINT_GREEN;
	static const Rgba LAVENDER;
	static const Rgba VIOLET;
	static const Rgba INDIGO;
};
//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods

///----------------------------------------------------------------------------------------------------------
///constructors

inline Rgba::Rgba(){
	//do nothing
}

//-----------------------------------------------------------------------------------------------------------

inline Rgba::Rgba(const Rgba& newColor){
	r = newColor.r;
	g = newColor.g;
	b = newColor.b;
	a = newColor.a;
}

//-----------------------------------------------------------------------------------------------------------

inline Rgba::Rgba(const Rgba& newColor, const unsigned char& newA){
	r = newColor.r;
	g = newColor.g;
	b = newColor.b;

	a = newA;
}

//-----------------------------------------------------------------------------------------------------------

inline Rgba::Rgba(const unsigned char& newR, const unsigned char& newG, const unsigned char& newB, const unsigned char& newA){
	r = newR;
	g = newG;
	b = newB;
	a = newA;
}

// inline Rgba::Rgba(float newR, float newG,  float newB, float newA ){
// 	r = (unsigned char) newR * 255.0f;
// 	g = (unsigned char) newG * 255.0f;
// 	b = (unsigned char) newB * 255.0f;
// 	a = (unsigned char) newA * 255.0f;
// }

inline Rgba::~Rgba(){
	//do nothing
}

inline const float Rgba::fR(){
	return (float)( (float)r * g_oneOverColorBits);
}

inline const float Rgba::fG(){
	return (float)( (float)g * g_oneOverColorBits);
}

inline const float Rgba::fB(){
	return (float) b * g_oneOverColorBits;
}

inline const float Rgba::fA(){
	return (float) a * g_oneOverColorBits;
}

///----------------------------------------------------------------------------------------------------------
///Equals Operator
inline bool Rgba::operator==(const Rgba& rgbaToEqual) const {
	if (r == rgbaToEqual.r && g == rgbaToEqual.g && b == rgbaToEqual.b && a == rgbaToEqual.a){
		return true;
	}
	else{
		return false;
	}//end of if/else
}
///----------------------------------------------------------------------------------------------------------
///Not Equals Operator
inline bool Rgba::operator!=(const Rgba& rgbaToEqual) const {
	if (r != rgbaToEqual.r && g != rgbaToEqual.g && b != rgbaToEqual.b && a != rgbaToEqual.a){
		return true;
	}
	else{
		return false;
	}//end of if/else
}

#endif