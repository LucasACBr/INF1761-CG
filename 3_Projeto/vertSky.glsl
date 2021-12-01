#version 460 core

layout(location = 0) in vec4 pos;

uniform mat4 mvp;

out vec3 dir;

void main(void)
{	
	dir = vec3(pos);
	gl_Position = mvp * pos;
}