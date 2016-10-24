//==============================================================================================================
//CubicHermiteCurve.cpp
//by Albert Chen Aug-22-2016.
//==============================================================================================================

#include "CubicHermiteCurve.hpp"

//===========================================================================================================


Vector2 CubicHermiteCurve::CalcPositionAtParametric(float t) {

	//Cubic Hermite Curve Equation
	//P(t) = s2(1 + 2t)A + t2(1 + 2s)D + s2tU – st2V

	t = GetLoopedParametric(t);

	float s = 1.0f - t;
	float sSquared = s * s;
	float tSquared = t * t;

	float coeffientOfA = sSquared * (1.0f + (2.0f * t));

	float coefficientOfD = tSquared * (1.0f + (2 * s));

	float coeffientOfU = sSquared * t;

	float coeffientOfV = s * tSquared;


	Vector2 pT = coeffientOfA * startPoint.pos + coefficientOfD * endPoint.pos + 
		coeffientOfU * startPoint.vel - coeffientOfV * endPoint.vel;

	return pT;
}

//-----------------------------------------------------------------------------------------------------------

Vector2 CubicHermiteCurve::CalcPositionAtParametric(float t, const Vector2& offset) {

	//Cubic Hermite Curve Equation
	//P(t) = s2(1 + 2t)A + t2(1 + 2s)D + s2tU – st2V
	
	t = GetLoopedParametric(t);

	float s = 1.0f - t;
	float sSquared = s * s;
	float tSquared = t * t;

	float coeffientOfA = sSquared * (1.0f + (2.0f * t));

	float coefficientOfD = tSquared * (1.0f + (2 * s));

	float coeffientOfU = sSquared * t;

	float coeffientOfV = s * tSquared;


	Vector2 pT = coeffientOfA * (startPoint.pos + offset) + coefficientOfD * (endPoint.pos + offset) +
		coeffientOfU * startPoint.vel - coeffientOfV * endPoint.vel;

	return pT;
}

//-----------------------------------------------------------------------------------------------------------


//===========================================================================================================

