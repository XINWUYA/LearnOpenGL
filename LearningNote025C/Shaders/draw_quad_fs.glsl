#version 430 core

out vec4 FragColor;

in vec2 o_TexCoords;
uniform sampler2D u_Texture;

void main()
{
	vec2 Color = texture(u_Texture, o_TexCoords).rg;
	FragColor = vec4(Color, 0.0f, 1.0f);
	//FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}