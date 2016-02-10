//==============================================================================================================
//MatrixUtils.cpp
//by Albert Chen Jul-4-2015.
//==============================================================================================================

#include "MatrixUtils.hpp"
//#include "Engine\Console\DevConsole.hpp"

//===========================================================================================================

//-----------------------------------------------------------------------------------------------------------

void ModelViewMatrix::SetRotationDegrees3D(const EulerAngles& rotationDegrees3D){
	
	Matrix4 modelViewRollRotationX;
	modelViewRollRotationX.RotateDegreesAboutX(rotationDegrees3D.rollDegreesAboutX);

	Matrix4 modelViewPitchRotationY;
	modelViewPitchRotationY.RotateDegreesAboutY(rotationDegrees3D.pitchDegreesAboutY);

	Matrix4 modelViewYawRotationZ;
	modelViewYawRotationZ.RotateDegreesAboutZ(rotationDegrees3D.yawDegreesAboutZ);

	//-90 around X, +90 around Z
	//yaw->pitch->roll
	m_rotationMatrix = modelViewYawRotationZ * modelViewPitchRotationY * modelViewRollRotationX;
	//m_rotationMatrix.Transpose();
}

//===========================================================================================================
