//==============================================================================================================
//PhysicsMotion2D.hpp
//by Albert Chen Jan-22-2015.
//==============================================================================================================

#ifndef _included_PhysicsMotion2D__
#define _included_PhysicsMotion2D__

#include "EngineMath.hpp"

class PhysicsMotion2D{
public:
	Vector2 m_position;
	Vector2 m_velocityUnitsPerSecond;
	Vector2 m_accelerationUnitsPerSecondSquared;
	float m_orientationDegrees;
	float m_angularVelocityDegreesPerSecond;

	PhysicsMotion2D();
	const Vector2& UpdatePositionWithVelocity(double deltaSeconds);
	const Vector2& UpdateVelocity(const Vector2& deltaVelocity );
	const float UpdateAngularVelocity(double deltaSeconds);
	void Update(double deltaSeconds );
	void ZeroThisPhysicsController();

	friend void ConsolePrintPhysicsMotion2D(const PhysicsMotion2D& consolePhysicsMotion2D);
};

///----------------------------------------------------------------------------------------------------------
///constructor
inline PhysicsMotion2D::PhysicsMotion2D(){
	//do nothing
}

inline void PhysicsMotion2D::ZeroThisPhysicsController(){
	m_orientationDegrees = 0.0;
	m_velocityUnitsPerSecond = Vector2( 0.0f, 0.0f );
	m_angularVelocityDegreesPerSecond = 0.0f;
	m_accelerationUnitsPerSecondSquared = Vector2( 0.0f, 0.0f );
}

inline void ConsolePrintPhysicsMotion2D(const PhysicsMotion2D& consolePhysicsMotion2D){
	ConsolePrintf("| PhysicsMotion2D |\n| Position< ");
	ConsolePrintVector2(consolePhysicsMotion2D.m_position);

	//if(consolePhysicsMotion2D.m_velocityUnitsPerSecond != Vector2(0.0f, 0.0f) ){
		ConsolePrintf(" >\n| Velocity< ");
		ConsolePrintVector2(consolePhysicsMotion2D.m_velocityUnitsPerSecond);
	//}

	//if(consolePhysicsMotion2D.m_accelerationUnitsPerSecondSquared != Vector2(0.0f, 0.0f) ){
		ConsolePrintf(" >\n| Acceleration< ");
		ConsolePrintVector2(consolePhysicsMotion2D.m_accelerationUnitsPerSecondSquared);
	//}

	ConsolePrintf(" >\n| OrientationDegrees =");
	ConsolePrintf(" %.02f |\n", consolePhysicsMotion2D.m_orientationDegrees);
	
	//if(consolePhysicsMotion2D.m_angularVelocityDegreesPerSecond != 0.0f){
		ConsolePrintf("| AngularVelocityDegrees =");
		ConsolePrintf(" %.02f |\n", consolePhysicsMotion2D.m_angularVelocityDegreesPerSecond);
	//}

}

#endif