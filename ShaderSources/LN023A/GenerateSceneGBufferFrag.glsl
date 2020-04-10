#version 430 core

layout(location = 0) out vec3 g_FragPosition;
layout(location = 1) out vec3 g_FragNormal;
layout(location = 2) out vec4 g_FragAlbedoSpec;

in vec2 oTexCoord;
in vec3 oNormal;
in vec3 oFragPos;

struct SMaterial
{
	sampler2D m_Diffuse0;
	sampler2D m_Specular0;
	sampler2D m_Ambient0;
	float m_Shininess;
};

uniform SMaterial Material;

void main()
{
	g_FragPosition = oFragPos;
	g_FragNormal = normalize(oNormal);
	g_FragAlbedoSpec.rgb = texture(Material.m_Diffuse0, oTexCoord).rgb;
	g_FragAlbedoSpec.a = texture(Material.m_Specular0, oTexCoord).r;
}