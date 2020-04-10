#version 430 core

layout(location = 0) in vec3 Pos;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoords;

out VS_OUT{
	vec2 m_TexCoord;
	vec3 m_Normal;
	vec3 m_FragPos;
} Vs_Out;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
	gl_Position = uProjection * uView * uModel * vec4(Pos, 1.0f);
	Vs_Out.m_TexCoord   = TexCoords;
	Vs_Out.m_Normal		= normalize(vec3(uProjection * vec4(mat3(transpose(inverse(uView * uModel))) *Normal, 0.0f)));
	Vs_Out.m_FragPos	= vec3(uModel * vec4(Pos, 1.0f));
}