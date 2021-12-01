#version 460 core

layout(location = 0) in vec4 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;


uniform vec4 leye; //light pos in eyespace
uniform mat4 mv;
uniform mat4 nm;
uniform mat4 mvp;

out data {
	vec3 neye;
	vec3 veye;
	vec3 light;
	vec2 texcoord;
}v;


void main(void)
{
	v.veye = vec3(mv*pos);
	if (leye.w == 0)
		v.light = normalize(vec3(leye));
	else
		v.light = normalize(vec3(leye)-v.veye);
	v.neye = normalize(vec3(nm*vec4(normal,0.0f)));
	v.texcoord = texcoord;
	gl_Position = mvp*pos;
}
