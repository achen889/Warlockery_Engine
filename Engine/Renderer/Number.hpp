//==============================================================================================================
//Numbers.hpp
//by Albert Chen Feb-23-2015.
//==============================================================================================================

#pragma once

#ifndef _included_Numbers__
#define _included_Numbers__

#define NUMBERS_SIZE_PIXELS 62.6f
#define ONE_OVER_NUMBERS 0.1f

#include "Engine\Renderer\OpenGLRenderer.hpp"
#include "Engine\Core\Time.hpp"
#include "Engine\Renderer\AnimatedTexture.hpp"

class Number{
public:
	 Number();
	~Number();
	 Number(const Vector2& position, unsigned int value );

	void Update(double deltaSeconds );
	void Draw(OpenGLRenderer& effectRenderer );
	

	friend std::vector<Number*> ConvertUnsignedIntIntoNumbersAtPosition(const Vector2& position , const unsigned int& value);
	friend void Draw(OpenGLRenderer& effectRenderer, std::vector<Number*> numbersToDraw );

	//variables
	Vector2 m_position;
	unsigned int m_value;
	Vector2s NumbersQuad;
	static AnimatedTexture* m_numberTexture;
	//bool m_EffectComplete;
};
//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods
inline Number::Number(){
	//do nothing	
}
inline Number::~Number(){
	//do nothing
}

typedef std::vector<Number*>Numbers;



#endif
