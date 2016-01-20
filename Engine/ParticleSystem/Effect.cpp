//==============================================================================================================
//Effect.cpp
//by Albert Chen Oct-19-2015.
//==============================================================================================================

#include "Effect.hpp"

//===========================================================================================================

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///Effect Methods

Effect::Effect(const EffectType& newEffectType, const float& newmagnitude, const float& newspread) :
type(newEffectType),
magnitude(newmagnitude),
spread(newspread)
{

}

std::string Effect::GetEffectTypeString(){
	std::string myEffectTypeText = "";
	if (type == EFFECT_TYPE_EXPLOSION){
		myEffectTypeText = "Explosion";
	}
	else if (type == EFFECT_TYPE_RINGS){
		myEffectTypeText = "Rings";
	}
	else if (type == EFFECT_TYPE_FOUNTAIN){
		myEffectTypeText = "Fountain";
	}
	else if (type == EFFECT_TYPE_SMOKE){
		myEffectTypeText = "Smoke";
	}

	return myEffectTypeText;
}

Vector3 Effect::GetForce(Particle& particleToApply){

	if (type == EFFECT_TYPE_EXPLOSION){
		return Vector3::ZERO;
	}
	else if (type == EFFECT_TYPE_FIREWORK){
		return CalcGravityForce(particleToApply.GetMass());
	}
	else if (type == EFFECT_TYPE_FOUNTAIN){
		return CalcGravityForce(particleToApply.GetMass());
	}
	else if (type == EFFECT_TYPE_DEBRIS){
		return CalcGravityForce(particleToApply.GetMass());
	}
	else if (type == EFFECT_TYPE_SMOKE){
		Vector3 windForce = CalcWindForce(particleToApply);
		return windForce;
	}
	else if (type == EFFECT_TYPE_TWISTER){
		Vector3 windForce = -1.0f * CalcGravityForce(particleToApply.GetMass());
		static float angle = 0.0f;
		angle += 0.025f;
		//float randomFountainRadius = GetRandomFloatInRange(0.1f, spread * 0.25f);

		windForce += /*randomFountainRadius**/Vector3(sin(angle), cos(angle), GetRandomFloatInRange(-1.0f, 1.0f));
		return windForce;
	}
	else if (type == EFFECT_TYPE_BLACKHOLE){
		float randomFountainRadius = GetRandomFloatInRange(0.5f, spread);
		Vector3 randomCircleVector = randomFountainRadius * Vector3(sin(GetRandomAngleRadians()), cos(GetRandomAngleRadians()), GetRandomFloatInRange(-0.5f, 0.5f));
		particleToApply.SetPosition(randomCircleVector);
		float dampingC = 0.024f;
		Vector3 blackHoleForce = -dampingC * randomCircleVector;

		return blackHoleForce;
	}
	else if (type == EFFECT_TYPE_RINGS){
		static float angle = 0.0f;
		angle += 1.0f;
		static float spreadModifer = spread * 0.13f;
		spreadModifer -= 0.006f;
		//if (spreadModifer < 0.0f)spreadModifer = spread * 0.27;
		//float randomFountainRadius = GetRandomFloatInRange(0.5f, spread);
		Vector3 randomCircleVector = spreadModifer * Vector3(sin(angle), cos(angle), 0.0f);
		//particleToApply.SetPosition(randomCircleVector);
		float dampingC = 0.025f;
		Vector3 blackHoleForce = magnitude * dampingC * randomCircleVector;

		return blackHoleForce;
	}

	return Vector3::ZERO;
}

Vector3 Effect::GetInitialVelocity(){
	if (type == EFFECT_TYPE_EXPLOSION){
		return GetRandomInitialVelocity(magnitude);
	}
	else if (type == EFFECT_TYPE_FIREWORK){
		return GetRandomInitialVelocity(magnitude);
	}
	else if (type == EFFECT_TYPE_FOUNTAIN){
		float randomFountainRadius = GetRandomFloatInRange(0.0f, spread);
		Vector2 randomCircleVector = randomFountainRadius * Vector2(sin(GetRandomAngleRadians()), cos(GetRandomAngleRadians()));

		return Vector3(randomCircleVector.x, randomCircleVector.y, GetRandomFloatInRange(0.0f, magnitude));
	}
	else if (type == EFFECT_TYPE_DEBRIS){
		float randomFountainRadius = GetRandomFloatInRange(0.0f, spread);
		Vector2 randomCircleVector = randomFountainRadius * Vector2(sin(GetRandomAngleRadians()), cos(GetRandomAngleRadians()));

		return Vector3(randomCircleVector.x, randomCircleVector.y, GetRandomFloatInRange(0.0f, magnitude));
	}
	else if (type == EFFECT_TYPE_SMOKE){
		float randomFountainRadius = GetRandomFloatInRange(0.0f, spread);
		Vector2 randomCircleVector = randomFountainRadius * Vector2(sin(GetRandomAngleRadians()), cos(GetRandomAngleRadians()));

		return Vector3(randomCircleVector.x, randomCircleVector.y, GetRandomFloatInRange(0.0f, magnitude));
	}
	else if (type == EFFECT_TYPE_TWISTER){
		//tornado will likely rely on massive amounts of intervals
		static float angle = 0.0f;

		angle += 0.08f;

		static float zHeightModifier = 0.01f;
		static float zMax = 6.0f;
		static float zMin = 3.0f;
		static float zHeight = zMin;
		if (zHeight >= zMax)zHeight = zMin;
		else if (zHeight >= zMin)zHeight += zHeightModifier;



		float randomFountainRadius = GetRandomFloatInRange(0.1f, spread * 0.4f);
		Vector3 randomCircleVector = randomFountainRadius * Vector3(sin(angle), cos(angle), zHeight);
		//Vector3 randomCircleVector = randomFountainRadius * Vector3(sin(angle), cos(angle), GetRandomFloatInRange(0.0f, magnitude) );
		return randomCircleVector;
	}
	else if (type == EFFECT_TYPE_BLACKHOLE){
		//float randomFountainRadius = GetRandomFloatInRange(0.0f, spread);
		static float angle = 0.0f;
		angle += 1.6f;
		Vector3 randomCircleVector = spread * Vector3(sin(angle), cos(angle), 0.0f);
		float dampingC = 0.012f;
		Vector3 blackHoleForce = -dampingC * randomCircleVector;

		return blackHoleForce;
	}
	else if (type == EFFECT_TYPE_RINGS){
		
	}




	return Vector3::ZERO;
}


//===========================================================================================================

