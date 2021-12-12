#version 460 core

layout(location = 0) in vec4 pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
//uniform mat4 mvp;
//uniform mat4 svp;


void main(void)
{
	mat4 mvp = proj * view * model;
	gl_Position = mvp*pos;
}