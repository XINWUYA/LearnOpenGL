#version 430 core

out vec4 FragColor;

in vec2 v2f_TexCoord;
in vec3 v2f_FragPos;
in vec3 v2f_Normal;

uniform vec3 uCameraPos;
uniform sampler2D uTexture;

vec3 LightPos = vec3(0.0, 1.0, -3.0);
vec3 LightColor = vec3(1.0);

void main()
{
	vec3 Normal = normalize(v2f_Normal); 
	float AmbientStrength = 0.1;
	vec3 TextureColor = vec3(texture(uTexture, v2f_TexCoord));

	vec3 LightDir = normalize(LightPos - v2f_FragPos);
	vec3 ViewDir = normalize(uCameraPos - v2f_FragPos);
	vec3 HalfwayDir = normalize(LightDir + ViewDir);

	vec3 Ambient = AmbientStrength * LightColor;

	vec3 Diffuse = max(dot(Normal, LightDir), 0.0) * LightColor;

	float SpecularStrength = 1;
	vec3 ReflectDir = reflect(-LightDir, Normal);
	//vec3 Specular = SpecularStrength * pow(max(dot(Normal, HalfwayDir), 0.0), 32) * LightColor;

	vec3 Specular = SpecularStrength * pow(max(dot(ReflectDir, ViewDir), 0.0), 8) * LightColor;

	vec3 ResultColor = (Ambient + Diffuse + Specular) * TextureColor;

	FragColor = vec4(ResultColor, 1.0);
}