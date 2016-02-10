#version 330 core

uniform float gTime;
uniform vec2 gMousePos;
uniform vec2 gCamPos;

uniform mat4 gView;
uniform mat4 gProj;
uniform mat4 gModel;
   
in vec3 inPosition;
in vec4 inColor;
in vec2 inUV;

// Name must match PS IN
out vec4 passColor;
out vec3 passPos;
out vec2 passUV;

void main( void )
{
    vec4 pos = vec4( inPosition, 1.0 );
    vec3 world_pos = (pos * gModel).xyz;
    float elapsedTime = 0.5f * gTime;
    //pos = pos * gModel;
    pos = pos  * gModel* gView * gProj;
    
    vec4 color = inColor;
    //color.xyz = -color.xyz * sin(elapsedTime);
    //color *= 0.95f;
    //color *= 0.5f + sin( pos.x * pos.y * elapsedTime); //shimmery effect

	passColor = color;
    passUV = inUV;
    passPos = pos.xyz;

	gl_Position = pos;
}