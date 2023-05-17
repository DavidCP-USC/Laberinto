#version 330 core

uniform vec3 ourColor;

void main()

{
	gl_FragColor = vec4 (ourColor,1.0f);
}