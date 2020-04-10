#version 430 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VS_OUT{
	vec2 m_TexCoord;
	vec3 m_Normal;
	vec3 m_FragPos;
} Gs_In[];

out vec2 oTexCoord;
out vec3 oNormal;
out vec3 oFragPos;

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

	oTexCoord = Gs_In[0].m_TexCoord;
	oNormal  = Gs_In[0].m_Normal;
	oFragPos = Gs_In[0].m_FragPos;
	gl_Position = explode(gl_in[0].gl_Position, Normal);
	EmitVertex();

	oTexCoord = Gs_In[1].m_TexCoord;
	oNormal  = Gs_In[1].m_Normal;
	oFragPos = Gs_In[1].m_FragPos;
	gl_Position = explode(gl_in[1].gl_Position, Normal);
	EmitVertex();

	oTexCoord = Gs_In[2].m_TexCoord;
	oNormal  = Gs_In[2].m_Normal;
	oFragPos = Gs_In[2].m_FragPos;
	gl_Position = explode(gl_in[2].gl_Position, Normal);
	EmitVertex();

	EndPrimitive();
}