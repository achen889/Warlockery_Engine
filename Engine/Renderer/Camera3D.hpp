//==============================================================================================================
//Camera3D.hpp
//by Albert Chen Mar-26-2015.
//==============================================================================================================

#pragma once

#ifndef _included_Camera3D__
#define _included_Camera3D__

#include "Engine\Math\Vector3.hpp"
#include "Engine\Math\EulerAngles.hpp"
#include "Engine\Math\Matrix4.hpp"
#include <math.h>
//#include "Engine\Console\DevConsole.hpp"

//Ephanov y up vector
static const Vector3 worldUnitUp(0.0f, 0.0f, 1.0f);

class Camera3D{
public:

	Camera3D() {
		//do nothing 
		//REGISTER_CONSOLE_COMMAND(GET_CAM_POS);
	}
	~Camera3D(){
		//do nothing
	}

	Camera3D(const Vector3& position, const EulerAngles& orientation );
	Vector3 GetForwardVector();
	Matrix4 LookAt(const Vector3& pointToLookAt);
	std::string ToString();
	void UpdateCameraFromInput(float deltaSeconds, const float& moveSpeed = 16.0f);
	Matrix4 GetCameraRotationMatrix();
	Matrix4 GetCameraViewMatrix();
		
public:
	//camera location in space
	Vector3 m_position;
	EulerAngles m_orientation;
	
	Vector3 vectorIofCamera;
	Vector3 vectorJofCamera;
	Vector3 vectorKofCamera;
};

///----------------------------------------------------------------------------------------------------------
///inline methods



inline Vector3 Camera3D::GetForwardVector(){
	float yawRadians = ConvertDegreesToRadians( m_orientation.yawDegreesAboutZ );
	float cosineOfYawRadians = cos(yawRadians);
	float sineOfYawRadians =  sin(yawRadians);
	//if pitch is 0 we only care about XY
	if(m_orientation.pitchDegreesAboutY == 0.0f){
		return Vector3(cosineOfYawRadians, sineOfYawRadians, 0.0f );
	}
	else{
		float pitchRadians = ConvertDegreesToRadians( m_orientation.pitchDegreesAboutY);
		float cosineOfPitchRadians = cos(pitchRadians);
		float negativeSineOfPitchRadians = -1.0f * sin(pitchRadians);

		return Vector3(cosineOfPitchRadians * cosineOfYawRadians, cosineOfPitchRadians * sineOfYawRadians, negativeSineOfPitchRadians);
	}//end of if/else
}

#endif