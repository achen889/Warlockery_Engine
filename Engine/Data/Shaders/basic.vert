#version 330 core

uniform mat4 gView;
uniform mat4 gProj;

in vec3 inPosition;
in vec4 inColor;
in vec2 inUV;

// Name must match PS IN
out vec4 passColor;
out vec2 passUV;

void main( void )
{
	vec4 pos = vec4(inPosition, 1.0f);
	pos = pos * gView * gProj;
	//pos =  gProj* gView * pos;

	passColor = inColor;
    passUV = inUV;
	gl_Position = pos;
}