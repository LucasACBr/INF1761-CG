#version 460 core

uniform float ka = 0.2f;
uniform vec4 ma = vec4(1.0f, 1.0f, 0.0f, 1.0f);
uniform vec4 md = vec4(0.0f, 0.0f, 1.0f, 1.0f);
uniform vec4 ms = vec4(0.0f, 0.0f, 1.0f, 0.0f);
uniform float shi = 128.0f;

in data {
	vec3 neye;
	vec3 veye;
	vec3 light;
	vec2 texcoord;
}f;


uniform sampler2D earth;

out vec4 color;

void main (void)
{
	vec3 vnorm = normalize(-f.light);
	vec3 nnorm = normalize(f.neye);
	vec3 lnorm = normalize(f.light);
	float ndotl = dot(nnorm,lnorm);
	color = (ma * ka + md * max(0,ndotl))*texture(earth,f.texcoord);// inserir para textura ->
	if (ndotl > 0) {
		vec3 refl = normalize(reflect(-lnorm,nnorm));
		color += ms * pow(max(0,dot(refl,vnorm)),shi);
	}
}
