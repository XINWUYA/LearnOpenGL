#version 430 core

out vec4 FragColor;

in vec3 o_WorldPos;

uniform samplerCube u_EnvironmentMap;

const float PI = 3.14159265359;

void main()
{
	vec3 Normal = normalize(o_WorldPos);

	vec3 Irradiance = vec3(0.0f);
	vec3 Up = vec3(0.0f, 1.0f, 0.0f);
	vec3 Right = cross(Up, Normal);
	Up = cross(Normal, Right);

	float SampleDelta = 0.025f;
	float SampleCnt = 0.0f;
	for (float Phi = 0.0f; Phi < 2.0f * PI; Phi += SampleDelta)
	{
		for (float Theta = 0.0f; Theta < 0.5f * PI; Theta += SampleDelta)
		{
			//在切线空间中，将球面坐标转换到笛卡尔采样集
			vec3 TangentSample = vec3(sin(Theta) * cos(Phi), sin(Theta) * sin(Phi), cos(Theta));
			//将采样从切线空间转换到世界空间中
			vec3 SampleVec = TangentSample.x * Right + TangentSample.y * Up + TangentSample.z * Normal;
			Irradiance += texture(u_EnvironmentMap, SampleVec).rgb * cos(Theta) * sin(Theta);
			SampleCnt++;
		}
	}
	Irradiance = PI * Irradiance * (1.0f / SampleCnt);
	FragColor = vec4(Irradiance, 1.0f);
}