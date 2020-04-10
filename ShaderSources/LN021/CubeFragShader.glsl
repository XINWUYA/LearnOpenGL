#version 430 core

out vec4 FragColor;

in vec2 v2f_TexCoord;
in vec3 v2f_FragPos;
in vec3 v2f_Normal;

uniform sampler2D uTexture;
uniform samplerCube u_ShadowMapTex;
uniform float u_FarPlane;
uniform vec3 uCameraPos;
uniform vec3 u_LightPos;
uniform bool u_IsScence = false;

vec3 LightColor = vec3(1.0);

float ShadowCalculation(vec3 fragPos)
{
	// Get vector between fragment position and light position
	vec3 fragToLight = fragPos - u_LightPos;
	// Use the light to fragment vector to sample from the depth map    
	float closestDepth = texture(u_ShadowMapTex, normalize(fragToLight)).r;
	// Now test for shadows
	float bias = 0.05;
	float shadow = length(fragToLight) - bias > closestDepth * u_FarPlane ? 1.0 : 0.0;

	return shadow;
}

void main()
{
	vec3 Normal = normalize(v2f_Normal);
	if (u_IsScence)
		Normal = -Normal;

	float AmbientStrength = 0.1;
	vec3 AlbedoColor = vec3(1.0f);

	vec3 LightDir = normalize(u_LightPos - v2f_FragPos);
	vec3 ViewDir = normalize(uCameraPos - v2f_FragPos);
	vec3 HalfwayDir = normalize(LightDir + ViewDir);

	vec3 Ambient = AmbientStrength * LightColor;

	vec3 Diffuse = max(dot(Normal, LightDir), 0.0) * LightColor;

	float SpecularStrength = 1;
	vec3 ReflectDir = reflect(-LightDir, Normal);
	vec3 Specular = SpecularStrength * pow(max(dot(Normal, HalfwayDir), 0.0), 32) * LightColor;

	float Shadow = ShadowCalculation(v2f_FragPos);
	vec3 ResultColor = (Ambient + (1.0 - Shadow) * (Diffuse + Specular)) * AlbedoColor;

	FragColor = vec4(ResultColor, 1.0);
	//FragColor = vec4(vec3(texture(u_ShadowMapTex, normalize(v2f_FragPos)).g), 1.0);
	//FragColor = vec4(normalize(vec3(v2f_FragPos)), 1.0);
}