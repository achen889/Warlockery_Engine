//==============================================================================================================
//Sphere3.cpp
//by Albert Chen Feb-18-2015.
//==============================================================================================================

#include "Engine\Math\Sphere3.hpp"

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///friend functions
const Sphere3 Interpolate( const Sphere3& start, const Sphere3& end, float fractionFromStartToEnd ){
	Vector3 displacementCenterStartToEnd = end.center - start.center;
	float displacementRadiusStartToEnd = end.radius - start.radius;

	Sphere3 interpolatedDisc ( (displacementCenterStartToEnd * fractionFromStartToEnd) , ( displacementRadiusStartToEnd * fractionFromStartToEnd) );
	interpolatedDisc.center +=start.center;
	interpolatedDisc.radius +=start.radius;

	return interpolatedDisc;
}

//return a sphere that contains sphere A and sphere B
const Sphere3 Union(const Sphere3& sphereA, const Sphere3& sphereB){
	Vector3 distAB = sphereB.center - sphereA.center;
	float distABSquared = DotProduct(distAB, distAB);
	Vector3 normDistAB = distAB;
	normDistAB.Normalize();
	Vector3 pA = sphereA.center - sphereA.radius * normDistAB;
	Vector3 pB = sphereB.center - sphereB.radius * normDistAB;

	Vector3 distP = pB - pA;

	Vector3 newCenterUnionAB = pA + 0.5f * distP;

	float newRadiusUnionAB = 0.5f * (distABSquared + sphereA.radius  + sphereB.radius);

	return Sphere3(newCenterUnionAB, newRadiusUnionAB);

}

