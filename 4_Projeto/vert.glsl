#version 460 core

layout(location = 0) in vec4 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;


uniform vec4 leye; //light pos in eyespace
uniform mat4 mvp;
uniform mat4 svp;
uniform mat4 model;
uniform mat4 view;

out data {
	vec3 normal;
	vec3 veye;      //Bump -> vtan
	vec3 light;		//Bump ->ltan
	vec2 texcoord;
	vec4 stexcoord;
}v;

void main(void)
{
	
	vec3 peye = vec3(view*model*pos);
	if (leye.w == 0)
		v.light = normalize(vec3(leye));
	else
		v.light = normalize(vec3(leye)-peye);

	v.veye = - peye;
	mat4 nm = transpose(inverse(model*view));
	//mat4 nm = transpose((model*view));
	v.normal = normalize(vec3(nm*vec4(normal,0.0f)));
	v.texcoord = texcoord;
	v.stexcoord = svp*pos;
	gl_Position = mvp*pos;
}
