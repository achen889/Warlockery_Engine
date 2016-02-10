#version 330 core

uniform float gTime;
uniform sampler2D gTexture;

// modifiers for pass variables
// smooth [default], flat, noperspective
in vec4 passColor;
in vec3 passPos;
in vec2 passUV;

out vec4 outColor;

const float cPI = 3.1456;

float blend1(float x){ 

	return x*x; 
}

void main( void )
{
   vec4 color = (passColor + vec4(1.0)) * .5f;
   float elapsedTime = .5 * gTime;
  
   vec2 uv = passUV; 

   vec4 texColor = texture( gTexture, uv );
 
 	//texColor = vec4(texColor.x);

	outColor = color * texColor;
}