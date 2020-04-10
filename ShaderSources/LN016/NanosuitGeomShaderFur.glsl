#version 430 core

layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

in VS_OUT{
	vec2 m_TexCoord;
	vec3 m_Normal;
	vec3 m_FragPos;
} Gs_In[];

uniform float uTime;

vec4 explode(vec4 vPosition, vec3 vNormal)
{
	float Magnitude = 2.0f;
	vec3 Direction = vNormal * Magnitude * ((sin(uTime) + 1.0f) / 2.0);
	return vPosition + vec4(Direction, 0.0);
}

vec3 getNormal()
{
	vec3 Dir1_0 = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 Dir1_2 = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
	return normalize(cross(Dir1_0, Dir1_2));
}

void main()
{
	vec3 Normal = getNormal();

	gl_Position = explode(gl_in[0].gl_Position, Normal);
	EmitVertex();
	gl_Position = explode(gl_in[0].gl_Position, Normal) + vec4(Normal, 0.0f) * 0.1;
	EmitVertex();
	EndPrimitive();

	gl_Position = explode(gl_in[1].gl_Position, Normal);
	EmitVertex();
	gl_Position = explode(gl_in[1].gl_Position, Normal) + vec4(Normal, 0.0f) * 0.1;
	EmitVertex();
	EndPrimitive();

	gl_Position = explode(gl_in[2].gl_Position, Normal);
	EmitVertex();
	gl_Position = explode(gl_in[2].gl_Position, Normal) + vec4(Normal, 0.0f) * 0.1;
	EmitVertex();
	EndPrimitive();
}