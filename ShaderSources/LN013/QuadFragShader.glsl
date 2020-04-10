#version 430 core

out vec4 gl_FragColor;

in vec2 oTexCoord;

uniform sampler2D uTexture;

const float Offset = 1.0f / 300.0f;

void main()
{
	//vec4 FragColor = texture(uTexture, oTexCoord);

	//gl_FragColor = FragColor;//原图
	//gl_FragColor = vec4(vec3(1 - FragColor), 1.0f);//反相
	//gl_FragColor = vec4(vec3((FragColor.r + FragColor.g + FragColor.b) / 3.0f), 1.0f);//灰度图
	//gl_FragColor = vec4(vec3(0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b), 1.0f);//带权灰度图

	//锐化核
	vec2 Offsets[9] = vec2[](
		vec2(-Offset,  Offset), //Left-Top
		vec2(-Offset,    0.0f), //Left-Center
		vec2(-Offset, -Offset), //Left-Bottom
		vec2(   0.0f,  Offset), //Top-Center
		vec2(   0.0f,    0.0f), //Center-Center
		vec2(   0.0f, -Offset), //Bottom-Center
		vec2( Offset,  Offset), //Right-Top
		vec2( Offset,    0.0f), //Right-Center
		vec2( Offset, -Offset)  //Right-Bottom
		);

	float SharpenKernel[9] = float[](
		-1.0f, -1.0f, -1.0f,
		-1.0f,  9.0f, -1.0f,
		-1.0f, -1.0f, -1.0f
		);
	float BlurKernel[9] = float[](
		1.0f, 2.0f, 1.0f,
		2.0f, 4.0f, 2.0f,
		1.0f, 2.0f, 1.0f
		);
	float EdgeDetectionKernel[9] = float[](
		1.0f,  1.0f, 1.0f,
		1.0f, -8.0f, 1.0f,
		1.0f,  1.0f, 1.0f
		);
	vec3 SampleTexture[9];
	for (int i = 0; i < 9; i++)
	{
		SampleTexture[i] = vec3(texture(uTexture, oTexCoord.st + Offsets[i]));
	}
	vec3 Result = vec3(0.0f);
	for (int i = 0; i < 9; i++)
	{
		//Result += SampleTexture[i] * SharpenKernel[i];//锐化核
		//Result += SampleTexture[i] * BlurKernel[i] / 16.0f;//模糊
		Result += SampleTexture[i] * EdgeDetectionKernel[i];//边缘检测
	}

	gl_FragColor = vec4(Result, 1.0f);
}