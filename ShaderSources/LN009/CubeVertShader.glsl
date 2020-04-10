#version 430 core

layout(location = 0) in vec3 Pos;
layout(location = 1) in vec2 Tex;
layout(location = 2) in vec3 Normal;

out vec2 oTexCoord;
out vec3 oNormal;
out vec3 oFragPos;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
	gl_Position = uProjection * uView * uModel * vec4(Pos, 1.0f);
	oTexCoord   = Tex;
	oNormal		= mat3(transpose(inverse(uModel))) * Normal;
	oFragPos	= vec3(uModel * vec4(Pos, 1.0f));
}