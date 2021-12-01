#version 460 core

layout(vertices = 1) out;

in vec4 pgeom[ ];
patch out vec3 pos;
patch out float radius;

void main(void)
{
	pos = vec3(pgeom[gl_InvocationID]);
	radius = pgeom[gl_InvocationID].w;
	gl_TessLevelOuter[0] = 64;
	gl_TessLevelOuter[1] = 64;
	gl_TessLevelOuter[2] = 64;
	gl_TessLevelOuter[3] = 64;
	gl_TessLevelInner[0] = 64;
	gl_TessLevelInner[1] = 64;
}
