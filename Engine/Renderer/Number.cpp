//==============================================================================================================
//Numbers.cpp
//by Albert Chen Feb-23-2015.
//==============================================================================================================


#include "Number.hpp"

AnimatedTexture* Number::m_numberTexture = NULL;

Number::Number(const Vector2& position, unsigned int value ){
	m_numberTexture = AnimatedTexture::CreateOrGetAnimatedTexture("Data/Images/Numbers/numbers.png", 10, Vector2(62.6f , 88.0f));

	m_position = position;

	m_value = value;

	NumbersQuad.push_back(Vector2(0.0f, 0.0f ) );
	NumbersQuad.push_back(Vector2(NUMBERS_SIZE_PIXELS , 0.0f )  );
	NumbersQuad.push_back(Vector2(NUMBERS_SIZE_PIXELS, NUMBERS_SIZE_PIXELS ) );
	NumbersQuad.push_back(Vector2(0.0f, NUMBERS_SIZE_PIXELS )  );
}

void Number::Update(double deltaSeconds ){
	UNUSED(deltaSeconds);
	//do nothing
}

void Number::Draw( OpenGLRenderer& effectRenderer ){
	
	//if(ElapsedTimeSinceEffectSpawn < EffectDurationSeconds ){
		effectRenderer.PushMatrix();
		//effectRenderer.SetModelViewColor(Rgba::WHITE );
		effectRenderer.SetModelViewTranslation(m_position.x - ( NUMBERS_SIZE_PIXELS * 0.5f ) , m_position.y -( NUMBERS_SIZE_PIXELS * 0.5f ) );

		Vector2 minCoordinatesOut;
		Vector2 maxCoordinatesOut;
		m_numberTexture->CalcTextureCoordinatesAtPercentComplete(m_value * ONE_OVER_NUMBERS , minCoordinatesOut, maxCoordinatesOut );
	
		Vector2s numberTextureCoordinates;

		numberTextureCoordinates.push_back(Vector2(minCoordinatesOut.x , maxCoordinatesOut.y ) );
		numberTextureCoordinates.push_back(maxCoordinatesOut );
		numberTextureCoordinates.push_back(Vector2(maxCoordinatesOut.x , minCoordinatesOut.y ) );
		numberTextureCoordinates.push_back(minCoordinatesOut );
		
		effectRenderer.DrawTextureQuad(*m_numberTexture, NumbersQuad, numberTextureCoordinates );
		effectRenderer.PopMatrix();

}
//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///friend methods
///----------------------------------------------------------------------------------------------------------
///retrieves the digits in reverse order so the input position should be the one's digit of the number
Numbers ConvertUnsignedIntIntoNumbersAtPosition(const Vector2& position, const unsigned int& value){

	Numbers numbersForIntValue;
	unsigned int valueToConvert = value;
	int positionCounter = 0;

	if(valueToConvert == 0){
		numbersForIntValue.push_back(new Number( position - Vector2(( NUMBERS_SIZE_PIXELS * 0.7f * positionCounter) , 0.0f ), valueToConvert) );
	}
	
	while(valueToConvert > 0){
		unsigned int valueDigit = valueToConvert % 10;
		numbersForIntValue.push_back(new Number( position - Vector2(( NUMBERS_SIZE_PIXELS * 0.7f * positionCounter) , 0.0f ), valueDigit) );
		valueToConvert = (unsigned int)( (float)valueToConvert * ONE_OVER_NUMBERS );
		positionCounter++;
	}
	
	return numbersForIntValue;
}

void Draw(OpenGLRenderer& effectRenderer, std::vector<Number*> numbersToDraw ){
	for(std::vector<Number*>::iterator numberIter = numbersToDraw.begin(); numberIter != numbersToDraw.end(); ++numberIter ){
		Number* number = (*numberIter);
		number->Draw( effectRenderer );
	}
}
