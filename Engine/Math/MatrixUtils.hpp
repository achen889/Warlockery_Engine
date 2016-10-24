//==============================================================================================================
//MatrixUtils.hpp
//by Albert Chen Jul-4-2015.
//==============================================================================================================

#pragma once

#include "Engine/Math/Matrix4.hpp"
#include "Engine/Math/EulerAngles.hpp"

struct ModelViewMatrix{
	//matrices
	Matrix4 m_translationMatrix;
	Matrix4 m_rotationMatrix;
	Matrix4 m_scaleMatrix;

	//methods

	//constructor
	ModelViewMatrix();
	~ModelViewMatrix();

	ModelViewMatrix(const Vector3& translation, const Vector3& scale, const EulerAngles& rotationDegrees3D);

	//translate
	void SetTranslation3D(const Vector3& translation);
	void SetTranslation2D(const Vector2& translation);

	//Rotate
	void ZeroRotation() { m_rotationMatrix = IDENTITY_MATRIX; }
	void SetRotationDegrees3D(const EulerAngles& rotationDegrees3D);
	void SetRotationDegrees2D(const float& degrees2D);

	//scale
	void SetScale(const Vector3& scale3D);
	void SetScale(const Vector2& scale2D);
	void SetScale(const float& scaleUniform);
	
	//set basis
	void SetBasisTransformation(const Matrix4& basis);

	Matrix4 GetModelViewTransformMatrix();

};

//===========================================================================================================
//inline methods

//constructors
inline ModelViewMatrix::ModelViewMatrix(){
	//do nothing;
}

inline ModelViewMatrix::~ModelViewMatrix(){
	//do nothing
}

//-----

inline ModelViewMatrix::ModelViewMatrix(const Vector3& translation, const Vector3& scale, const EulerAngles& rotationDegrees3D){

	SetTranslation3D(translation);
	SetScale(scale);
	SetRotationDegrees3D(rotationDegrees3D);
}

//-----------------------------------------------------------------------------------------------------------

inline void ModelViewMatrix::SetTranslation3D(const Vector3& translation){
	m_translationMatrix.Translate(translation);
}

//-----------------------------------------------------------------------------------------------------------

inline void ModelViewMatrix::SetTranslation2D(const Vector2& translation){
	m_translationMatrix.Translate(translation);
}


//-----------------------------------------------------------------------------------------------------------

inline void ModelViewMatrix::SetRotationDegrees2D(const float& degrees2D){
	
	//m_rotationMatrix = Matrix4::CreateRotationDegreesAboutZ(degrees2D); //causes silly things with bullets
	m_rotationMatrix.RotateDegrees2D(degrees2D); //causes bullets to swirl

}

//-----------------------------------------------------------------------------------------------------------

inline void ModelViewMatrix::SetScale(const Vector3& scale3D){
	m_scaleMatrix.Scale(scale3D);
}

//-----------------------------------------------------------------------------------------------------------

inline void ModelViewMatrix::SetScale(const Vector2& scale2D){
	m_scaleMatrix.Scale(scale2D);
}

//-----------------------------------------------------------------------------------------------------------

inline void ModelViewMatrix::SetScale(const float& scaleUniform){
	m_scaleMatrix.SetScale(scaleUniform);// Scale(scaleUniform);
}

//-----------------------------------------------------------------------------------------------------------

inline void ModelViewMatrix::SetBasisTransformation(const Matrix4& basis){
	m_scaleMatrix = basis;
}

//-----------------------------------------------------------------------------------------------------------

inline Matrix4 ModelViewMatrix::GetModelViewTransformMatrix(){
	Matrix4 modelViewTransform;

	modelViewTransform = m_scaleMatrix * m_rotationMatrix * m_translationMatrix;

	//modelViewTransform = m_translationMatrix * m_rotationMatrix * m_scaleMatrix;
	
	return modelViewTransform;
}
