#version 430 core

out vec4 FragColor;

in vec2 v2f_TexCoord;

uniform sampler2D uShadowMapTex;

void main()
{
	float DepthValue = texture(uShadowMapTex, v2f_TexCoord).r;
	FragColor = vec4(vec3(DepthValue), 1.0);
}