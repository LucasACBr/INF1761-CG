#version 460 core

layout (quads) in;

#define pi 3.14159265

//const vec3 leye = vec3(-0.30f, 1.00f, 0.55f); light pos in eye space
const vec3 lobj = vec3(-0.30f, 1.00f, 0.55f); // light pos in eye space
uniform mat4 mvp;
uniform mat4 inv;

patch in vec3 pos;
patch in float radius;

out data{
	vec3 veye;      //Bump -> vtan
	vec3 light;		//Bump ->ltan
	vec2 texcoord;
	vec3 t;
	vec3 n;
}v;

void main(void)
{

	float theta = 2*pi*gl_TessCoord.x;
	float phi = pi*gl_TessCoord.y;
	vec4 vpos;
	vpos.x = pos.x + radius*sin(theta)*sin(phi);
	vpos.y = pos.y +radius*cos(phi);
	vpos.z = pos.z + radius*cos(theta)*sin(phi);
	vpos.w = 1.0f;

	v.n.x=sin(theta)*sin(phi);
	v.n.y=cos(phi);
	v.n.z=cos(theta)*sin(phi);

	v.t.x = (2 * pi * cos(theta) * sin(phi)); 
    v.t.y = 0;
    v.t.z = (-2 * pi * sin(theta) * sin(phi));

	vec4 eye = inv * vec4(0.0f,0.0f,0.0f,1.0f);
	v.veye = normalize(vec3(eye-vpos));
	//vec4 lobj = inv * leye;

	//v.light = normalize(vec3(lobj)-vec3(vpos));
	//v.veye = normalize(vec3(eye)-vec3(vpos));
	//if (leye.w == 0)---------------------------------------> Sol logo não occore condicional
		v.light = normalize(vec3(lobj));
	//else
	//	v.light = normalize(vec3(lobj) - v.veye);

	v.texcoord = vec2(gl_TessCoord);
	gl_Position = mvp*vpos;
}
