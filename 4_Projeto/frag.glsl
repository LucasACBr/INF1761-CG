#version 460 core

const float ka = 0.4f;
uniform vec4 ma; //= vec4(0.2f, 0.2f, 0.2f, 1.0f);
uniform vec4 md; //= vec4(1.0f, 1.0f, 1.0f, 1.0f);
uniform vec4 ms; //= vec4(0.5f, 0.5f, 0.5f, 1.0f);
const float shi = 64.0f;

in data {
	vec3 normal;
	vec3 veye;      //Bump -> vtan
	vec3 light;		//Bump ->ltan
	vec2 texcoord;
	vec4 stexcoord;
}f;

uniform sampler2D smile;

out vec4 color;

void main (void)
{
	vec3 vnorm = normalize(f.veye);
	vec3 nnorm = normalize(f.normal);
	vec3 lnorm = normalize(f.light);
	float ndotl = dot(nnorm,lnorm);
	float kd = (1-ka)* max(0,dot(nnorm,lnorm));
	vec4 texel = textureProj(smile,f.stexcoord);
	vec3 lcolor = ((1 - texel.a) * vec3(1.0,1.0,1.0) + texel.a * vec3(texel));
	//vec3 lcolor = vec3(texel);
	color = vec4(ka * vec3(ma) + kd * lcolor * vec3(md),md.a);
	if (kd > 0) 
	{
		vec3 refl = normalize(reflect(-lnorm,nnorm));
		color += vec4(lcolor*vec3(ms)*pow(max(0,dot(refl,vnorm)),shi),1.0);
	}
}