//==============================================================================================================
//Lighting.hpp
//by Albert Chen Jul-6-2015.
//==============================================================================================================

#pragma once
#include "Engine\Math\Vector3.hpp"
#include "Engine\Core\Rgba.hpp"
#include "Engine\Math\Vector2.hpp"

#ifndef _included_Light__
#define _included_Light__

enum LightType{
	LIGHT_TYPE_POINT_LIGHT,
	LIGHT_TYPE_DIRECTIONAL_LIGHT,
	LIGHT_TYPE_SPOT_LIGHT,
};

static const int MAX_LIGHTS_TO_BIND = 16;

class Camera3D;
class OpenGLRenderer;


struct Light{

	Vector3 m_lightPosition;
	Rgba m_lightColor;
	float m_lightPower;

	float m_lightPositionFactor;
	float m_lightFacingFactor;

	Vector3 m_lightFacing;
	float m_spotFactor;
	float m_spotPower;

	Vector2 m_lightIntensity;
	float m_lightAngle;
	

	Light();
	~Light();

	Light(LightType mylightType, Vector3 lightPosition, float lightPower, const Rgba& lightColor = Rgba::WHITE, Vector3 lightFacing = Vector3::ZERO, float lightAngle = -1.0f );
	
	float GetLightAngle();

	void RenderDebugLight(OpenGLRenderer* renderer, Camera3D& camera );

};


typedef std::vector<Light*> Lights;

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline functions

inline Light::Light(){
	//do nothing
}

inline Light::~Light(){
	//do nothing
}

inline float Light::GetLightAngle(){
	if (m_lightAngle < 0.0f){
		return -1.0f;
	}
	return cos((ConvertDegreesToRadians(m_lightAngle* 0.5f ) ));
}

//-----------------------------------------------------------------------------------------------------------

#endif