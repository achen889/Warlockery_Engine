//==============================================================================================================
//Effect.hpp
//by Albert Chen Oct-19-2015.
//==============================================================================================================

#pragma once

#ifndef _included_Effect__
#define _included_Effect__

#include "ParticlePhysics.hpp"
//#include "Particle.hpp"
#include "Engine\Math\Vector2.hpp"
#include "Engine\Core\XML\XMLUtils.hpp"

//===========================================================================================================

enum EffectType{
	EFFECT_TYPE_EXPLOSION, //basic
	EFFECT_TYPE_FIREWORK, //explosion with gravity
	EFFECT_TYPE_FOUNTAIN, //basic
	EFFECT_TYPE_SMOKE, //fountain with wind
	EFFECT_TYPE_DEBRIS, //still dunno how to do efficient collision
	EFFECT_TYPE_TRAIL, //does nothing, must use pos to create trail
	EFFECT_TYPE_TWISTER, //attempt at a tornado
	EFFECT_TYPE_BLACKHOLE, //contracts towards center
	EFFECT_TYPE_RINGS, //pretty ring patterns
	NUM_OF_EFFECT_TYPES
};

//===========================================================================================================

struct Effect{
	
	float magnitude;
	float spread;

	EffectType type;

	Effect(const EffectType& newEffectType, const float& newmagnitude = 10.0f, const float& spread = 3.0f);

	Effect(XMLNode& effectNode);
	
	std::string GetEffectTypeString();

	void SetMagnitude(const float& newmagnitude){ magnitude = newmagnitude; }

	Vector3 GetForce(Particle& particleToApply);

	Vector3 GetInitialVelocity();
	
};

//===========================================================================================================

#endif //__includedEffect__

