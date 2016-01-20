//==============================================================================================================
//Math2D.cpp
//by Albert Chen Jan-21-2015.
//==============================================================================================================

#include "Math3D.hpp"



bool DoAABB3sOverlap( const AABB3& first, const AABB3& second ){
	if(second.mins.x > first.maxs.x){
		return false;
	}
	if(second.maxs.x < first.mins.x){
		return false;
	}
	if(second.mins.y > first.maxs.y){
		return false;
	}
	if(second.maxs.y < first.mins.y){
		return false;
	}
	if(second.mins.z > first.maxs.z){
		return false;
	}
	if(second.maxs.z < first.mins.z){
		return false;
	}
	return true;

}

bool DoSpheresOverlap( const Sphere3& first, const Sphere3& second ){
	float sumRadii = first.radius + second.radius;
	bool DoOverlap = CalcDistanceSquared(first.center, second.center ) < ( sumRadii * sumRadii );

	return DoOverlap;
}

const Sphere3 CalcBoundingSphere(Vector3s& pointCloud ){
	AABB3 indexOfPointCloud = AABB3(Vector3::ZERO, Vector3::ZERO );
	unsigned int pointCloudIndex = 1;

	//for each point check for min x,y,z and max x,y,z
	while(pointCloudIndex < pointCloud.size()){
		
		float xComponentOfPointCloudIndex;
		float yComponentOfPointCloudIndex;
		float zComponentOfPointCloudIndex;

		pointCloud[pointCloudIndex].GetXYZ(xComponentOfPointCloudIndex, yComponentOfPointCloudIndex, zComponentOfPointCloudIndex);
		//get index of mins x, y, z
		if(xComponentOfPointCloudIndex < pointCloud[(unsigned int)indexOfPointCloud.mins.x].x){
			indexOfPointCloud.mins.x = (float)pointCloudIndex;
		}
		if(yComponentOfPointCloudIndex < pointCloud[(unsigned int)indexOfPointCloud.mins.y].y){
			indexOfPointCloud.mins.y = (float)pointCloudIndex;
		}
		if(zComponentOfPointCloudIndex < pointCloud[(unsigned int)indexOfPointCloud.mins.z].z){
			indexOfPointCloud.mins.z = (float)pointCloudIndex;
		}
		//get index maxs x, y, z
		if(xComponentOfPointCloudIndex > pointCloud[(unsigned int)indexOfPointCloud.maxs.x].x){
			indexOfPointCloud.maxs.x = (float)pointCloudIndex;
		}
		if(yComponentOfPointCloudIndex > pointCloud[(unsigned int)indexOfPointCloud.maxs.y].y){
			indexOfPointCloud.maxs.y = (float)pointCloudIndex;
		}
		if(zComponentOfPointCloudIndex > pointCloud[(unsigned int)indexOfPointCloud.maxs.z].z){
			indexOfPointCloud.maxs.z = (float)pointCloudIndex;
		}

		
		pointCloudIndex++;

	}//end of while



	float distXSquared = ( pointCloud[(unsigned int)indexOfPointCloud.maxs.x].x + pointCloud[(unsigned int)indexOfPointCloud.mins.x].x );
	distXSquared *= distXSquared;

	float distYSquared = ( pointCloud[(unsigned int)indexOfPointCloud.maxs.y].y + pointCloud[(unsigned int)indexOfPointCloud.mins.y].y );
	distYSquared *= distYSquared;

	float distZSquared = ( pointCloud[(unsigned int)indexOfPointCloud.maxs.z].z + pointCloud[(unsigned int)indexOfPointCloud.mins.z].z );
	distZSquared *= distZSquared;

	float newBoundingSphereInitialRadius = 0.0f;

	//distXsquared is biggest
	if (distXSquared > distYSquared && distXSquared > distZSquared){
		newBoundingSphereInitialRadius = sqrt(distXSquared);
	}
	//distYsquared is biggest
	if (distYSquared > distXSquared && distYSquared > distZSquared){
		newBoundingSphereInitialRadius = sqrt(distYSquared);
	}

	//distZsquared is biggest
	if (distZSquared > distXSquared && distZSquared > distYSquared){
		newBoundingSphereInitialRadius = sqrt(distZSquared);
	}

	Vector3 newBoundingSphereInitialCenter;

	AABB3 pointsAtBoundsOfMinMax = 
	AABB3(
		Vector3( pointCloud[(unsigned int)indexOfPointCloud.mins.x].x , pointCloud[(unsigned int)indexOfPointCloud.mins.y].y , pointCloud[(unsigned int)indexOfPointCloud.mins.z].z ) , 
		Vector3( pointCloud[(unsigned int)indexOfPointCloud.maxs.x].x , pointCloud[(unsigned int)indexOfPointCloud.maxs.y].y , pointCloud[(unsigned int)indexOfPointCloud.maxs.z].z )
	);

	newBoundingSphereInitialCenter = pointsAtBoundsOfMinMax.CalcCenter();

	Sphere3 newBoundingSphere(newBoundingSphereInitialCenter, newBoundingSphereInitialRadius);

	pointCloudIndex = 0;
	//loop through again growing sphere each time
	while(pointCloudIndex < pointCloud.size()){
		Vector3 vertexAtIndex = pointCloud[pointCloudIndex];

		float distFromVertexToSphereCenterSquared = CalcDistanceSquared(vertexAtIndex, newBoundingSphere.center);
		//if dist is still greater, radius becomes dist
		if(distFromVertexToSphereCenterSquared > newBoundingSphere.radius * newBoundingSphere.radius){
			newBoundingSphere.radius = distFromVertexToSphereCenterSquared;
		}

	}
	
	return newBoundingSphere;
}

bool IsPointInsideSphere(const Vector3& point, const Sphere3& sphere ){
		if (CalcDistanceSquared(sphere.center, point) <= sphere.radius * sphere.radius ) {
			return true;
		}
		return false;
}

// 
// ///----------------------------------------------------------------------------------------------------------
// //generic 3D raycast, step and sample, decently fast, 
// RayCastResult3D Raycast3D(LineSegment3 rayToTrace, Raycast3DIsImpactFunction* isImpact3DCallback, const float& tStep  ){
// 	//ClearRaycastBlockSelected();
// 	RayCastResult3D castingRay;
// 
// 	float t = 0.0f;
// 
// 	for (t = 0.0f; t < 1.0f; t+=tStep){
// 		Vector3 rayToTracePointAtT = rayToTrace.GetPointAtParametricValue(t);
// 		castingRay.impactPoint = rayToTracePointAtT;
// 
// 		if(isImpact3DCallback(castingRay.impactPoint)){
// 			//set camera forward ray cast
// 			castingRay.impactPoint = rayToTracePointAtT;
// 			castingRay.didImpact = true;
// 			break;
// 		}
// 
// 	}//end of for
// 
// 	return castingRay;
// 
// }

