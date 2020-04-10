#version 430 core

out vec4 gl_FragColor;

in vec2 oTexCoord;
in vec3 oNormal;
in vec3 oFragPos;

uniform vec3 uCameraPos;

uniform sampler2D uTexture;
uniform sampler2D uTexture1;
uniform samplerCube uCubeMap;

void main()
{
	//∑¥…‰
	/*vec3 ViewDir = normalize(oFragPos - uCameraPos);
	vec3 ReflectDir = reflect(ViewDir, normalize(oNormal));
	gl_FragColor = vec4(texture(uCubeMap, ReflectDir).rgb, 1.0f);*/
	//gl_FragColor = texture(uTexture, oTexCoord) * texture(uTexture1, oTexCoord);

	//’€…‰
	float RefractRatio = 1.00f / 1.52f;
	vec3 ViewDir = normalize(oFragPos - uCameraPos);
	vec3 RefractDir = refract(ViewDir, normalize(oNormal), RefractRatio);
	gl_FragColor = vec4(texture(uCubeMap, RefractDir).rgb, 1.0f);
}