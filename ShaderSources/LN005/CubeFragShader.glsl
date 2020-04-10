#version 430 core

out vec4 gl_FragColor;

in vec2 oTexCoord;
in vec3 oNormal;
in vec3 oFragPos;

uniform sampler2D uTexture;
uniform sampler2D uTexture1;

uniform vec3 uObjectColor;
uniform vec3 uLightColor;
uniform vec3 uLightPos;
uniform vec3 uCameraPos;

void main()
{
	float AmbientStrength = 0.5f;
	float SpecularStrength = 0.9f;

	vec3 Ambient = AmbientStrength * uLightColor;

	vec3 Normal = normalize(oNormal);
	vec3 LightDir = normalize(uLightPos - oFragPos);
	float Diff = max(dot(Normal, LightDir), 0.0f);
	vec3 Diffuse = Diff * uLightColor;

	vec3 ViewDir = normalize(uCameraPos - oFragPos);
	vec3 ReflectDir = reflect(-LightDir, Normal);
	float Spec = pow(max(dot(ViewDir, ReflectDir), 0.0f), 64);
	vec3 Specular = SpecularStrength * Spec * uLightColor;

	vec3 Result = (Ambient + Diffuse + Specular) * uObjectColor;
	gl_FragColor = vec4(Result, 1.0f) * texture(uTexture, oTexCoord) * texture(uTexture1, oTexCoord);
}