//==============================================================================================================
//Light.cpp
//by Albert Chen Jul-6-2015.
//==============================================================================================================

#include "Light.hpp"
#include "Engine/Renderer/OpenGLRenderer.hpp"

Light::Light(LightType mylighttype, Vector3 lightPosition, float lightPower, const Rgba& lightColor, Vector3 lightFacing, float lightAngle ) :
m_lightPosition(lightPosition),
m_lightPower(lightPower),
m_lightColor(lightColor),
m_lightFacing(lightFacing),
m_lightAngle(lightAngle)
{
	if (m_lightFacing != Vector3::ZERO){
		m_lightFacing.Normalize();
	}	

	if (mylighttype == LIGHT_TYPE_POINT_LIGHT){
		m_lightPositionFactor = 1.0f;

		m_lightFacingFactor = 0.0f;

		m_spotPower = 1.0f;

		m_lightIntensity = Vector2(0.0f, 1.0f);

		m_lightAngle = -1.0f;
	}

	else if (mylighttype == LIGHT_TYPE_DIRECTIONAL_LIGHT){
		m_lightPositionFactor = 0.0f;

		m_lightFacingFactor = 1.0f;

		m_spotPower = 0.0f;

		m_lightAngle = -1.0f;

		m_lightIntensity = Vector2(lightPower, lightPower);
	}

	else if (mylighttype == LIGHT_TYPE_SPOT_LIGHT){
		m_lightPositionFactor = 1.0f;

		m_lightFacingFactor = 0.0f;

		m_lightIntensity = Vector2(0.0f, 1.0f);

		m_lightAngle = 15.0f;

		m_spotPower = 1.0f;
	}

}

//-----------------------------------------------------------------------------------------------------------

void Light::RenderDebugLight(OpenGLRenderer* renderer, Camera3D& camera){
	UNUSED(camera);
	//renderer->ApplyCameraTransform(camera);

	renderer->DrawSphere3D(Sphere3(m_lightPosition, 0.03f), 4, 4, m_lightColor);
	renderer->DrawArrow3D(LineSegment3( m_lightPosition, m_lightPosition + m_lightFacing), m_lightColor, m_lightColor);
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///friend methods

