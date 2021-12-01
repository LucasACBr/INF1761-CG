#version 460 core

layout(location = 0) in vec4 geom;

out vec4 pgeom;


void main(void)
{
	pgeom = geom;
}

