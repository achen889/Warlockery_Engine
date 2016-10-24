//==============================================================================================================
//Rgba.hpp
//by Albert Chen Jan-29-2015.
//==============================================================================================================

#pragma once
#include <string>
#include "..\Console\Console.hpp"
#include <map>
//#include "Utilities.hpp"

#ifndef _included_Rgba__
#define _included_Rgba__

static const float g_ColorBits = 255.0f;
static const float g_oneOverColorBits = 1.0f / g_ColorBits;

class Vector4;

class Rgba{
public:
	 Rgba();
	 Rgba(const Rgba& newColor);
	 Rgba(const Rgba& newColor, const unsigned char& newA );
	 Rgba(const unsigned char& newR, const unsigned char& newG, const unsigned char& newB, const unsigned char& newA = 255);
	 Rgba(const std::string& colorString);

	~Rgba();

	const float fR() const;
	const float fG() const;
	const float fB() const;
	const float fA() const;

	static const unsigned char cR(const float& r);
	static const unsigned char cG(const float& g);
	static const unsigned char cB(const float& b);
	static const unsigned char cA(const float& a);
	
	Vector4 ToVector4() const;
	
	
	//operators
	bool Rgba::operator==(const Rgba& rgbaToEqual) const;
	bool Rgba::operator!=(const Rgba& rgbaToEqual) const;

	//friend methods
	friend Vector4 ToVector4(const Rgba& colorToConvert);
	friend const Rgba Interpolate(const Rgba& start, const Rgba& end, float fractionFromStartToEnd );
	friend Rgba StringToRgba(const std::string& s);
	friend void SetFromString(Rgba& out_value, const std::string& attrString) {
		out_value = StringToRgba(attrString);
	}
	
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
	//solid white
	r = 255;
	g = 255;
	b = 255;
	a = 255;
	//translucent magenta
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

inline Rgba::~Rgba(){
	//do nothing
}

inline const float Rgba::fR() const {
	return (float)( (float)r * g_oneOverColorBits);
}

inline const float Rgba::fG() const {
	return (float)( (float)g * g_oneOverColorBits);
}

inline const float Rgba::fB()const {
	return (float) b * g_oneOverColorBits;
}

inline const float Rgba::fA() const {
	return (float) a * g_oneOverColorBits;
}

inline const unsigned char Rgba::cR(const float& r){
	return (unsigned char)((float)r * g_ColorBits);
}

inline const unsigned char Rgba::cG(const float& g){
	return (unsigned char)((float)g * g_ColorBits);
}

inline const unsigned char Rgba::cB(const float& b){
	return (unsigned char)((float)b * g_ColorBits);
}

inline const unsigned char Rgba::cA(const float& a){
	return (unsigned char)((float)a * g_ColorBits);
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

//-----------------------------------------------------------------------------------------------------------

///----------------------------------------------------------------------------------------------------------
///friend methods


//===========================================================================================================

typedef std::map<std::string, Rgba> ColorMap;
typedef std::pair<std::string, Rgba> ColorMapEntry;

struct ColorScheme{

	std::string name;
	ColorMap colors;

};

typedef std::map<std::string, ColorScheme> ColorSchemeMap;
typedef std::pair<std::string, ColorScheme> ColorSchemeMapEntry;

//===========================================================================================================

#endif