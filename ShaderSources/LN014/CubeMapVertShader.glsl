#version 430 core

layout(location = 0) in vec3 Pos;

out vec3 oTexCoord;

uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
	vec4 Position = uProjection * uView * vec4(Pos, 1.0f);
	gl_Position = Position.xyww;
	oTexCoord   = Pos;
}