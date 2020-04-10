#version 430 core
out vec4 FragColor;

in vec3 o_WorldPos;

uniform samplerCube u_EnvironmentMap;

void main()
{
	//vec3 EnvColor = texture(u_EnvironmentMap, o_WorldPos).rgb;
	vec3 EnvColor = textureLod(u_EnvironmentMap, o_WorldPos, 1.2).rgb;

	// HDR tonemap and gamma correct
	EnvColor = EnvColor / (EnvColor + vec3(1.0));
	EnvColor = pow(EnvColor, vec3(1.0 / 2.2));

	FragColor = vec4(EnvColor, 1.0);
}