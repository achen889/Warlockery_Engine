//==============================================================================================================
//GraphicsUtils.hpp
//by Albert Chen Jul-12-2015.
//==============================================================================================================

#include "Engine/Console/DevConsole.hpp"

#include "Engine/Renderer/MeshRenderer.hpp"

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///Debug Shapes
enum ShapeType{
	SHAPE_TYPE_SPHERE3D,
	SHAPE_TYPE_POINT3D,
	SHAPE_TYPE_BOX3D,
	SHAPE_TYPE_LINE3D,
	SHAPE_TYPE_ARROW3D,
	NUM_SHAPETYPES
};

//typedef std::map<std::string, DebugShape*> DebugShapeMap;
//typedef std::map<std::string, DebugShape*>::iterator DebugShapeMapIterator;

struct DebugShape{
	ShapeType shape;
	Vector3 position;
	Vector3 radius;

	MeshRenderer m_shapeMeshRenderer;

	Rgba startColor;
	Rgba endColor;

	float secondsSinceSpawn;
	float lifespanSeconds;
	bool depthTestOn;
	bool dualMode; //drawn twice with depth test on and off

	DebugShape();

	//constructors
	DebugShape(const ShapeType& newshape, const Vector3&  newpos, const Vector3& newRad = Vector3::ZERO, float lifespanSeconds = 0.1f,
		const Rgba& newstartCol = Rgba::MAGENTA, const Rgba& newendCol = Rgba::MAGENTA,
		bool isdepthTestOn = true, bool isDualMode = false);

	DebugShape(const LineSegment3& linesegToDebug, const float& lifespanSeconds = 0.1f,
		const Rgba& newstartCol = Rgba::MAGENTA, const Rgba& newendCol = Rgba::MAGENTA, const ShapeType& newshape = SHAPE_TYPE_LINE3D, bool isdepthTestOn = true, bool isDualMode = false);

	DebugShape(const Sphere3& sphereToDebug, const float& lifespanSeconds = 0.1f,
		const Rgba& newstartCol = Rgba::MAGENTA, bool isdepthTestOn = true, bool isDualMode = false);

	DebugShape(const Vector3& pointToDebug, const float& lifespanSeconds = 0.1f,
		const Rgba& newstartCol = Rgba::MAGENTA, const Rgba& newendCol = Rgba::MAGENTA, bool isdepthTestOn = true, bool isDualMode = false);

	DebugShape(const AABB3& boxToDebug, const float& lifespanSeconds = 0.1f,
		const Rgba& newstartCol = Rgba::MAGENTA, const Rgba& newendCol = Rgba::MAGENTA, bool isdepthTestOn = true, bool isDualMode = false);
	//-----------------------------------------------------------------------------------------------------------
	void Update(double deltaSeconds){
		secondsSinceSpawn += (float)deltaSeconds;
	}

	void Draw(OpenGLRenderer* renderer, Camera3D* camera = NULL);

	void DrawDebugShape(OpenGLRenderer* renderer, Camera3D* camera = NULL);

	bool IsDead(){
		if (lifespanSeconds < 0.0f){
			return false;
		}

		return (secondsSinceSpawn > lifespanSeconds);
	}
};

struct DebugShapes{
	std::vector<DebugShape*> debugShapesToProcess;
	//DebugShapeMap debugShapesMap;

	void Update(double deltaSeconds);
	
	void Draw(OpenGLRenderer* renderer, Camera3D* camera = NULL);

	DebugShape* AddDebugShape(DebugShape* debugShape);
};

static DebugShapes g_debugShapes;




