#version 460 core

const float ka = 0.2f;
const vec4 ma = vec4(0.2f, 0.2f, 0.2f, 1.0f);
const vec4 md = vec4(1.0f, 1.0f, 1.0f, 1.0f);
const vec4 ms = vec4(0.5f, 0.5f, 0.5f, 1.0f);
const float shi = 64.0f;
const float reflCoef = 1.0f;

in data {
	vec3 veye;      //Bump -> vtan
	vec3 light;		//Bump ->ltan
	vec2 texcoord;
	vec3 t;
	vec3 n;
}f;

uniform samplerCube sky;
uniform sampler2D golfballmap;
//uniform sampler2D bricks;

out vec4 color;

void main (void)
{
	vec3 vnorm = normalize(f.veye);
	vec3 nnorm = normalize(f.n);
	vec3 tnorm = normalize(f.t);
	vec3 lnorm = normalize(f.light);
	vec3 b = cross(nnorm,tnorm);
	mat3 base = mat3(tnorm,b,nnorm);
	nnorm = base*(2*vec3(texture(golfballmap,f.texcoord)) - 1);
	float ndotl = dot(nnorm,lnorm);
	//vec3 dir = transpose(f.base) * normalize(reflect(-vnorm,nnorm)*vec3(1,1,-1));
	vec3 dir = normalize(reflect(-vnorm,nnorm)*vec3(1,1,-1));
	//color = (ma * ka + md * max(0,ndotl))*texture(sky,f.texcoord);
	color = min(1,(ka + max(0,ndotl)))*((1-reflCoef)*md + reflCoef*texture(sky,dir));
	if (ndotl > 0) 
	{
		vec3 refl = normalize(reflect(-lnorm,nnorm));
		color += ms * pow(max(0,dot(refl,vnorm)),shi);
	}
}