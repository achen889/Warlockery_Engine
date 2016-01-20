//==============================================================================================================
//Matrix4.cpp
//by Albert Chen Feb-27-2015.
//==============================================================================================================

#include "Matrix4.hpp"

///----------------------------------------------------------------------------------------------------------
///explicit constructors

Matrix4::Matrix4(const Vector2& iBasis, const Vector2& jBasis ){
	m_iBasis =      Vector4(iBasis.x,iBasis.y,0.0f,0.0f);
	m_jBasis =      Vector4(jBasis.x,jBasis.y,0.0f,0.0f);
	m_kBasis =      Vector4(0.0f,0.0f,1.0f,0.0f);
	m_translation = Vector4(0.0f,0.0f,0.0f,1.0f);
}
Matrix4::Matrix4(const Vector2& iBasis, const Vector2& jBasis, const Vector2& translation){
	m_iBasis =      Vector4(iBasis.x,iBasis.y,0.0f,0.0f);
	m_jBasis =      Vector4(jBasis.x,jBasis.y,0.0f,0.0f);
	m_kBasis =      Vector4(0.0f,0.0f,1.0f,0.0f);
	m_translation = Vector4(translation.x,translation.y,0.0f,1.0f);
}
Matrix4::Matrix4(const Vector3& iBasis, const Vector3& jBasis, const Vector3& kBasis){
	m_iBasis =      Vector4(iBasis.x,iBasis.y,iBasis.z,0.0f);
	m_jBasis =      Vector4(jBasis.x,jBasis.y,jBasis.z,0.0f);
	m_kBasis =      Vector4(kBasis.x,kBasis.y,kBasis.z,0.0f);
	m_translation = Vector4(0.0f,0.0f,0.0f,1.0f);
}

Matrix4::Matrix4(const Vector3& iBasis, const Vector3& jBasis, const Vector3& kBasis, const Vector3& translation){
	m_iBasis =      Vector4(iBasis.x,iBasis.y,iBasis.z,0.0f);
	m_jBasis =      Vector4(jBasis.x,jBasis.y,jBasis.z,0.0f);
	m_kBasis =      Vector4(kBasis.x,kBasis.y,kBasis.z,0.0f);
	m_translation = Vector4(translation.x,translation.y,translation.z,1.0f);
}

Matrix4::Matrix4(const Vector4& iBasis, const Vector4& jBasis, const Vector4& kBasis, const Vector4& translation){
	m_iBasis =		iBasis;
	m_jBasis =		jBasis;
	m_kBasis =		kBasis;
	m_translation = translation;
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///transform methods, mult this matrix by something

//this rows x other columns matrix multiplication
const Matrix4 Matrix4::operator*(const Matrix4& matrixToTransformWith) const{
	//
	Vector4 colOfXBases = Vector4(matrixToTransformWith.m_iBasis.x, matrixToTransformWith.m_jBasis.x, matrixToTransformWith.m_kBasis.x, matrixToTransformWith.m_translation.x);
	Vector4 colOfYBases = Vector4(matrixToTransformWith.m_iBasis.y, matrixToTransformWith.m_jBasis.y, matrixToTransformWith.m_kBasis.y, matrixToTransformWith.m_translation.y);
	Vector4 colOfZBases = Vector4(matrixToTransformWith.m_iBasis.z, matrixToTransformWith.m_jBasis.z, matrixToTransformWith.m_kBasis.z, matrixToTransformWith.m_translation.z);
	Vector4 colOfWBases = Vector4(matrixToTransformWith.m_iBasis.w, matrixToTransformWith.m_jBasis.w, matrixToTransformWith.m_kBasis.w, matrixToTransformWith.m_translation.w);

	Matrix4 concatenated;
	concatenated.m_iBasis.x = DotProduct( m_iBasis , colOfXBases );
	concatenated.m_iBasis.y = DotProduct( m_iBasis , colOfYBases );
	concatenated.m_iBasis.z = DotProduct( m_iBasis , colOfZBases );
	concatenated.m_iBasis.w = DotProduct( m_iBasis , colOfWBases );

	concatenated.m_jBasis.x = DotProduct( m_jBasis , colOfXBases );
	concatenated.m_jBasis.y = DotProduct( m_jBasis , colOfYBases );
	concatenated.m_jBasis.z = DotProduct( m_jBasis , colOfZBases );
	concatenated.m_jBasis.w = DotProduct( m_jBasis , colOfWBases );

	concatenated.m_kBasis.x = DotProduct( m_kBasis , colOfXBases );
	concatenated.m_kBasis.y = DotProduct( m_kBasis , colOfYBases );
	concatenated.m_kBasis.z = DotProduct( m_kBasis , colOfZBases );
	concatenated.m_kBasis.w = DotProduct( m_kBasis , colOfWBases );

	concatenated.m_translation.x = DotProduct( m_translation , colOfXBases );
	concatenated.m_translation.y = DotProduct( m_translation , colOfYBases );
	concatenated.m_translation.z = DotProduct( m_translation , colOfZBases );
	concatenated.m_translation.w = DotProduct( m_translation , colOfWBases );

	return concatenated;
}

void Matrix4::Transform(const Matrix4& matrixToTransformWith ){
	Vector4 rowOfXBases = Vector4(m_iBasis.x, m_jBasis.x, m_kBasis.x, m_translation.x);
	Vector4 rowOfYBases = Vector4(m_iBasis.y, m_jBasis.y, m_kBasis.y, m_translation.y);
	Vector4 rowOfZBases = Vector4(m_iBasis.z, m_jBasis.z, m_kBasis.z, m_translation.z);
	Vector4 rowOfWBases = Vector4(m_iBasis.w, m_jBasis.w, m_kBasis.w, m_translation.w);

	Matrix4 concatenated;
	concatenated.m_iBasis.x = DotProduct( rowOfXBases , matrixToTransformWith.m_iBasis);
	concatenated.m_jBasis.x = DotProduct( rowOfXBases , matrixToTransformWith.m_jBasis);
	concatenated.m_kBasis.x = DotProduct( rowOfXBases , matrixToTransformWith.m_kBasis);
	concatenated.m_translation.x = DotProduct( rowOfXBases , matrixToTransformWith.m_translation);

	concatenated.m_iBasis.y = DotProduct( rowOfYBases , matrixToTransformWith.m_iBasis);
	concatenated.m_jBasis.y = DotProduct( rowOfYBases , matrixToTransformWith.m_jBasis);
	concatenated.m_kBasis.y = DotProduct( rowOfYBases , matrixToTransformWith.m_kBasis);
	concatenated.m_translation.y = DotProduct( rowOfYBases , matrixToTransformWith.m_translation);

	concatenated.m_iBasis.z = DotProduct( rowOfZBases , matrixToTransformWith.m_iBasis);
	concatenated.m_jBasis.z = DotProduct( rowOfZBases , matrixToTransformWith.m_jBasis);
	concatenated.m_kBasis.z = DotProduct( rowOfZBases , matrixToTransformWith.m_kBasis);
	concatenated.m_translation.z = DotProduct( rowOfZBases , matrixToTransformWith.m_translation);

	concatenated.m_iBasis.w = DotProduct( rowOfZBases , matrixToTransformWith.m_iBasis);
	concatenated.m_jBasis.w = DotProduct( rowOfZBases , matrixToTransformWith.m_jBasis);
	concatenated.m_kBasis.w = DotProduct( rowOfZBases , matrixToTransformWith.m_kBasis);
	concatenated.m_translation.w = DotProduct( rowOfZBases , matrixToTransformWith.m_translation);
	
	//set my vector 4's to the right values
	m_iBasis = concatenated.m_iBasis;
	m_jBasis = concatenated.m_jBasis;
	m_kBasis = concatenated.m_kBasis;
	m_translation = concatenated.m_translation;
	
}

const Matrix4 Matrix4::GetTransformed( const Matrix4& newTransformToApply ) const {
	Vector4 rowOfXBases = Vector4(m_iBasis.x, m_jBasis.x, m_kBasis.x, m_translation.x);
	Vector4 rowOfYBases = Vector4(m_iBasis.y, m_jBasis.y, m_kBasis.y, m_translation.y);
	Vector4 rowOfZBases = Vector4(m_iBasis.z, m_jBasis.z, m_kBasis.z, m_translation.z);
	Vector4 rowOfWBases = Vector4(m_iBasis.w, m_jBasis.w, m_kBasis.w, m_translation.w);

	Matrix4 concatenated;
	concatenated.m_iBasis.x = DotProduct( rowOfXBases , newTransformToApply.m_iBasis);
	concatenated.m_jBasis.x = DotProduct( rowOfXBases , newTransformToApply.m_jBasis);
	concatenated.m_kBasis.x = DotProduct( rowOfXBases , newTransformToApply.m_kBasis);
	concatenated.m_translation.x = DotProduct( rowOfXBases , newTransformToApply.m_translation);

	concatenated.m_iBasis.y = DotProduct( rowOfYBases , newTransformToApply.m_iBasis);
	concatenated.m_jBasis.y = DotProduct( rowOfYBases , newTransformToApply.m_jBasis);
	concatenated.m_kBasis.y = DotProduct( rowOfYBases , newTransformToApply.m_kBasis);
	concatenated.m_translation.y = DotProduct( rowOfYBases , newTransformToApply.m_translation);

	concatenated.m_iBasis.z = DotProduct( rowOfZBases , newTransformToApply.m_iBasis);
	concatenated.m_jBasis.z = DotProduct( rowOfZBases , newTransformToApply.m_jBasis);
	concatenated.m_kBasis.z = DotProduct( rowOfZBases , newTransformToApply.m_kBasis);
	concatenated.m_translation.z = DotProduct( rowOfZBases , newTransformToApply.m_translation);

	concatenated.m_iBasis.w = DotProduct( rowOfZBases , newTransformToApply.m_iBasis);
	concatenated.m_jBasis.w = DotProduct( rowOfZBases , newTransformToApply.m_jBasis);
	concatenated.m_kBasis.w = DotProduct( rowOfZBases , newTransformToApply.m_kBasis);
	concatenated.m_translation.w = DotProduct( rowOfZBases , newTransformToApply.m_translation);

	return concatenated;
}

Vector2 Matrix4::TransformPosition( const Vector2& position2D ) const{
	Vector4 positionToTransform = Vector4(position2D.x, position2D.y, 0.0f, 1.0f);

	Vector4 rowOfXBases = Vector4(m_iBasis.x, m_jBasis.x, m_kBasis.x, m_translation.x);
	Vector4 rowOfYBases = Vector4(m_iBasis.y, m_jBasis.y, m_kBasis.y, m_translation.y);

	Vector2 positionAfterTransform;
	positionAfterTransform.x = DotProduct(rowOfXBases, positionToTransform);
	positionAfterTransform.y = DotProduct(rowOfYBases, positionToTransform);

	return positionAfterTransform;

}

Vector3 Matrix4::TransformPosition( const Vector3& position3D ) const{
	Vector4 positionToTransform = Vector4(position3D.x, position3D.y, position3D.z, 1.0f);

	Vector4 rowOfXBases = Vector4(m_iBasis.x, m_jBasis.x, m_kBasis.x, m_translation.x);
	Vector4 rowOfYBases = Vector4(m_iBasis.y, m_jBasis.y, m_kBasis.y, m_translation.y);
	Vector4 rowOfZBases = Vector4(m_iBasis.z, m_jBasis.z, m_kBasis.z, m_translation.z);

	Vector3 positionAfterTransform;
	positionAfterTransform.x = DotProduct(rowOfXBases, positionToTransform);
	positionAfterTransform.y = DotProduct(rowOfYBases, positionToTransform);
	positionAfterTransform.z = DotProduct(rowOfZBases, positionToTransform);

	return positionAfterTransform;
}

Vector2 Matrix4::TransformDirection( const Vector2& direction2D ) const{
	Vector4 directionToTransform = Vector4(direction2D.x, direction2D.y, 0.0f, 0.0f);

	Vector4 rowOfXBases = Vector4(m_iBasis.x, m_jBasis.x, m_kBasis.x, m_translation.x);
	Vector4 rowOfYBases = Vector4(m_iBasis.y, m_jBasis.y, m_kBasis.y, m_translation.y);

	Vector2 directionAfterTransform;
	directionAfterTransform.x = DotProduct(rowOfXBases, directionToTransform);
	directionAfterTransform.y = DotProduct(rowOfYBases, directionToTransform);

	return directionAfterTransform;

}

Vector3 Matrix4::TransformDirection( const Vector3& direction3D ) const{
	Vector4 directionToTransform = Vector4(direction3D.x, direction3D.y, direction3D.z, 0.0f);

	Vector4 rowOfXBases = Vector4(m_iBasis.x, m_jBasis.x, m_kBasis.x, m_translation.x);
	Vector4 rowOfYBases = Vector4(m_iBasis.y, m_jBasis.y, m_kBasis.y, m_translation.y);
	Vector4 rowOfZBases = Vector4(m_iBasis.z, m_jBasis.z, m_kBasis.z, m_translation.z);

	Vector3 directionAfterTransform;
	directionAfterTransform.x = DotProduct(rowOfXBases, directionToTransform);
	directionAfterTransform.y = DotProduct(rowOfYBases, directionToTransform);
	directionAfterTransform.z = DotProduct(rowOfZBases, directionToTransform);

	return directionAfterTransform;
}

Vector4 Matrix4::TransformVector( const Vector4& homogeneousVector ) const{
	Vector4 rowOfXBases = Vector4(m_iBasis.x, m_jBasis.x, m_kBasis.x, m_translation.x);
	Vector4 rowOfYBases = Vector4(m_iBasis.y, m_jBasis.y, m_kBasis.y, m_translation.y);
	Vector4 rowOfZBases = Vector4(m_iBasis.z, m_jBasis.z, m_kBasis.z, m_translation.z);
	Vector4 rowOfWBases = Vector4(m_iBasis.w, m_jBasis.w, m_kBasis.w, m_translation.w);

	Vector4 resultOfTransform;
	resultOfTransform.x = DotProduct(rowOfXBases, homogeneousVector);
	resultOfTransform.y = DotProduct(rowOfYBases, homogeneousVector);
	resultOfTransform.z = DotProduct(rowOfZBases, homogeneousVector);
	resultOfTransform.w = DotProduct(rowOfWBases, homogeneousVector);

	return resultOfTransform;
}
//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///modifiers
Matrix4 Matrix4::Transpose(){
	Vector4 rowOfXBases = Vector4(m_iBasis.x, m_jBasis.x, m_kBasis.x, m_translation.x);
	Vector4 rowOfYBases = Vector4(m_iBasis.y, m_jBasis.y, m_kBasis.y, m_translation.y);
	Vector4 rowOfZBases = Vector4(m_iBasis.z, m_jBasis.z, m_kBasis.z, m_translation.z);
	Vector4 rowOfWBases = Vector4(m_iBasis.w, m_jBasis.w, m_kBasis.w, m_translation.w);
	
	m_iBasis = rowOfXBases;
	m_jBasis = rowOfYBases;
	m_kBasis = rowOfZBases;
	m_translation = rowOfWBases;

	return *this;
}

void Matrix4::Translate( const Vector2& translation2D ){
	// z translation assumed to be 0
	m_translation.x += translation2D.x;
	m_translation.y += translation2D.y;
	
}

void Matrix4::Translate( const Vector3& translation3D ){
	m_translation.x += translation3D.x;
	m_translation.y += translation3D.y;
	m_translation.z += translation3D.z;

}
///----------------------------------------------------------------------------------------------------------
///scaling methods
void Matrix4::Scale( float uniformScale ){
	m_iBasis *= uniformScale;
	m_jBasis *= uniformScale;
	m_kBasis *= uniformScale;
	//m_translation *= uniformScale;
}

void Matrix4::Scale( const Vector2& nonUniformScale2D ){
	// z scale assumed to be 1
	m_iBasis.x *= nonUniformScale2D.x;
	m_jBasis.x *= nonUniformScale2D.x;
	m_kBasis.x *= nonUniformScale2D.x;

	m_iBasis.y *= nonUniformScale2D.y;
	m_jBasis.y *= nonUniformScale2D.y;
	m_kBasis.y *= nonUniformScale2D.y;
	
}

void Matrix4::Scale( const Vector3& nonUniformScale3D ){
	// z scale assumed to be 1
	m_iBasis.x *= nonUniformScale3D.x;
	m_jBasis.x *= nonUniformScale3D.x;
	m_kBasis.x *= nonUniformScale3D.x;

	m_iBasis.y *= nonUniformScale3D.y;
	m_jBasis.y *= nonUniformScale3D.y;
	m_kBasis.y *= nonUniformScale3D.y;

	m_iBasis.z *= nonUniformScale3D.z;
	m_jBasis.z *= nonUniformScale3D.z;
	m_kBasis.z *= nonUniformScale3D.z;

}

void Matrix4::SetScale(float uniformScale){
	m_iBasis = Vector4(uniformScale, 0.0f, 0.0f, 0.0f);
	m_jBasis = Vector4(0.0f, uniformScale, 0.0f, 0.0f);
	m_kBasis = Vector4(0.0f, 0.0f, uniformScale, 0.0f);
	//m_translation *= uniformScale;
}


///----------------------------------------------------------------------------------------------------------
///rotate methods

void Matrix4::RotateDegreesAboutX( float degrees ){
	float CosineOfDegrees = cos(degrees);
	float SineOfDegrees = sin(degrees);

	Vector4 tempJ = m_jBasis * CosineOfDegrees - m_kBasis * SineOfDegrees;
	Vector4 tempK = m_kBasis * CosineOfDegrees + m_jBasis * SineOfDegrees;

	m_jBasis = tempJ;
	m_kBasis = tempK;

// 	m_jBasis.y *= CosineOfDegrees;
// 	m_jBasis.z *= SineOfDegrees;
// 
// 	m_kBasis.y *= -1.0f * SineOfDegrees;
// 	m_kBasis.z *= CosineOfDegrees;
}

void Matrix4::RotateDegreesAboutY( float degrees ){
	float CosineOfDegrees = cos(degrees);
	float SineOfDegrees = sin(degrees);

	Vector4 tempI = m_iBasis * CosineOfDegrees + m_kBasis * SineOfDegrees;
	Vector4 tempK = m_kBasis * CosineOfDegrees - m_iBasis * SineOfDegrees;

	m_iBasis = tempI;
	m_kBasis = tempK;

// 	m_iBasis.x *= CosineOfDegrees;
// 	m_iBasis.z *= SineOfDegrees;
// 
// 	m_kBasis.x *= -1.0f * SineOfDegrees;
// 	m_kBasis.z *= CosineOfDegrees;
}

void Matrix4::RotateDegreesAboutZ( float degrees ){
	float CosineOfDegrees = cos(degrees);
	float SineOfDegrees = sin(degrees);

	Vector4 tempI = m_iBasis * CosineOfDegrees + m_jBasis * SineOfDegrees;
	Vector4 tempJ = m_jBasis * CosineOfDegrees - m_iBasis * SineOfDegrees;

	m_iBasis = tempI;
	m_jBasis = tempJ;

// 	m_iBasis.x *= CosineOfDegrees;
// 	m_iBasis.y *= SineOfDegrees;
// 
// 	m_jBasis.x *= -1.0f * SineOfDegrees;
// 	m_jBasis.y *= CosineOfDegrees;

}// Possibly also offered as RotateDegrees2D

void Matrix4::RotateRadiansAboutX( float radians ){
	float degreesFromRadians = radians * DegreesPerRadian;

	RotateDegreesAboutX(degreesFromRadians);
}

void Matrix4::RotateRadiansAboutY( float radians ){
	float degreesFromRadians = radians * DegreesPerRadian;

	RotateDegreesAboutY(degreesFromRadians);
}

void Matrix4::RotateRadiansAboutZ( float radians ){
	float degreesFromRadians = radians * DegreesPerRadian;

	RotateDegreesAboutZ(degreesFromRadians);
}

///----------------------------------------------------------------------------------------------------------
///static creator methods
Matrix4 Matrix4::CreateTranslation( const Vector2& translation2D ){
	Matrix4 newTranslation2D = Matrix4();
	newTranslation2D.m_translation.x = translation2D.x;
	newTranslation2D.m_translation.y = translation2D.y;

	return newTranslation2D;
}// z translation assumed to be 0

Matrix4 Matrix4::CreateTranslation( const Vector3& translation3D ){
	Matrix4 newTranslation3D = Matrix4();
	newTranslation3D.m_translation.x = translation3D.x;
	newTranslation3D.m_translation.y = translation3D.y;
	newTranslation3D.m_translation.z = translation3D.z;

	return newTranslation3D;
}

Matrix4 Matrix4::CreateScale( float uniformScale ){
	Matrix4 newUniformScale = Matrix4();
	newUniformScale.m_iBasis *= uniformScale;
	newUniformScale.m_jBasis *= uniformScale;
	newUniformScale.m_kBasis *= uniformScale;
	newUniformScale.m_translation *= uniformScale;

	return newUniformScale;
}

Matrix4 Matrix4::CreateScale( const Vector2& nonUniformScale2D ){
	Matrix4 newNonUniformScaleFor2D = Matrix4();
	newNonUniformScaleFor2D.m_iBasis.x *= nonUniformScale2D.x;
	newNonUniformScaleFor2D.m_jBasis.x *= nonUniformScale2D.x;
	newNonUniformScaleFor2D.m_kBasis.x *= nonUniformScale2D.x;

	newNonUniformScaleFor2D.m_iBasis.y *= nonUniformScale2D.y;
	newNonUniformScaleFor2D.m_jBasis.y *= nonUniformScale2D.y;
	newNonUniformScaleFor2D.m_kBasis.y *= nonUniformScale2D.y;

	return newNonUniformScaleFor2D;
} // z scale assumed to be 1

Matrix4 Matrix4::CreateScale( const Vector3& nonUniformScale3D ){
	Matrix4 newNonUniformScaleFor3D = Matrix4();
	newNonUniformScaleFor3D.m_iBasis.x *= nonUniformScale3D.x;
	newNonUniformScaleFor3D.m_jBasis.x *= nonUniformScale3D.x;
	newNonUniformScaleFor3D.m_kBasis.x *= nonUniformScale3D.x;

	newNonUniformScaleFor3D.m_iBasis.y *= nonUniformScale3D.y;
	newNonUniformScaleFor3D.m_jBasis.y *= nonUniformScale3D.y;
	newNonUniformScaleFor3D.m_kBasis.y *= nonUniformScale3D.y;

	newNonUniformScaleFor3D.m_iBasis.z *= nonUniformScale3D.z;
	newNonUniformScaleFor3D.m_jBasis.z *= nonUniformScale3D.z;
	newNonUniformScaleFor3D.m_kBasis.z *= nonUniformScale3D.z;

	return newNonUniformScaleFor3D;
}

Matrix4 Matrix4::CreateRotationDegreesAboutX( float degrees ){
	Matrix4 newRotationDegreesAboutX;

	float CosineOfDegrees = cos(degrees);
	float SineOfDegrees = sin(degrees);

	newRotationDegreesAboutX.m_jBasis.y *= CosineOfDegrees;
	newRotationDegreesAboutX.m_jBasis.z *= SineOfDegrees;

	newRotationDegreesAboutX.m_kBasis.y *= -1.0f * SineOfDegrees;
	newRotationDegreesAboutX.m_kBasis.z *= CosineOfDegrees;

	return newRotationDegreesAboutX;
}

Matrix4 Matrix4::CreateRotationDegreesAboutY( float degrees ){
	Matrix4 newRotationDegreesAboutY;

	float CosineOfDegrees = cos(degrees);
	float SineOfDegrees = sin(degrees);

	newRotationDegreesAboutY.m_iBasis.x *= CosineOfDegrees;
	newRotationDegreesAboutY.m_iBasis.z *= SineOfDegrees;

	newRotationDegreesAboutY.m_kBasis.x *= -1.0f * SineOfDegrees;
	newRotationDegreesAboutY.m_kBasis.z *= CosineOfDegrees;

	return newRotationDegreesAboutY;
}

Matrix4 Matrix4::CreateRotationDegreesAboutZ( float degrees ){
	Matrix4 newRotationDegreesAboutZ;

	float CosineOfDegrees = cos(degrees);
	float SineOfDegrees = sin(degrees);

	newRotationDegreesAboutZ.m_iBasis.x *= CosineOfDegrees;
	newRotationDegreesAboutZ.m_iBasis.y *= SineOfDegrees;

	newRotationDegreesAboutZ.m_jBasis.x *= -1.0f * SineOfDegrees;
	newRotationDegreesAboutZ.m_jBasis.y *= CosineOfDegrees;

	return newRotationDegreesAboutZ;
} // a.k.a. CreateRotationDegrees2D

Matrix4 Matrix4::CreateRotationRadiansAboutX( float radians ){
	float degreesFromRadians = radians * DegreesPerRadian;

	return CreateRotationDegreesAboutX(degreesFromRadians);
}

Matrix4 Matrix4::CreateRotationRadiansAboutY( float radians ){
	float degreesFromRadians = radians * DegreesPerRadian;

	return CreateRotationDegreesAboutY(degreesFromRadians);
}
Matrix4 Matrix4::CreateRotationRadiansAboutZ( float radians ){
	float degreesFromRadians = radians * DegreesPerRadian;

	return CreateRotationDegreesAboutZ(degreesFromRadians);
} // a.k.a. CreateRotationRadians2D

const Matrix4& Matrix4::operator=(const Matrix4& vectorToAssign){
	m_iBasis = vectorToAssign.m_iBasis;
	m_jBasis = vectorToAssign.m_jBasis;
	m_kBasis = vectorToAssign.m_kBasis;
	m_translation = vectorToAssign.m_translation;

	return *this;
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///friend methods
// 
// //rows of matrix times the column of the vector4
// const Vector4 operator*(const Vector4& vec4ToTransform, const Matrix4& matrixToTransformWith ){
// 
// 	//treat vector as column of vals
// 	float newX = matrixToTransformWith.m_iBasis.x * vec4ToTransform.x +
// 		matrixToTransformWith.m_iBasis.y * vec4ToTransform.y +
// 		matrixToTransformWith.m_iBasis.z * vec4ToTransform.z +
// 		matrixToTransformWith.m_iBasis.w * vec4ToTransform.w;
// 
// 	float newY = matrixToTransformWith.m_jBasis.x * vec4ToTransform.x +
// 		matrixToTransformWith.m_jBasis.y * vec4ToTransform.y +
// 		matrixToTransformWith.m_jBasis.z * vec4ToTransform.z +
// 		matrixToTransformWith.m_jBasis.w * vec4ToTransform.w;
// 
// 	float newZ = matrixToTransformWith.m_kBasis.x * vec4ToTransform.x +
// 		matrixToTransformWith.m_kBasis.y * vec4ToTransform.y +
// 		matrixToTransformWith.m_kBasis.z * vec4ToTransform.z +
// 		matrixToTransformWith.m_kBasis.w * vec4ToTransform.w;
// 	
// 
// 	float wVal = matrixToTransformWith.m_translation.x * vec4ToTransform.x +
// 		matrixToTransformWith.m_translation.y * vec4ToTransform.y +
// 		matrixToTransformWith.m_translation.z * vec4ToTransform.z +
// 		matrixToTransformWith.m_translation.w * vec4ToTransform.w;
// 
// 	return Vector4(newX, newY, newZ, wVal);
// 
// }

//===========================================================================================================

