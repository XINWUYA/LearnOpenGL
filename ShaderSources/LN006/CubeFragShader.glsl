#version 430 core

out vec4 gl_FragColor;

in vec2 oTexCoord;
in vec3 oNormal;
in vec3 oFragPos;

uniform vec3 uCameraPos;

struct SLight
{
	vec3 m_Position;
	vec3 m_Ambient;
	vec3 m_Diffuse;
	vec3 m_Specular;
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
	vec3 Ambient = Light.m_Ambient * vec3(texture(Material.m_Diffuse, oTexCoord));

	vec3 Normal = normalize(oNormal);
	vec3 LightDir = normalize(Light.m_Position - oFragPos);
	float Diff = max(dot(Normal, LightDir), 0.0f);
	vec3 Diffuse = Light.m_Diffuse * (Diff * vec3(texture(Material.m_Diffuse, oTexCoord)));

	vec3 ViewDir = normalize(uCameraPos - oFragPos);
	vec3 ReflectDir = reflect(-LightDir, Normal);
	float Spec = pow(max(dot(ViewDir, ReflectDir), 0.0f), Material.m_Shininess);
	vec3 Specular = Light.m_Specular * (Spec * vec3(texture(Material.m_Specular, oTexCoord)));

	vec3 Result = Ambient + Diffuse + Specular;
	gl_FragColor = vec4(Result, 1.0f);
	//gl_FragColor = vec4(Result, 1.0f) * texture(uTexture, oTexCoord) * texture(uTexture1, oTexCoord);
}