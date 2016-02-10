//==============================================================================================================
//EulerAngles.hpp
//by Albert Chen Mar-26-2015.
//==============================================================================================================

#pragma once

#ifndef _included_EulerAngles__
#define _included_EulerAngles__

#include "Vector3.hpp"

class EulerAngles{
public:
	//variables
	float yawDegreesAboutZ;
	float pitchDegreesAboutY;
	float rollDegreesAboutX;

	//methods
	EulerAngles();
	EulerAngles(const float& newYaw, const float& newPitch, const float& newRoll );
	EulerAngles(const EulerAngles& newEulerAngles);

	Vector3 ToVector3();

};
///----------------------------------------------------------------------------------------------------------
///inline functions
///----------------------------------------------------------------------------------------------------------
///constructors
inline EulerAngles::EulerAngles(){
	//do nothing
}

inline EulerAngles::EulerAngles(const float& newYaw, const float& newPitch, const float& newRoll ){
	yawDegreesAboutZ = newYaw;
	pitchDegreesAboutY = newPitch;
	rollDegreesAboutX = newRoll;
}

inline EulerAngles::EulerAngles(const EulerAngles& newEulerAngles){
	yawDegreesAboutZ = newEulerAngles.yawDegreesAboutZ;
	pitchDegreesAboutY = newEulerAngles.pitchDegreesAboutY;
	rollDegreesAboutX = newEulerAngles.rollDegreesAboutX;
}

///----------------------------------------------------------------------------------------------------------
///accessors

inline Vector3 EulerAngles::ToVector3(){


	//return Vector3(-yawDegreesAboutZ, -pitchDegreesAboutY, rollDegreesAboutX);

	return Vector3(rollDegreesAboutX, pitchDegreesAboutY, yawDegreesAboutZ);
}

#endif