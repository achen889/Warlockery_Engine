//==============================================================================================================
//Noise.hpp
//by Squirrel Eiserloh Apr-2-2015.
//modified by Albert Chen
//==============================================================================================================
#pragma once
#ifndef included_Noise
#define included_Noise

//---------------------------------------------------------------------------
#include "Vector2.hpp"
#include "IntVec2.hpp"

//Rgba GetPseudoRandomNoiseColor(Rgba inputColor );
//---------------------------------------------------------------------------
//Eiserloh functions
float GetPseudoRandomNoiseValueNegOneToOne1D( int position );
float GetPseudoRandomNoiseValueZeroToOne1D( int position );
float GetPseudoRandomNoiseValueNegOneToOne2D( int positionX, int positionY );
float GetPseudoRandomNoiseValueZeroToOne2D( int positionX, int positionY );
float GetPseudoRandomNoiseValueNegOneToOne3D( int positionX, int positionY, int positionZ );
float GetPseudoRandomNoiseValueZeroToOne3D( int positionX, int positionY, int positionZ );
float GetPseudoNoiseAngleRadians1D( int position );
float GetPseudoNoiseAngleRadians2D( int positionX, int positionY );
Vector2 GetPseudoRandomNoiseUnitVector2D( int positionX, int positionY, int randomSeed = 0 );
float ComputePerlinNoiseValueAtPosition2D( const Vector2& position,float perlinNoiseGridCellSize,int numOctaves,float baseAmplitude,float persistance,int randomSeed = 0);

//---------------------------------------------------------------------------
const float cosineLookup256[ 256 ] =
{
	 1.0000000f,
	 0.9996965f,
	 0.9987860f,
	 0.9972692f,
	 0.9951469f,
	 0.9924205f,
	 0.9890916f,
	 0.9851622f,
	 0.9806348f,
	 0.9755120f,
	 0.9697969f,
	 0.9634931f,
	 0.9566044f,
	 0.9491349f,
	 0.9410893f,
	 0.9324722f,
	 0.9232891f,
	 0.9135455f,
	 0.9032472f,
	 0.8924006f,
	 0.8810122f,
	 0.8690889f,
	 0.8566381f,
	 0.8436671f,
	 0.8301840f,
	 0.8161969f,
	 0.8017143f,
	 0.7867449f,
	 0.7712980f,
	 0.7553827f,
	 0.7390089f,
	 0.7221865f,
	 0.7049255f,
	 0.6872367f,
	 0.6691306f,
	 0.6506183f,
	 0.6317110f,
	 0.6124202f,
	 0.5927576f,
	 0.5727351f,
	 0.5523650f,
	 0.5316595f,
	 0.5106312f,
	 0.4892929f,
	 0.4676576f,
	 0.4457384f,
	 0.4235485f,
	 0.4011015f,
	 0.3784111f,
	 0.3554908f,
	 0.3323548f,
	 0.3090170f,
	 0.2854916f,
	 0.2617929f,
	 0.2379352f,
	 0.2139331f,
	 0.1898011f,
	 0.1655539f,
	 0.1412062f,
	 0.1167727f,
	 0.0922684f,
	 0.0677080f,
	 0.0431065f,
	 0.0184789f,
	-0.0061599f,
	-0.0307951f,
	-0.0554115f,
	-0.0799943f,
	-0.1045285f,
	-0.1289992f,
	-0.1533917f,
	-0.1776910f,
	-0.2018824f,
	-0.2259513f,
	-0.2498830f,
	-0.2736630f,
	-0.2972769f,
	-0.3207102f,
	-0.3439489f,
	-0.3669788f,
	-0.3897859f,
	-0.4123563f,
	-0.4346764f,
	-0.4567326f,
	-0.4785116f,
	-0.5000000f,
	-0.5211849f,
	-0.5420534f,
	-0.5625928f,
	-0.5827906f,
	-0.6026346f,
	-0.6221128f,
	-0.6412133f,
	-0.6599245f,
	-0.6782351f,
	-0.6961339f,
	-0.7136102f,
	-0.7306531f,
	-0.7472525f,
	-0.7633983f,
	-0.7790806f,
	-0.7942899f,
	-0.8090170f,
	-0.8232529f,
	-0.8369891f,
	-0.8502171f,
	-0.8629290f,
	-0.8751170f,
	-0.8867737f,
	-0.8978920f,
	-0.9084653f,
	-0.9184870f,
	-0.9279511f,
	-0.9368518f,
	-0.9451838f,
	-0.9529420f,
	-0.9601216f,
	-0.9667184f,
	-0.9727283f,
	-0.9781476f,
	-0.9829731f,
	-0.9872018f,
	-0.9908313f,
	-0.9938591f,
	-0.9962837f,
	-0.9981033f,
	-0.9993171f,
	-0.9999241f,
	-0.9999241f,
	-0.9993171f,
	-0.9981033f,
	-0.9962837f,
	-0.9938591f,
	-0.9908313f,
	-0.9872018f,
	-0.9829731f,
	-0.9781476f,
	-0.9727283f,
	-0.9667184f,
	-0.9601216f,
	-0.9529420f,
	-0.9451838f,
	-0.9368518f,
	-0.9279511f,
	-0.9184870f,
	-0.9084653f,
	-0.8978920f,
	-0.8867737f,
	-0.8751170f,
	-0.8629290f,
	-0.8502171f,
	-0.8369891f,
	-0.8232529f,
	-0.8090170f,
	-0.7942899f,
	-0.7790806f,
	-0.7633983f,
	-0.7472525f,
	-0.7306531f,
	-0.7136102f,
	-0.6961339f,
	-0.6782351f,
	-0.6599245f,
	-0.6412133f,
	-0.6221128f,
	-0.6026346f,
	-0.5827906f,
	-0.5625928f,
	-0.5420534f,
	-0.5211849f,
	-0.5000000f,
	-0.4785116f,
	-0.4567326f,
	-0.4346764f,
	-0.4123563f,
	-0.3897859f,
	-0.3669788f,
	-0.3439489f,
	-0.3207102f,
	-0.2972769f,
	-0.2736630f,
	-0.2498830f,
	-0.2259513f,
	-0.2018824f,
	-0.1776910f,
	-0.1533917f,
	-0.1289992f,
	-0.1045285f,
	-0.0799943f,
	-0.0554115f,
	-0.0307951f,
	-0.0061599f,
	 0.0184789f,
	 0.0431065f,
	 0.0677080f,
	 0.0922684f,
	 0.1167727f,
	 0.1412062f,
	 0.1655539f,
	 0.1898011f,
	 0.2139331f,
	 0.2379352f,
	 0.2617929f,
	 0.2854916f,
	 0.3090170f,
	 0.3323548f,
	 0.3554908f,
	 0.3784111f,
	 0.4011015f,
	 0.4235485f,
	 0.4457384f,
	 0.4676576f,
	 0.4892929f,
	 0.5106312f,
	 0.5316595f,
	 0.5523650f,
	 0.5727351f,
	 0.5927576f,
	 0.6124202f,
	 0.6317110f,
	 0.6506183f,
	 0.6691306f,
	 0.6872367f,
	 0.7049255f,
	 0.7221865f,
	 0.7390089f,
	 0.7553827f,
	 0.7712980f,
	 0.7867449f,
	 0.8017143f,
	 0.8161969f,
	 0.8301840f,
	 0.8436671f,
	 0.8566381f,
	 0.8690889f,
	 0.8810122f,
	 0.8924006f,
	 0.9032472f,
	 0.9135455f,
	 0.9232891f,
	 0.9324722f,
	 0.9410893f,
	 0.9491349f,
	 0.9566044f,
	 0.9634931f,
	 0.9697969f,
	 0.9755120f,
	 0.9806348f,
	 0.9851622f,
	 0.9890916f,
	 0.9924205f,
	 0.9951469f,
	 0.9972692f,
	 0.9987860f,
	 0.9996965f,
	 1.0000000f
};


//---------------------------------------------------------------------------
const float sineLookup256[ 256 ] =
{
	 0.0000000f,
	 0.0246374f,
	 0.0492599f,
	 0.0738525f,
	 0.0984003f,
	 0.1228883f,
	 0.1473017f,
	 0.1716257f,
	 0.1958455f,
	 0.2199464f,
	 0.2439137f,
	 0.2677330f,
	 0.2913897f,
	 0.3148696f,
	 0.3381583f,
	 0.3612417f,
	 0.3841057f,
	 0.4067366f,
	 0.4291206f,
	 0.4512441f,
	 0.4730936f,
	 0.4946558f,
	 0.5159178f,
	 0.5368666f,
	 0.5574894f,
	 0.5777738f,
	 0.5977075f,
	 0.6172782f,
	 0.6364742f,
	 0.6552839f,
	 0.6736956f,
	 0.6916984f,
	 0.7092813f,
	 0.7264336f,
	 0.7431448f,
	 0.7594049f,
	 0.7752040f,
	 0.7905324f,
	 0.8053809f,
	 0.8197405f,
	 0.8336024f,
	 0.8469582f,
	 0.8597999f,
	 0.8721195f,
	 0.8839097f,
	 0.8951633f,
	 0.9058734f,
	 0.9160336f,
	 0.9256377f,
	 0.9346798f,
	 0.9431544f,
	 0.9510565f,
	 0.9583812f,
	 0.9651241f,
	 0.9712810f,
	 0.9768483f,
	 0.9818226f,
	 0.9862007f,
	 0.9899802f,
	 0.9931587f,
	 0.9957342f,
	 0.9977052f,
	 0.9990705f,
	 0.9998293f,
	 0.9999810f,
	 0.9995257f,
	 0.9984636f,
	 0.9967953f,
	 0.9945219f,
	 0.9916447f,
	 0.9881655f,
	 0.9840863f,
	 0.9794098f,
	 0.9741386f,
	 0.9682760f,
	 0.9618256f,
	 0.9547913f,
	 0.9471774f,
	 0.9389884f,
	 0.9302293f,
	 0.9209055f,
	 0.9110226f,
	 0.9005867f,
	 0.8896040f,
	 0.8780812f,
	 0.8660254f,
	 0.8534438f,
	 0.8403441f,
	 0.8267342f,
	 0.8126224f,
	 0.7980172f,
	 0.7829276f,
	 0.7673627f,
	 0.7513319f,
	 0.7348450f,
	 0.7179119f,
	 0.7005430f,
	 0.6827489f,
	 0.6645402f,
	 0.6459281f,
	 0.6269238f,
	 0.6075389f,
	 0.5877853f,
	 0.5676747f,
	 0.5472195f,
	 0.5264322f,
	 0.5053252f,
	 0.4839114f,
	 0.4622039f,
	 0.4402157f,
	 0.4179603f,
	 0.3954512f,
	 0.3727020f,
	 0.3497265f,
	 0.3265387f,
	 0.3031527f,
	 0.2795826f,
	 0.2558428f,
	 0.2319476f,
	 0.2079117f,
	 0.1837495f,
	 0.1594758f,
	 0.1351052f,
	 0.1106527f,
	 0.0861329f,
	 0.0615609f,
	 0.0369515f,
	 0.0123197f,
	-0.0123197f,
	-0.0369515f,
	-0.0615609f,
	-0.0861329f,
	-0.1106527f,
	-0.1351052f,
	-0.1594758f,
	-0.1837495f,
	-0.2079117f,
	-0.2319476f,
	-0.2558428f,
	-0.2795826f,
	-0.3031527f,
	-0.3265387f,
	-0.3497265f,
	-0.3727020f,
	-0.3954512f,
	-0.4179603f,
	-0.4402157f,
	-0.4622039f,
	-0.4839114f,
	-0.5053252f,
	-0.5264322f,
	-0.5472195f,
	-0.5676747f,
	-0.5877853f,
	-0.6075389f,
	-0.6269238f,
	-0.6459281f,
	-0.6645402f,
	-0.6827489f,
	-0.7005430f,
	-0.7179119f,
	-0.7348450f,
	-0.7513319f,
	-0.7673627f,
	-0.7829276f,
	-0.7980172f,
	-0.8126224f,
	-0.8267342f,
	-0.8403441f,
	-0.8534438f,
	-0.8660254f,
	-0.8780812f,
	-0.8896040f,
	-0.9005867f,
	-0.9110226f,
	-0.9209055f,
	-0.9302293f,
	-0.9389884f,
	-0.9471774f,
	-0.9547913f,
	-0.9618256f,
	-0.9682760f,
	-0.9741386f,
	-0.9794098f,
	-0.9840863f,
	-0.9881655f,
	-0.9916447f,
	-0.9945219f,
	-0.9967953f,
	-0.9984636f,
	-0.9995257f,
	-0.9999810f,
	-0.9998293f,
	-0.9990705f,
	-0.9977052f,
	-0.9957342f,
	-0.9931587f,
	-0.9899802f,
	-0.9862007f,
	-0.9818226f,
	-0.9768483f,
	-0.9712810f,
	-0.9651241f,
	-0.9583812f,
	-0.9510565f,
	-0.9431544f,
	-0.9346798f,
	-0.9256377f,
	-0.9160336f,
	-0.9058734f,
	-0.8951633f,
	-0.8839097f,
	-0.8721195f,
	-0.8597999f,
	-0.8469582f,
	-0.8336024f,
	-0.8197405f,
	-0.8053809f,
	-0.7905324f,
	-0.7752040f,
	-0.7594049f,
	-0.7431448f,
	-0.7264336f,
	-0.7092813f,
	-0.6916984f,
	-0.6736956f,
	-0.6552839f,
	-0.6364742f,
	-0.6172782f,
	-0.5977075f,
	-0.5777738f,
	-0.5574894f,
	-0.5368666f,
	-0.5159178f,
	-0.4946558f,
	-0.4730936f,
	-0.4512441f,
	-0.4291206f,
	-0.4067366f,
	-0.3841057f,
	-0.3612417f,
	-0.3381583f,
	-0.3148696f,
	-0.2913897f,
	-0.2677330f,
	-0.2439137f,
	-0.2199464f,
	-0.1958455f,
	-0.1716257f,
	-0.1473017f,
	-0.1228883f,
	-0.0984003f,
	-0.0738525f,
	-0.0492599f,
	-0.0246374f,
	 0.0000000f,
};

//===========================================================================================================

// Rgba GetPseudoRandomNoiseColor(Rgba inputColor ){
// 	//get random red
// 	
// 	float noiseRed = GetPseudoRandomNoiseValueZeroToOne1D( (int)inputColor.r );
// 	float noiseGreen = GetPseudoRandomNoiseValueZeroToOne1D( (int)inputColor.g );
// 	float noiseBlue = GetPseudoRandomNoiseValueZeroToOne1D( (int)inputColor.b );
// 	float noiseAlpha = GetPseudoRandomNoiseValueZeroToOne1D( (int)inputColor.a );
// 
// 	//Rgba noiseColor 
// 	return Rgba::WHITE;
// 
// }



//---------------------------------------------------------------------------
inline float GetPseudoRandomNoiseValueNegOneToOne1D( int position )
{
	const float ONE_OVER_HALF_MAX_POSITIVE_INT = (1.f / 1073741824.f);
	int bits = (position << 13) ^ position;
	int pseudoRandomPositiveInt = (bits * ((bits * bits * 15731) + 789221) + 1376312589) & 0x7fffffff;
	float pseudoRandomFloatNegOneToOne = 1.f - (ONE_OVER_HALF_MAX_POSITIVE_INT * (float) pseudoRandomPositiveInt);
	return pseudoRandomFloatNegOneToOne;
}


//---------------------------------------------------------------------------
inline float GetPseudoRandomNoiseValueZeroToOne1D( int position )
{
	const float ONE_OVER_MAX_POSITIVE_INT = (1.f / 2147483648.f);
	int bits = (position << 13) ^ position;
	int pseudoRandomPositiveInt = (bits * ((bits * bits * 15731) + 789221) + 1376312589) & 0x7fffffff;
	float pseudoRandomFloatZeroToOne = ONE_OVER_MAX_POSITIVE_INT * (float) pseudoRandomPositiveInt;
	return pseudoRandomFloatZeroToOne;
}


//---------------------------------------------------------------------------
inline float GetPseudoRandomNoiseValueNegOneToOne2D( int positionX, int positionY )
{
	const float ONE_OVER_HALF_MAX_POSITIVE_INT = (1.f / 1073741824.f);
	int position = positionX + (positionY * 57);
	int bits = (position << 13) ^ position;
	int pseudoRandomPositiveInt = (bits * ((bits * bits * 15731) + 789221) + 1376312589) & 0x7fffffff;
	float pseudoRandomFloatNegOneToOne = 1.f - (ONE_OVER_HALF_MAX_POSITIVE_INT * (float) pseudoRandomPositiveInt);
	return pseudoRandomFloatNegOneToOne;
}


//---------------------------------------------------------------------------
inline float GetPseudoRandomNoiseValueZeroToOne2D( int positionX, int positionY )
{
	const float ONE_OVER_MAX_POSITIVE_INT = (1.f / 2147483648.f);
	int position = positionX + (positionY * 57);
	int bits = (position << 13) ^ position;
	int pseudoRandomPositiveInt = (bits * ((bits * bits * 15731) + 789221) + 1376312589) & 0x7fffffff;
	float pseudoRandomFloatZeroToOne = ONE_OVER_MAX_POSITIVE_INT * (float) pseudoRandomPositiveInt;
	return pseudoRandomFloatZeroToOne;
}


//---------------------------------------------------------------------------
inline float GetPseudoRandomNoiseValueNegOneToOne3D( int positionX, int positionY, int positionZ )
{
	const float ONE_OVER_HALF_MAX_POSITIVE_INT = (1.f / 1073741824.f);
	int position = positionX + (positionY * 57) + (positionZ * 3023);
	int bits = (position << 13) ^ position;
	int pseudoRandomPositiveInt = (bits * ((bits * bits * 15731) + 789221) + 1376312589) & 0x7fffffff;
	float pseudoRandomFloatNegOneToOne = 1.f - (ONE_OVER_HALF_MAX_POSITIVE_INT * (float) pseudoRandomPositiveInt);
	return pseudoRandomFloatNegOneToOne;
}


//---------------------------------------------------------------------------
inline float GetPseudoRandomNoiseValueZeroToOne3D( int positionX, int positionY, int positionZ )
{
	const float ONE_OVER_MAX_POSITIVE_INT = (1.f / 2147483648.f);
	int seed = positionX + (positionY * 57) + (positionZ * 3023);
	int bits = (seed << 13) ^ seed;
	int pseudoRandomPositiveInt = (bits * ((bits * bits * 15731) + 789221) + 1376312589) & 0x7fffffff;
	float pseudoRandomFloatZeroToOne = ONE_OVER_MAX_POSITIVE_INT * (float) pseudoRandomPositiveInt;
	return pseudoRandomFloatZeroToOne;
}


//---------------------------------------------------------------------------
inline float GetPseudoNoiseAngleRadians1D( int position )
{
	const float ONE_OVER_MAX_POSITIVE_INT = (1.f / 2147483648.f);
	const float SCALE_FACTOR = ONE_OVER_MAX_POSITIVE_INT * TWO_PI;
	int bits = (position << 13) ^ position;
	int pseudoRandomPositiveInt = (bits * ((bits * bits * 15731) + 789221) + 1376312589) & 0x7fffffff;
	float pseudoRandomFloatZeroToTwoPi = SCALE_FACTOR * (float) pseudoRandomPositiveInt;
	return pseudoRandomFloatZeroToTwoPi;
}


//---------------------------------------------------------------------------
inline float GetPseudoNoiseAngleRadians2D( int positionX, int positionY )
{
	const float ONE_OVER_MAX_POSITIVE_INT = (1.f / 2147483648.f);
	const float SCALE_FACTOR = ONE_OVER_MAX_POSITIVE_INT * TWO_PI;
	int position = positionX + (positionY * 57);
	int bits = (position << 13) ^ position;
	int pseudoRandomPositiveInt = (bits * ((bits * bits * 15731) + 789221) + 1376312589) & 0x7fffffff;
	float pseudoRandomFloatZeroToTwoPi = SCALE_FACTOR * (float) pseudoRandomPositiveInt;
	return pseudoRandomFloatZeroToTwoPi;
}


//---------------------------------------------------------------------------
inline Vector2 GetPseudoRandomNoiseUnitVector2D( int xPosition, int yPosition, int randomSeed )
{
//	const float ONE_OVER_MAX_POSITIVE_INT = (1.f / 2147483648.f);
//	const float SCALE_FACTOR = ONE_OVER_MAX_POSITIVE_INT * TWO_PI;
	int position = xPosition + (yPosition * 57) + (randomSeed * 95651);
	int bits = (position << 13) ^ position;
	int pseudoRandomPositiveInt = (bits * ((bits * bits * 15731) + 789221) + 1376312589) & 0x7fffffff;
	unsigned char randomByte = 0xff & pseudoRandomPositiveInt;

	return Vector2( cosineLookup256[ randomByte ], sineLookup256[ randomByte ] );
//	float pseudoRandomFloatZeroToTwoPi = SCALE_FACTOR * (float) pseudoRandomPositiveInt;
//	return Vector2( cos( pseudoRandomFloatZeroToTwoPi ), sin( pseudoRandomFloatZeroToTwoPi ) );
}

#endif // included_Noise
