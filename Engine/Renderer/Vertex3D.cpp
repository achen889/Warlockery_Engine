//==============================================================================================================
//Vertex3D.cpp
//by Albert Chen Jul-4-2015.
//==============================================================================================================

#include "Vertex3D.hpp"
#include "Engine\Math\AABB2.hpp"
#include "Engine\Math\Math3D.hpp"

Vertex3D::Vertex3D(){
	//do nothing
}

Vertex3D::Vertex3D(const Vector3& position, const Rgba& color, const Vector2& texCoords, const Vector3& normal, const Vector3& tangent, const Vector3& bitangent) :
m_position(position),
m_color(color),
m_texCoords(texCoords),
m_normal(normal),
m_tangent(tangent),
m_biTangent(bitangent)
{

}

Vertex3D::~Vertex3D(){
	//do nothing
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///friend methods

//-----------------------------------------------------------------------------------------------------------

void GenerateVertexArrayQuad2D(Vertex3Ds& out_vertexArray, const AABB2& boxCoords, const Rgba& vertexColor, bool clearVerts){

	if (clearVerts){
		out_vertexArray.clear();
	}

	out_vertexArray.reserve(4);

	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.mins.y, 0.0f), vertexColor )); //0
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.maxs.y, 0.0f), vertexColor )); //1
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.maxs.y, 0.0f), vertexColor )); //2
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.mins.y, 0.0f), vertexColor )); //3
	
}

//-----------------------------------------------------------------------------------------------------------


void GenerateVertexArrayQuad3D(Vertex3Ds& out_vertexArray, const AABB3& boxCoords, const Rgba& vertexColor, bool clearVerts){

	if (clearVerts){
		out_vertexArray.clear();
	}

	out_vertexArray.reserve(24);

	Vector2s samplerDefaultTextureCoords = GetTextureCoordinatesFromAABB2(AABB2::ZERO_TO_ONE);



	//push stuff back to the out vertex
	//Z+ Up X+ Fwd Y+ Rt
	//GL triangle box
	//back
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.maxs.y, boxCoords.mins.z), vertexColor, samplerDefaultTextureCoords[1], Vector3::BACKWARD));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.mins.y, boxCoords.mins.z), vertexColor, samplerDefaultTextureCoords[0], Vector3::BACKWARD));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.maxs.y, boxCoords.maxs.z), vertexColor, samplerDefaultTextureCoords[2], Vector3::BACKWARD));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.mins.y, boxCoords.mins.z), vertexColor, samplerDefaultTextureCoords[0], Vector3::BACKWARD));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.maxs.y, boxCoords.maxs.z), vertexColor, samplerDefaultTextureCoords[2], Vector3::BACKWARD));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.mins.y, boxCoords.maxs.z), vertexColor, samplerDefaultTextureCoords[3], Vector3::BACKWARD));
	
	//front	
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.maxs.y, boxCoords.mins.z), vertexColor, samplerDefaultTextureCoords[0], Vector3::FORWARD));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.mins.y, boxCoords.mins.z), vertexColor, samplerDefaultTextureCoords[1], Vector3::FORWARD));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.maxs.y, boxCoords.maxs.z), vertexColor, samplerDefaultTextureCoords[3], Vector3::FORWARD));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.mins.y, boxCoords.mins.z), vertexColor, samplerDefaultTextureCoords[1], Vector3::FORWARD));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.maxs.y, boxCoords.maxs.z), vertexColor, samplerDefaultTextureCoords[3], Vector3::FORWARD));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.mins.y, boxCoords.maxs.z), vertexColor, samplerDefaultTextureCoords[2], Vector3::FORWARD));
 	
	//left																					   
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.mins.y, boxCoords.mins.z), vertexColor, samplerDefaultTextureCoords[1], Vector3::LEFT));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.mins.y, boxCoords.maxs.z), vertexColor, samplerDefaultTextureCoords[2], Vector3::LEFT));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.mins.y, boxCoords.maxs.z), vertexColor, samplerDefaultTextureCoords[3], Vector3::LEFT));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.mins.y, boxCoords.maxs.z), vertexColor, samplerDefaultTextureCoords[3], Vector3::LEFT));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.mins.y, boxCoords.mins.z), vertexColor, samplerDefaultTextureCoords[0], Vector3::LEFT));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.mins.y, boxCoords.mins.z), vertexColor, samplerDefaultTextureCoords[1], Vector3::LEFT));
 	 	
	//right																						   
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.maxs.y, boxCoords.mins.z), vertexColor, samplerDefaultTextureCoords[0], Vector3::RIGHT));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.maxs.y, boxCoords.maxs.z), vertexColor, samplerDefaultTextureCoords[3], Vector3::RIGHT));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.maxs.y, boxCoords.maxs.z), vertexColor, samplerDefaultTextureCoords[2], Vector3::RIGHT));	
 	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.maxs.y, boxCoords.maxs.z), vertexColor, samplerDefaultTextureCoords[2], Vector3::RIGHT));
 	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.maxs.y, boxCoords.mins.z), vertexColor, samplerDefaultTextureCoords[1], Vector3::RIGHT));
 	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.maxs.y, boxCoords.mins.z), vertexColor, samplerDefaultTextureCoords[0], Vector3::RIGHT));

	//top																						   
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.mins.y, boxCoords.maxs.z), vertexColor, samplerDefaultTextureCoords[0], Vector3::UP));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.maxs.y, boxCoords.maxs.z), vertexColor, samplerDefaultTextureCoords[1], Vector3::UP));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.maxs.y, boxCoords.maxs.z), vertexColor, samplerDefaultTextureCoords[2], Vector3::UP));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.maxs.y, boxCoords.maxs.z), vertexColor, samplerDefaultTextureCoords[2], Vector3::UP));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.mins.y, boxCoords.maxs.z), vertexColor, samplerDefaultTextureCoords[3], Vector3::UP));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.mins.y, boxCoords.maxs.z), vertexColor, samplerDefaultTextureCoords[0], Vector3::UP));
 	
	//bottom																					   
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.mins.y, boxCoords.mins.z), vertexColor, samplerDefaultTextureCoords[3], Vector3::DOWN));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.maxs.y, boxCoords.mins.z), vertexColor, samplerDefaultTextureCoords[2], Vector3::DOWN));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.maxs.y, boxCoords.mins.z), vertexColor, samplerDefaultTextureCoords[1], Vector3::DOWN));	
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.maxs.y, boxCoords.mins.z), vertexColor, samplerDefaultTextureCoords[1], Vector3::DOWN));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.mins.y, boxCoords.mins.z), vertexColor, samplerDefaultTextureCoords[0], Vector3::DOWN));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.mins.y, boxCoords.mins.z), vertexColor, samplerDefaultTextureCoords[3], Vector3::DOWN));


}



//-----------------------------------------------------------------------------------------------------------

void GenerateVertexArrayTextureQuad(Vertex3Ds& out_vertexArray, const AABB2& boxCoords, const AABB2& textureCoords, const Rgba& vertexColor, bool clearVerts){
	
	if (clearVerts){
		out_vertexArray.clear();
	}
	
	out_vertexArray.reserve(4);

	Vector2s samplerDefaultTextureCoords = GetTextureCoordinatesFromAABB2(textureCoords);

	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.mins.y, 0.0f), vertexColor, samplerDefaultTextureCoords[0])); //0
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.maxs.y, 0.0f), vertexColor, samplerDefaultTextureCoords[3])); //1
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.maxs.y, 0.0f), vertexColor, samplerDefaultTextureCoords[2])); //2
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.mins.y, 0.0f), vertexColor, samplerDefaultTextureCoords[1])); //3
	
	//finally fixed, the vertexes are correct again

	//currently has bugs with line loop, line strip, has bug with triangle strip

}

//-----------------------------------------------------------------------------------------------------------

void GenerateVertexArrayCoordinateAxes(Vertex3Ds& out_vertexArray){
	//use draw line strip
	out_vertexArray.clear();
	out_vertexArray.reserve(10000);
	//coordinate axes for normal
	//x axis
	out_vertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, 0.0f), Rgba::RED, Vector2::ZERO));
	out_vertexArray.push_back(Vertex3D(Vector3(1.0f, 0.0f, 0.0f), Rgba::MAGENTA, Vector2::ZERO));
	//x arrowhead
	out_vertexArray.push_back(Vertex3D(Vector3(1.0f, 0.1f, 0.0f), Rgba::MAGENTA, Vector2::ZERO));
	out_vertexArray.push_back(Vertex3D(Vector3(1.2f, 0.0f, 0.0f), Rgba::ORANGE, Vector2::ZERO));
	out_vertexArray.push_back(Vertex3D(Vector3(1.0f, -0.1f, 0.0f), Rgba::MAGENTA, Vector2::ZERO));
	out_vertexArray.push_back(Vertex3D(Vector3(1.0f, 0.0f, 0.0f), Rgba::RED, Vector2::ZERO));
	//y axis
	out_vertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, 0.0f), Rgba::GREEN, Vector2::ZERO));
	out_vertexArray.push_back(Vertex3D(Vector3(0.0f, 1.0f, 0.0f), Rgba::MINT_GREEN, Vector2::ZERO));
	//y arrowhead
	out_vertexArray.push_back(Vertex3D(Vector3(0.1f, 1.0f, 0.0f), Rgba::NEON_GREEN, Vector2::ZERO));
	out_vertexArray.push_back(Vertex3D(Vector3(0.0f, 1.2f, 0.0f), Rgba::MINT_GREEN, Vector2::ZERO));
	out_vertexArray.push_back(Vertex3D(Vector3(-0.1f, 1.0f, 0.0f), Rgba::NEON_GREEN, Vector2::ZERO));
	out_vertexArray.push_back(Vertex3D(Vector3(0.0f, 1.0f, 0.0f), Rgba::GREEN, Vector2::ZERO));
	//z axis
	out_vertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, 0.0f), Rgba::BLUE, Vector2::ZERO));
	out_vertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, 1.0f), Rgba::PURPLE, Vector2::ZERO));
	//z arrowhead
	out_vertexArray.push_back(Vertex3D(Vector3(0.0f, 0.1f, 1.0f), Rgba::LAVENDER, Vector2::ZERO));
	out_vertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, 1.2f), Rgba::TRON_BLUE, Vector2::ZERO));
	out_vertexArray.push_back(Vertex3D(Vector3(0.0f, -0.1f, 1.0f), Rgba::LAVENDER, Vector2::ZERO));
	out_vertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, 1.0f), Rgba::BLUE, Vector2::ZERO));
}

//-----------------------------------------------------------------------------------------------------------

///----------------------------------------------------------------------------------------------------------
///I think I fixed the inverted texture issues
void GenerateVertexArraySphere3D(Vertex3Ds& out_vertexArray, Sphere3 sphereToDraw, const int& wedges, const int& slices, const Rgba& viewColor){
	//UNUSED(viewColor);
	//out_vertexArray.clear();
	out_vertexArray.reserve(wedges * slices);
	//push stuff back to the out vertex
	const float inverseTwoPI = 1.0f / TWO_PI;
	const float inversePI = 1.0f / PI;

	float wedgeStep = TWO_PI / (float)(wedges);
	float sliceStep = PI / (float)(slices);

	for (float w = 0.0f; w < TWO_PI; w += wedgeStep){

		float wNext = w + wedgeStep;

		float sineOfWedges = sin(w);
		float cosineOfWedges = cos(w);

		float sineOfWedgesNext = sin(wNext);
		float cosineOfWedgesNext = cos(wNext);
		//float s = 0.0f;
		for (float s = 0.0f; s <= PI + sliceStep; s += sliceStep){
			float sNext = s + sliceStep;

			float sineOfSlices = sin(s);
			float cosineOfSlices = cos(s);

			float sineOfSlicesNext = sin(sNext);
			float cosineOfSlicesNext = cos(sNext);

			float sphereDrawX;
			float sphereDrawY;
			float sphereDrawZ;

			float sphereToDrawUMap;
			float sphereToDrawVMap;

			Vector2 sphereUVMapping;
			Vector3 nextPointOnSphere;
			Vector3 unitCenterToSurface;
			Vector3 tangentOfSphereNormal;
			Vector3 biTangentOfSphereNormal;

			//0
			sphereDrawX = sineOfSlices*cosineOfWedges; // sinS * cosW	
			sphereDrawY = sineOfSlices*sineOfWedges; // sinS * sinW	
			sphereDrawZ = cosineOfSlices; // cosS
			nextPointOnSphere = Vector3(
				sphereDrawX * sphereToDraw.radius + sphereToDraw.center.x,
				sphereDrawY * sphereToDraw.radius + sphereToDraw.center.y,
				sphereDrawZ * sphereToDraw.radius + sphereToDraw.center.z
				);
			unitCenterToSurface = nextPointOnSphere - sphereToDraw.center;
			unitCenterToSurface.Normalize();
			sphereToDrawUMap = w * inverseTwoPI;	
			sphereToDrawVMap = s * inversePI;
			sphereUVMapping = Vector2(1 - sphereToDrawUMap, sphereToDrawVMap);
			//sphereUVMapping.Normalize();	
			tangentOfSphereNormal = Vector3(-sphereDrawY, sphereDrawX, 0.0f); 
			tangentOfSphereNormal.Normalize();
			biTangentOfSphereNormal = CrossProduct(tangentOfSphereNormal, unitCenterToSurface);
			biTangentOfSphereNormal.Normalize();
			out_vertexArray.push_back(Vertex3D(nextPointOnSphere, viewColor, sphereUVMapping, unitCenterToSurface, tangentOfSphereNormal, biTangentOfSphereNormal));

			//1
			sphereDrawX = sineOfSlicesNext*cosineOfWedges; // sinS * cosW
			sphereDrawY = sineOfSlicesNext*sineOfWedges; // sinS * sinW
			sphereDrawZ = cosineOfSlicesNext; // cosS
			nextPointOnSphere = Vector3(
				sphereDrawX * sphereToDraw.radius + sphereToDraw.center.x,
				sphereDrawY * sphereToDraw.radius + sphereToDraw.center.y,
				sphereDrawZ * sphereToDraw.radius + sphereToDraw.center.z
				);
			unitCenterToSurface = nextPointOnSphere - sphereToDraw.center;
			unitCenterToSurface.Normalize();
			sphereToDrawUMap = w * inverseTwoPI; //0.5f + (atan2f(unitPointToCenter.z, unitPointToCenter.x) * inverseTwoPI);
			sphereToDrawVMap = sNext * inversePI; //0.5f - (asinf(unitPointToCenter.y) * inversePI);
			sphereUVMapping = Vector2(1 - sphereToDrawUMap, sphereToDrawVMap);
			//sphereUVMapping.Normalize();
			tangentOfSphereNormal = Vector3(-sphereDrawY, sphereDrawX, 0.0f);
			tangentOfSphereNormal.Normalize();
			biTangentOfSphereNormal = CrossProduct(tangentOfSphereNormal, unitCenterToSurface);
			biTangentOfSphereNormal.Normalize();
			out_vertexArray.push_back(Vertex3D(nextPointOnSphere, viewColor, sphereUVMapping, unitCenterToSurface, tangentOfSphereNormal, biTangentOfSphereNormal));
			
			//3
			sphereDrawX = sineOfSlices*cosineOfWedgesNext; // sinS * cosWN
			sphereDrawY = sineOfSlices*sineOfWedgesNext; // sinS * sinW
			sphereDrawZ = cosineOfSlices; // cosS
			nextPointOnSphere = Vector3(
				sphereDrawX * sphereToDraw.radius + sphereToDraw.center.x,
				sphereDrawY * sphereToDraw.radius + sphereToDraw.center.y,
				sphereDrawZ * sphereToDraw.radius + sphereToDraw.center.z
				);
			unitCenterToSurface = nextPointOnSphere - sphereToDraw.center;
			unitCenterToSurface.Normalize();
			sphereToDrawUMap = wNext * inverseTwoPI; //0.5f + (atan2f(unitPointToCenter.z, unitPointToCenter.x) * inverseTwoPI);		
			sphereToDrawVMap = s * inversePI; //0.5f - (asinf(unitPointToCenter.y) * inversePI);
			sphereUVMapping = Vector2(1 - sphereToDrawUMap, sphereToDrawVMap);
			//sphereUVMapping.Normalize();
			tangentOfSphereNormal = Vector3(-sphereDrawY, sphereDrawX, 0.0f);
			tangentOfSphereNormal.Normalize();
			biTangentOfSphereNormal = CrossProduct(tangentOfSphereNormal, unitCenterToSurface);
			biTangentOfSphereNormal.Normalize();
			out_vertexArray.push_back(Vertex3D(nextPointOnSphere, viewColor, sphereUVMapping, unitCenterToSurface, tangentOfSphereNormal, biTangentOfSphereNormal));
			
			//2
			sphereDrawX = sineOfSlicesNext*cosineOfWedgesNext; // sinSN * cosWN
			sphereDrawY = sineOfSlicesNext*sineOfWedgesNext; // sinSN * sinWN
			sphereDrawZ = cosineOfSlicesNext; // cosS
			nextPointOnSphere = Vector3(
				sphereDrawX * sphereToDraw.radius + sphereToDraw.center.x,
				sphereDrawY * sphereToDraw.radius + sphereToDraw.center.y,
				sphereDrawZ * sphereToDraw.radius + sphereToDraw.center.z
				);
			unitCenterToSurface = nextPointOnSphere - sphereToDraw.center;
			unitCenterToSurface.Normalize();
			sphereToDrawUMap = wNext * inverseTwoPI; //0.5f + (atan2f(unitPointToCenter.z, unitPointToCenter.x) * inverseTwoPI);		
			sphereToDrawVMap = sNext * inversePI; //0.5f - (asinf(unitPointToCenter.y) * inversePI);
			sphereUVMapping = Vector2(1-sphereToDrawUMap, sphereToDrawVMap);
			//sphereUVMapping.Normalize();
			tangentOfSphereNormal = Vector3(-sphereDrawY, sphereDrawX, 0.0f);
			tangentOfSphereNormal.Normalize();
			biTangentOfSphereNormal = CrossProduct(tangentOfSphereNormal, unitCenterToSurface);
			biTangentOfSphereNormal.Normalize();
			out_vertexArray.push_back(Vertex3D(nextPointOnSphere, viewColor, sphereUVMapping, unitCenterToSurface, tangentOfSphereNormal, biTangentOfSphereNormal));

		}
	}
}

//-----------------------------------------------------------------------------------------------------------

void GenerateVertexArrayDisc2D(Vertex3Ds& out_verts, Disc2 discToDraw, const Rgba& viewColor){
	//std::vector<Vector2>DiscVertices;
	out_verts.reserve(360);
	//calculate disc points
	Vector2 DiscVertex;

	Vector3 discRenderPoint = ToVector3(DiscVertex);

	Vector2 discTexCoords;
	discTexCoords.SetUnitLengthAndHeadingDegrees(0.0f);
	
	Vertex3D discVert3D = Vertex3D(discRenderPoint, viewColor, discTexCoords);
	const int degreeStep = 1;
	for (int i = 1; i < 360; i += degreeStep){
		DiscVertex.SetLengthAndHeadingRadians(discToDraw.radius, float(i));
		DiscVertex += discToDraw.center;
		//DiscVertex.RotateDegrees(degreeStep);

		discTexCoords.RotateDegrees((float)degreeStep);

		discRenderPoint = ToVector3(DiscVertex, 0.0f);

		discVert3D.m_position = discRenderPoint;
		discVert3D.m_texCoords = discTexCoords;

		out_verts.push_back(discVert3D);
	}

}

//-----------------------------------------------------------------------------------------------------------

void AppendVerticesToExistingVertexArray(Vertex3Ds& out_vertexArray, const Vertex3Ds& vertsToAppend){
	
	//append the out vert array
	for (Vertex3DConstIterator it = vertsToAppend.begin(); it != vertsToAppend.end(); ++it){
		Vertex3D vertex = (*it);
		
		out_vertexArray.push_back(vertex);
	}

}
