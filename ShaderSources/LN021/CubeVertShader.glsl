#version 430 core

layout(location = 0) in vec3 Pos;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 Tex;

out vec2 v2f_TexCoord;
out vec3 v2f_FragPos;
out vec3 v2f_Normal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
	gl_Position = uProjection * uView * uModel * vec4(Pos, 1.0f);
	v2f_TexCoord = Tex;
	v2f_FragPos = vec3(uModel * vec4(Pos, 1.0));
	v2f_Normal = mat3(transpose(inverse(uModel))) * Normal;
}