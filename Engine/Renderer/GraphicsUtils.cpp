//==============================================================================================================
//GraphicsUtils.cpp
//by Albert Chen Jul-12-2015.
//==============================================================================================================

#include "GraphicsUtils.hpp"

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///Debug Shape and DebugShapes
//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///constructors

DebugShape::DebugShape(){
	//do nothing
	
	//m_shapeMeshRenderer->BindVertexArray();
}

///----------------------------------------------------------------------------------------------------------
///super constructor
DebugShape::DebugShape(const ShapeType& newshape, const Vector3&  newpos, const Vector3& newRad, float lifespanSeconds,
	const Rgba& newstartCol, const Rgba& newendCol,
	bool isdepthTestOn, bool isDualMode) :
	shape(newshape),
	position(newpos),
	radius(newRad),
	startColor(newstartCol),
	endColor(newendCol),
	secondsSinceSpawn(0.0f),
	lifespanSeconds(lifespanSeconds),
	depthTestOn(isdepthTestOn),
	dualMode(isDualMode)

{

}

///----------------------------------------------------------------------------------------------------------
///LineSeg Constructor
DebugShape::DebugShape(const LineSegment3& linesegToDebug, const float& lifespanSeconds,
	const Rgba& newstartCol, const Rgba& newendCol, const ShapeType& newshape, bool isdepthTestOn, bool isDualMode) :
	shape(newshape),
	position(linesegToDebug.startPoint),
	radius(linesegToDebug.endPoint),
	startColor(newstartCol),
	endColor(newendCol),
	secondsSinceSpawn(0.0f),
	lifespanSeconds(lifespanSeconds),
	depthTestOn(isdepthTestOn),
	dualMode(isDualMode)

{

}

///----------------------------------------------------------------------------------------------------------
///sphere constructor
DebugShape::DebugShape(const Sphere3& sphereToDebug, const float& lifespanSeconds,
	const Rgba& newstartCol, bool isdepthTestOn, bool isDualMode) :
	shape(SHAPE_TYPE_SPHERE3D),
	position(sphereToDebug.center),
	startColor(newstartCol),
	endColor(Rgba::WHITE),
	secondsSinceSpawn(0.0f),
	lifespanSeconds(lifespanSeconds),
	depthTestOn(isdepthTestOn),
	dualMode(isDualMode)

{
	
	m_shapeMeshRenderer.m_material->InitializeMaterial("Data/basic.vert", "Data/basic.frag");
	m_shapeMeshRenderer.m_material->m_samplerInUse = false;

	m_shapeMeshRenderer.m_mesh->SetDrawMode(GL_LINE_STRIP);
	m_shapeMeshRenderer.m_mesh->InitializeSphereMesh(sphereToDebug, startColor);
	

	m_shapeMeshRenderer.BindVertexArray();

	radius.x = sphereToDebug.radius;
}

DebugShape::DebugShape(const Vector3& pointToDebug, const float& lifespanSeconds,
	const Rgba& newstartCol, const Rgba& newendCol, bool isdepthTestOn, bool isDualMode) :
	shape(SHAPE_TYPE_POINT3D),
	position(pointToDebug),
	startColor(newstartCol),
	endColor(newendCol),
	secondsSinceSpawn(0.0f),
	lifespanSeconds(lifespanSeconds),
	depthTestOn(isdepthTestOn),
	dualMode(isDualMode)
{

}

DebugShape::DebugShape(const AABB3& boxToDebug, const float& lifespanSeconds,
	const Rgba& newstartCol, const Rgba& newendCol, bool isdepthTestOn, bool isDualMode) :
	shape(SHAPE_TYPE_BOX3D),
	position(boxToDebug.mins),
	radius(boxToDebug.maxs),
	startColor(newstartCol),
	endColor(newendCol),
	secondsSinceSpawn(0.0f),
	lifespanSeconds(lifespanSeconds),
	depthTestOn(isdepthTestOn),
	dualMode(isDualMode)
{

}

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///draw debug shape

void DebugShape::Draw(OpenGLRenderer* renderer, Camera3D* camera){
	if (!IsDead()){
		if (!depthTestOn){
			renderer->DisableModelViewDepthTest();
			
			DrawDebugShape(renderer, camera);
		}
		else if (depthTestOn){
			renderer->SetModelViewDepthTest();

			DrawDebugShape(renderer, camera);
		}
		else if (dualMode){
			startColor.a = 128;
			endColor.a = 128;
			renderer->DisableModelViewDepthTest();
			DrawDebugShape(renderer, camera);

			renderer->SetModelViewDepthTest();
			DrawDebugShape(renderer, camera);
		}

	}//render if not dead
}

void DebugShape::DrawDebugShape(OpenGLRenderer* renderer, Camera3D* camera){
	if (shape == SHAPE_TYPE_POINT3D){
		renderer->DrawPoint3D(position);
	}
	if (shape == SHAPE_TYPE_SPHERE3D){
		renderer->DrawSphere3D(Sphere3(position, radius.x), 16, 16, startColor);
		//renderer->DrawVertexArray(m_shapeMeshRenderer.m_mesh.m_drawMode, m_shapeMeshRenderer.m_vaoID, m_shapeMeshRenderer.m_mesh.m_numVerticesToDraw);
		if (camera){
			m_shapeMeshRenderer.RenderMesh(*camera, true);
		}
	}
	else if (shape == SHAPE_TYPE_BOX3D){
		//renderer->DrawAABB3(AABB3(position, radius.x, radius.y, radius.z), startColor); //draw with center/radius
		renderer->DrawAABB3(AABB3(position, radius), startColor); //draw as mins,maxs
	}
	else if (shape == SHAPE_TYPE_LINE3D){
		renderer->DrawLineSegment3D(LineSegment3(position, radius), startColor, endColor);
	}
	else if (shape == SHAPE_TYPE_ARROW3D){
		renderer->DrawArrow3D(LineSegment3(position, radius), startColor, endColor);
	}
}

//===========================================================================================================
//debug shapes methods

///----------------------------------------------------------------------------------------------------------
///update all debug shapes

void DebugShapes::Update(double deltaSeconds){

	for (std::vector<DebugShape*>::iterator it = debugShapesToProcess.begin(); it != debugShapesToProcess.end();){
		DebugShape& debugShape = (**it);

		if (!debugShape.IsDead()){
			//it = debugShapesToProcess.erase(it);
			// 			}
			// 			else{
			debugShape.Update(deltaSeconds);
			++it;
		}
		else{
			it = debugShapesToProcess.erase(it);
		}
	}
}

void DebugShapes::Draw(OpenGLRenderer* renderer, Camera3D* camera){
	for (unsigned int i = 0; i < debugShapesToProcess.size(); i++){
		debugShapesToProcess[i]->Draw(renderer, camera);
	}
}

//-----------------------------------------------------------------------------------------------------------

DebugShape* DebugShapes::AddDebugShape(DebugShape* debugShape){
	debugShapesToProcess.push_back(debugShape);
	//debugShapesToRender.push_back(new DebugShape());
	return debugShape;
}

