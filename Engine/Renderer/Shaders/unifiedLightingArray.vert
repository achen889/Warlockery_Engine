#version 330 core

uniform float gTime;

uniform mat4 gView;
uniform mat4 gProj;
uniform mat4 gModel;
uniform vec3 gCamPos;

in vec3 inPosition;
in vec4 inColor;
in vec2 inUV;
in vec3 inTangent;
in vec3 inBiTangent;
in vec3 inNormal;

out vec3 passPosition;
out vec3 passLightColor;
out vec3 passNormal;
out vec2 passUV0;
out vec3 passTangent;
out vec3 passBiTangent;
out vec3 passCamPos;

void main( void )
{
	vec4 pos = vec4( inPosition, 1.0 );
    vec3 world_pos = (pos * gModel).xyz;
    float elapsedTime = 0.5f * gTime;
    //pos = pos * gModel;
	pos = pos  * gModel* gView * gProj;

	
	

	vec3 Tangent = normalize(inTangent);
	vec3 BiTangent = normalize(inBiTangent);
	vec3 Normal = normalize(inNormal);

	//mat3 TBNMatrix = mat3(Tangent, BiTangent, Normal );

    passPosition = world_pos;
	passNormal = (vec4(Normal, 0.0f) * gModel ).xyz;
    passUV0 = inUV;
    passLightColor = inColor.xyz;
	gl_Position = pos;
	passTangent =  (vec4(Tangent, 0.0f) * gModel ).xyz;
	passBiTangent = (vec4( BiTangent, 0.0f) * gModel ).xyz;
	passCamPos = (vec4(gCamPos, 0.0f) * gModel).xyz;
	//passTBN = TBNMatrix;
}