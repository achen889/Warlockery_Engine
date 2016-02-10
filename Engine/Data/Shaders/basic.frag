#version 330 core

// modifiers for pass variables
// smooth [default], flat, noperspective

in vec4 passColor;
in vec2 passUV;

out vec4 outColor;

void main( void )
{
	outColor = passColor;
}