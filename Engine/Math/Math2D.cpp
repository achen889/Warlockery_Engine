//==============================================================================================================
//Math2D.cpp
//by Albert Chen Jan-21-2015.
//==============================================================================================================

#include "Math2D.hpp"

//===========================================================================================================

bool DoAABBsOverlap( const AABB2& first, const AABB2& second ){
	if(second.mins.x > first.maxs.x){
		return false;
	}
	if(second.maxs.x < first.mins.x){
		return  false;
	}
	if(second.mins.y > first.maxs.y){
		return false;
	}
	if(second.maxs.y < first.mins.y){
		return false;
	}
	return true;
}

//-----------------------------------------------------------------------------------------------------------

bool DoDiscsOverlap( const Disc2& first, const Disc2& second ){
	float sumRadii = first.radius + second.radius;
	bool DoOverlap = CalcDistanceSquared(first.center, second.center ) < ( sumRadii * sumRadii );
	return DoOverlap;
}

//-----------------------------------------------------------------------------------------------------------

bool IsPointInsideDisc2(const Disc2& disc, const Vector2& point){
	if (CalcDistanceSquared(disc.center, point) <= disc.radius * disc.radius) {
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------------------------------------

bool DoAABBAndPointOverlap(const AABB2& boxA, const Vector2& pointB){
	if (pointB.x > boxA.maxs.x)
		return false;
	if (pointB.x < boxA.mins.x)
		return false;
	if (pointB.y > boxA.maxs.y)
		return false;
	if (pointB.y < boxA.mins.y)
		return false;

	return true;
}

//-----------------------------------------------------------------------------------------------------------

bool DoAABBAndPointOverlap(const Vector2& pointA, const AABB2& boxB){
	 
	return DoAABBAndPointOverlap(boxB, pointA);
}

//-----------------------------------------------------------------------------------------------------------

bool DoShapesOverlap( const AABB2& box, const Disc2& disc ){
//	float discCenterPlusRadiusX = disc.center.x + disc.radius;
//	float discCenterPlusRadiusY = disc.center.y + disc.radius;

	//check the edges
// 	if( discCenterPlusRadiusX < box.mins.x ){
// 		return false;
// 	}
// 	else if(discCenterPlusRadiusX > box.maxs.x ){
// 		return false;
// 	}
// 	else if(discCenterPlusRadiusY < box.mins.y ){
// 		return false;
// 	}
// 	else if(discCenterPlusRadiusY > box.maxs.y ){
// 		return false;
// 	}

	//check the corners
	if(!disc.IsPointInside(box.mins ) &&
	   !disc.IsPointInside(Vector2(box.mins.x, box.maxs.y ) ) &&
	   !disc.IsPointInside(Vector2(box.maxs.x, box.mins.y ) ) &&
	   !disc.IsPointInside(box.maxs ) ){
	   return false;
	}

	return true;
}

//-----------------------------------------------------------------------------------------------------------

bool DoShapesOverlap( const Disc2& disc, const AABB2& box ){
	return DoShapesOverlap(box, disc );
}

///----------------------------------------------------------------------------------------------------------
//generic 2D raycast, step and sample, decently fast, 
TraceResult2D Raycast2D(LineSegment2 rayToTrace, Raycast2DIsImpactFunction* isImpact2DCallback, const float& tStep  ){
	//ClearRaycastBlockSelected();
	TraceResult2D castingRay;

	float t = 0.0f;
	
	for (t = 0.0f; t < 1.0f; t+=tStep){

		Vector2 rayToTracePointAtT = rayToTrace.GetPointAtParametricValue(t);

		//set camera forward ray cast
		castingRay.impactPoint = rayToTracePointAtT;

		if(isImpact2DCallback(castingRay.impactPoint) ){

			Vector2 planeNormalFromImpactToStart = castingRay.impactPoint - rayToTrace.startPoint;
			planeNormalFromImpactToStart.Normalize();
			castingRay.impactNormal = planeNormalFromImpactToStart;

			castingRay.didImpact = true;
			break;
		}
		

	}//end of for

	return castingRay;

}


