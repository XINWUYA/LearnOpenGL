#version 430 core

layout(location = 0) in vec3 Pos;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoords;
layout(location = 3) in mat4 InstanceMatrices;

out vec2 oTexCoord;
out vec3 oNormal;
out vec3 oFragPos;

uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
	gl_Position = uProjection * uView * InstanceMatrices * vec4(Pos, 1.0f);
	oTexCoord   = TexCoords;
	oNormal		= mat3(transpose(inverse(InstanceMatrices))) * Normal;
	oFragPos	= vec3(InstanceMatrices * vec4(Pos, 1.0f));
}