#include "Vector4.hpp"
//==============================================================================================================
//Matrix4.hpp
//by Albert Chen Feb-27-2015.
//==============================================================================================================
#pragma once

#ifndef _included_Matrix4__
#define _included_Matrix4__

#include "Engine\Math\Vector2.hpp"
#include "Engine\Math\Vector3.hpp"
#include "Engine\Math\Vector4.hpp"

class Matrix4{
public:
	//variables
	Vector4 m_iBasis;
	Vector4 m_jBasis;
	Vector4 m_kBasis;
	Vector4 m_translation;
	//constructors
	Matrix4();
	Matrix4(const Matrix4& copy);
	~Matrix4();
	//overloaded constructors
	//for each of the following (where all non-provided terms should be identity values):
	explicit Matrix4(const Vector2& iBasis, const Vector2& jBasis );
	explicit Matrix4(const Vector2& iBasis, const Vector2& jBasis, const Vector2& translation);
	explicit Matrix4(const Vector3& iBasis, const Vector3& jBasis, const Vector3& kBasis);
	explicit Matrix4(const Vector3& iBasis, const Vector3& jBasis, const Vector3& kBasis, const Vector3& translation);
	explicit Matrix4(const Vector4& iBasis, const Vector4& jBasis, const Vector4& kBasis, const Vector4& translation);
	//operators
	const Matrix4& operator=(const Matrix4& vectorToAssign);
	//manipulators
	Matrix4 MakeIdentity();

	const Matrix4 operator*(const Matrix4& matrixToTransformWith) const;
	void Transform(const Matrix4& matrixToTransformWith );
	const Matrix4 Matrix4::GetTransformed( const Matrix4& newTransformToApply ) const;
	//accessors
	const float* GetAsFloatArray() const;
	float* GetAsFloatArray();
	//transform methods
	Vector2 TransformPosition( const Vector2& position2D ) const; // Hard coded to assume z=0, w=1
	Vector3 TransformPosition( const Vector3& position3D ) const; // Hard coded to assume w=1
	Vector2 TransformDirection( const Vector2& direction2D ) const; // Hard coded to assume z=0, w=0
	Vector3 TransformDirection( const Vector3& direction3D ) const; // Hard coded to assume w=0
	Vector4 TransformVector( const Vector4& homogeneousVector ) const;
	//modifiers
	Matrix4 Transpose();
	void Translate( const Vector2& translation2D ); // z translation assumed to be 0
	void Translate( const Vector3& translation3D );
	void Scale( float uniformScale );
	void Scale( const Vector2& nonUniformScale2D ); // z scale assumed to be 1
	void Scale( const Vector3& nonUniformScale3D );
	void SetScale(float uniformScale);
	void RotateDegreesAboutX(float degrees);
	void RotateDegreesAboutY( float degrees );
	void RotateDegreesAboutZ( float degrees ); // Possibly also offered as RotateDegrees2D
	void RotateDegrees2D(float degrees );
	void RotateRadiansAboutX( float radians );
	void RotateRadiansAboutY( float radians );
	void RotateRadiansAboutZ( float radians );
	void RotateRadians2D(float radians );
	std::string ToString();
	//static creator methods
	static Matrix4 CreateTranslation( const Vector2& translation2D ); // z translation assumed to be 0
	static Matrix4 CreateTranslation( const Vector3& translation3D );
	static Matrix4 CreateScale( float uniformScale );
	static Matrix4 CreateScale( const Vector2& nonUniformScale2D ); // z scale assumed to be 1
	static Matrix4 CreateScale( const Vector3& nonUniformScale3D );
	static Matrix4 CreateRotationDegreesAboutX( float degrees );
	static Matrix4 CreateRotationDegreesAboutY( float degrees );
	static Matrix4 CreateRotationDegreesAboutZ( float degrees ); // a.k.a. CreateRotationDegrees2D
	static Matrix4 CreateRotationRadiansAboutX( float radians );
	static Matrix4 CreateRotationRadiansAboutY( float radians );
	static Matrix4 CreateRotationRadiansAboutZ( float radians ); // a.k.a. CreateRotationRadians2D

	//friend methods
	//friend const Vector4 operator*(const Vector4& vec4ToTransform, const Matrix4& matrixToTransformWith);
};

typedef std::vector<Matrix4> Matrices;


static const Matrix4 IDENTITY_MATRIX = Matrix4();

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods
///----------------------------------------------------------------------------------------------------------
///constructors
inline Matrix4::Matrix4(){
	//set matrix to identity
	m_iBasis =      Vector4(1.0f,0.0f,0.0f,0.0f);
	m_jBasis =      Vector4(0.0f,1.0f,0.0f,0.0f);
	m_kBasis =      Vector4(0.0f,0.0f,1.0f,0.0f);
	m_translation = Vector4(0.0f,0.0f,0.0f,1.0f);
}

inline Matrix4::Matrix4(const Matrix4& copy){
	m_iBasis = copy.m_iBasis;
	m_jBasis = copy.m_jBasis;
	m_kBasis = copy.m_kBasis;
	m_translation = copy.m_translation;
}

inline Matrix4::~Matrix4(){
	//do nothing
}
///----------------------------------------------------------------------------------------------------------
///manipulators
inline Matrix4 Matrix4::MakeIdentity(){
	//set matrix to identity
	m_iBasis =      Vector4(1.0f,0.0f,0.0f,0.0f);
	m_jBasis =      Vector4(0.0f,1.0f,0.0f,0.0f);
	m_kBasis =      Vector4(0.0f,0.0f,1.0f,0.0f);
	m_translation = Vector4(0.0f,0.0f,0.0f,1.0f);

	return (*this);
}
///----------------------------------------------------------------------------------------------------------
///accessors

inline std::string Matrix4::ToString(){
	std::string matrixString = "";
	matrixString += m_iBasis.ToString()+"\n";
	matrixString += m_jBasis.ToString() + "\n";
	matrixString += m_kBasis.ToString() + "\n";
	matrixString += m_translation.ToString() + "\n";

	return matrixString;
}

inline const float* Matrix4::GetAsFloatArray() const{
	return &m_iBasis.x;
}
inline float* Matrix4::GetAsFloatArray(){
	//the float array is formatted to work with glLoadMatrix

	//return matrixAsArray;

	return &m_iBasis.x;
}
///----------------------------------------------------------------------------------------------------------
///manipulators
inline void Matrix4::RotateDegrees2D(float degrees ){
	RotateDegreesAboutZ( degrees );
}

inline void Matrix4::RotateRadians2D(float radians ){
	RotateRadiansAboutZ(radians);
}

///----------------------------------------------------------------------------------------------------------
///friend methods



#endif