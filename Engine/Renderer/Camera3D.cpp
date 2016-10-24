//==============================================================================================================
//Camera3D.cpp
//by Albert Chen Mar-26-2015.
//==============================================================================================================

#include "Camera3D.hpp"
#include "Engine\Math\Vector2.hpp"
#include "Engine\Input\InputSystem.hpp"

Camera3D::Camera3D(const Vector3& position, const EulerAngles& orientation){
	m_position = position;
	m_orientation = orientation;

	//REGISTER_CONSOLE_COMMAND(GET_CAMERA_VIEW_MATRIX);
}

//do fancy stuff here
Matrix4 Camera3D::LookAt(const Vector3& pointToLookAt){

	Vector3 vectorIofCamera;
	Vector3 vectorJofCamera;
	Vector3 vectorKofCamera;
	
	vectorKofCamera = m_position- pointToLookAt;
	vectorKofCamera.Normalize();

	vectorIofCamera = CrossProduct(worldUnitUp, vectorKofCamera);
	vectorIofCamera.Normalize();

	vectorJofCamera = CrossProduct(vectorKofCamera, vectorIofCamera);
	vectorJofCamera.Normalize();

	 Matrix4 cameraLookAtRotation = Matrix4(vectorIofCamera , vectorJofCamera, vectorKofCamera, Vector3(0.0f,0.0f,0.0f ) );

	 return cameraLookAtRotation;
}

//-----------------------------------------------------------------------------------------------------------

std::string Camera3D::ToString(){
	std::string cameraDataText = "";
	cameraDataText += "Camera Position: "+m_position.ToString()+"\n";
	cameraDataText += "Camera Orientation: "+m_orientation.ToVector3().ToString()+"\n";

	return cameraDataText;
}

//-----------------------------------------------------------------------------------------------------------

void Camera3D::UpdateCameraFromInput(float deltaSeconds, const float& moveSpeed){
	const float degreesPerMouseDelta = 0.08f;
	float moveSpeedUnitsPerSecond = moveSpeed;
	const float flySpeedUnitsPerSecond = moveSpeed * 0.5f;
	const float turboMultiplier = 8.0f;
	
	float yawRadians = ConvertDegreesToRadians(m_orientation.yawDegreesAboutZ);
	UNUSED(yawRadians);

	//this allows movement absolute to the world basis
	//move camera forward according to yaw
	Vector3 cameraForwardXY;// = Vector3(cos(yawRadians), sin(yawRadians), 0.0f);

	//define forward
	cameraForwardXY = GetForwardVector();
	cameraForwardXY.Normalize();

	//cameraForwardXY = Vector3::FORWARD;

	//move camera left according to yaw
	Vector3 cameraLeftXY = Vector3(-cameraForwardXY.y, cameraForwardXY.x, 0);
	//move camera right according to yaw
	Vector3 cameraRightXY = -1.0f * cameraLeftXY;
	//move camera backward according to yaw
	Vector3 cameraBackwardXY = -1.0f * cameraForwardXY;

	Vector3 cameraMovementVector(0.0f, 0.0f, 0.0f);

	//move in a direction
	if (theInputSystem->IsKeyDown('W') || theInputSystem->IsKeyDown(VK_UP)){
		cameraMovementVector += cameraForwardXY * moveSpeedUnitsPerSecond * deltaSeconds;
	}
	else if (theInputSystem->IsKeyDown('S') || theInputSystem->IsKeyDown(VK_DOWN)){
		cameraMovementVector += cameraBackwardXY * moveSpeedUnitsPerSecond * deltaSeconds;
	}
	else if (theInputSystem->IsKeyDown('A') || theInputSystem->IsKeyDown(VK_LEFT)){
		cameraMovementVector += cameraLeftXY * moveSpeedUnitsPerSecond * deltaSeconds;
	}
	else if (theInputSystem->IsKeyDown('D') || theInputSystem->IsKeyDown(VK_RIGHT)){
		cameraMovementVector += cameraRightXY * moveSpeedUnitsPerSecond * deltaSeconds;
	}

	//keyboard flight
	else if (theInputSystem->IsKeyDown('Q')){
		cameraMovementVector.z += flySpeedUnitsPerSecond * deltaSeconds;
	}
	else if (theInputSystem->IsKeyDown('E')){
		cameraMovementVector.z -= flySpeedUnitsPerSecond * deltaSeconds;
	}

	//turn on turbo, turbo only increases move speed
	if (theInputSystem->IsKeyDown(VK_SHIFT)){
		cameraMovementVector *= turboMultiplier;
	}

	//adding the combined movement vector to camera pos
	m_position += cameraMovementVector;
	
	Vector2 mouseResetPosition (400.0f , 300.0f );
	Vector2 mousePosition = theInputSystem->GetMousePosition();
	//theInputSystem->ConsolePrintMousePosition();
	Vector2 distanceMouseHasMovedSinceLastFrame = mousePosition - mouseResetPosition;

	if(!theInputSystem->IsKeyDown(VK_CONTROL))
		theInputSystem->SnapMousePosition((int)mouseResetPosition.x, (int)mouseResetPosition.y );

	m_orientation.yawDegreesAboutZ -= distanceMouseHasMovedSinceLastFrame.x * degreesPerMouseDelta * deltaSeconds;
	m_orientation.pitchDegreesAboutY += distanceMouseHasMovedSinceLastFrame.y * degreesPerMouseDelta* deltaSeconds;
	
	//clamp pitch degrees
	if(m_orientation.pitchDegreesAboutY > 89.0f )
		m_orientation.pitchDegreesAboutY = 89.0f;
	if(m_orientation.pitchDegreesAboutY < -89.0f )
		m_orientation.pitchDegreesAboutY = -89.0f;
}

//-----------------------------------------------------------------------------------------------------------

Matrix4 Camera3D::GetCameraRotationMatrix(){
	//get rotation matrix
	Matrix4 camerarollRotationX;
	camerarollRotationX.RotateDegreesAboutX(m_orientation.rollDegreesAboutX);
	Matrix4 camerapitchRotationY;
	camerapitchRotationY.RotateDegreesAboutY(m_orientation.pitchDegreesAboutY);
	Matrix4 camerayawRotationZ;
	camerayawRotationZ.RotateDegreesAboutZ(m_orientation.yawDegreesAboutZ);
	//-90 around X, +90 around Z

	//yaw->pitch->roll
	Matrix4 cameraRotationMatrix = camerayawRotationZ * camerapitchRotationY * camerarollRotationX;

	return cameraRotationMatrix;
}

//-----------------------------------------------------------------------------------------------------------

Matrix4 Camera3D::GetCameraViewMatrix(){
	//get transform matrix
	Matrix4 cameraTransformMatrix;
	cameraTransformMatrix.Translate(m_position);
	cameraTransformMatrix.m_translation.x *= -1.0f; //stopgap fix for -zUP issue
	cameraTransformMatrix.m_translation.z *= -1.0f; //stopgap fix for -zUP issue
	
	Matrix4 viewMatrix = /* GetCameraRotationMatrix() * */ cameraTransformMatrix * GetCameraRotationMatrix();
	//R*T = rotate around origin, movement correct
	//T*R = rotate correct, absolute movement
	viewMatrix.Transpose();
	//the movement is absolute regardless of rotation for whatever reason...

	return viewMatrix;
}

//-----------------------------------------------------------------------------------------------------------

ModelViewMatrix Camera3D::GetCameraModelViewMatrix() {

	ModelViewMatrix outViewMat;
	outViewMat.m_translationMatrix = GetCameraViewMatrix();
	outViewMat.m_rotationMatrix = GetCameraRotationMatrix();

	return outViewMat;
	//return ModelViewMatrix(m_position, Vector3(1,1,1), m_orientation);
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///friend methods


