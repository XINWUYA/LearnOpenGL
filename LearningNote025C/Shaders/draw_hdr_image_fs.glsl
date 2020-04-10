#version 430 core

out vec4 FragColor;

in vec3 o_WorldPos;
//in vec2 oTexCoords;

uniform sampler2D u_EquirectangularMap;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
	vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
	uv *= invAtan;
	uv += 0.5;
	return uv;
}

void main()
{
	vec2 uv = SampleSphericalMap(normalize(o_WorldPos)); // make sure to normalize localPos
	vec3 color = texture(u_EquirectangularMap, uv).rgb;

	FragColor = vec4(color, 1.0);
	//FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0);
}