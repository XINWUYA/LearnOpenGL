#version 430 core

out vec4 gl_FragColor;

in vec2 oTexCoord;
in vec3 oNormal;
in vec3 oFragPos;

uniform vec3 uCameraPos;

struct SLight
{
	vec3 m_Position;
	vec3 m_Direction;
	float m_InnerCutOffAngle;
	float m_OuterCutOffAngle;
	vec3 m_Ambient;
	vec3 m_Diffuse;
	vec3 m_Specular;
	//Light Attenuation
	float m_Constant;
	float m_Liner;
	float m_Quadratic;
};

struct SMaterial
{
	sampler2D m_Diffuse;
	sampler2D m_Specular;
	float m_Shininess;
};

uniform SLight Light;
uniform SMaterial Material;

void main()
{
	float LightDistance = length(Light.m_Position - oFragPos);
	float Attenuation = 1.0f / (Light.m_Constant + Light.m_Liner * LightDistance + Light.m_Quadratic * LightDistance * LightDistance);
	
	vec3 Ambient = Light.m_Ambient * vec3(texture(Material.m_Diffuse, oTexCoord));

	vec3 Normal = normalize(oNormal);
	vec3 LightDir = normalize(Light.m_Position - oFragPos);
	float Diff = max(dot(Normal, LightDir), 0.0f);
	vec3 Diffuse = Light.m_Diffuse * (Diff * vec3(texture(Material.m_Diffuse, oTexCoord)));

	vec3 ViewDir = normalize(uCameraPos - oFragPos);
	vec3 ReflectDir = reflect(-LightDir, Normal);
	float Spec = pow(max(dot(ViewDir, ReflectDir), 0.0f), Material.m_Shininess);
	vec3 Specular = Light.m_Specular * (Spec * vec3(texture(Material.m_Specular, oTexCoord)));

	float Theta = dot(LightDir, normalize(-Light.m_Direction));
	float Intensity = clamp((Theta - Light.m_OuterCutOffAngle) / (Light.m_InnerCutOffAngle - Light.m_OuterCutOffAngle), 0.0f, 1.0f);

	if(Theta > Light.m_OuterCutOffAngle)
	{
		vec3 Result = (Ambient + (Diffuse  + Specular) * Intensity) * Attenuation;
		gl_FragColor = vec4(Result, 1.0f);
	}
	else
		gl_FragColor = vec4(Ambient, 1.0f);
}