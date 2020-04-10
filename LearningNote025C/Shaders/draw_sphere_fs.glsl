#version 430 core
out vec4 FragColor;

in vec3 o_WorldPos;
in vec2 o_TexCoords;
in vec3 o_Normal;

uniform vec3 u_Albedo;
uniform float u_AO;
uniform float u_Metallic;
uniform float u_Roughness;
uniform vec3 u_LightPositionArray[4];
uniform vec3 u_LightColorArray[4];

uniform vec3 u_CameraPos;
uniform samplerCube u_IrradianceMap;
uniform samplerCube u_PrefilteredMap;
uniform sampler2D u_BRDFMap;

const float PI = 3.14159265359;

/*
*菲涅尔方程：返回物体表面光线被反射的百分比（也就是反射方程中的参数Ks)
*vF0: 0°入射角的反射：表示如果直接（垂直）观察表面时有多少光线会被反射，因材料而不同
*/
vec3 FresnelSchlick(float vCosTheta, vec3 vF0)
{
	return vF0 + (1.0f - vF0) * pow(1.0f - vCosTheta, 5.0f);
}

vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
	return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

float DistributionGGX(vec3 vN, vec3 vH, float vRoughness)
{
	float a = vRoughness * vRoughness;
	float a2 = a * a;
	float NdotH = max(dot(vN, vH), 0.0f);
	float NdotH2 = NdotH * NdotH;

	float Nom = a2;
	float Denom = (NdotH2 * (a2 - 1.0f) + 1.0f);
	Denom = PI * Denom * Denom;

	return Nom / Denom;
}

float GeometrySchlickGGX(float vNdotV, float vRoughness)
{
	float r = (vRoughness + 1.0);
	float k = (r*r) / 8.0;

	float Nom = vNdotV;
	float Denom = vNdotV * (1.0 - k) + k;

	return Nom / Denom;
}

float GeometrySmith(vec3 vN, vec3 vV, vec3 vL, float vRoughness)
{
	float NdotV = max(dot(vN, vV), 0.0);
	float NdotL = max(dot(vN, vL), 0.0);
	float GGX2 = GeometrySchlickGGX(NdotV, vRoughness);
	float GGX1 = GeometrySchlickGGX(NdotL, vRoughness);

	return GGX1 * GGX2;
}

void main()
{
	vec3 Normal = normalize(o_Normal);
	vec3 ViewDir = normalize(u_CameraPos - o_WorldPos);
	vec3 ReflectDir = reflect(-ViewDir, Normal);
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, u_Albedo, u_Metallic);
	
	const float MAX_REFLECTION_LOD = 4.0f;
	vec3 PrefilteredColor = textureLod(u_PrefilteredMap, ReflectDir, u_Roughness * MAX_REFLECTION_LOD).rgb;

	vec3 ResultColor = vec3(0.0f);
	for (int i = 0; i < 4; ++i)
	{
		vec3 LightDir = normalize(u_LightPositionArray[i] - o_WorldPos);
		vec3 H = normalize(ViewDir + LightDir);//?怎么理解 Phong模型改进？

		float Distance = length(u_LightPositionArray[i] - o_WorldPos);
		float LightAttenuation = 1.0f / (Distance * Distance);
		vec3 LightRadiance = u_LightColorArray[i] * LightAttenuation;

		vec3 F = FresnelSchlick(max(dot(H, ViewDir), 0.0f), F0);
		float NDF = DistributionGGX(Normal, H, u_Roughness);
		float G = GeometrySmith(Normal, ViewDir, LightDir, u_Roughness);

		vec3 Specular = NDF * G * F / (4 * max(dot(Normal, ViewDir), 0.0f) * max(dot(Normal, LightDir), 0.0f) + 0.001);//0.001是为了避免除0

		vec3 Ks = F;
		vec3 Kd = vec3(1.0) - Ks;
		Kd *= 1.0 - u_Metallic;

		float NormaldotLightDir = max(dot(Normal, LightDir), 0.0f);
		ResultColor += (Kd * u_Albedo / PI + Specular) * LightRadiance * NormaldotLightDir;
	}

	vec3 Ks = FresnelSchlickRoughness(max(dot(Normal, ViewDir), 0.0f), F0, u_Roughness);
	vec3 Kd = vec3(1.0) - Ks;
	Kd *= 1.0 - u_Metallic;
	vec3 Irradiance = texture(u_IrradianceMap, Normal).rgb;
	vec3 Diffuse = Irradiance * u_Albedo;

	vec2 EnvironmentBRDF = texture(u_BRDFMap, vec2(max(dot(Normal, ViewDir), 0.0f), u_Roughness)).rg;
	vec3 Specular = PrefilteredColor * (Ks * EnvironmentBRDF.x + EnvironmentBRDF.y);

	vec3 AmbientColor = (Kd * Diffuse + Specular) * u_AO;
	ResultColor += AmbientColor;

	//Gamma Correction
	ResultColor = ResultColor / (ResultColor + vec3(1.0f));
	ResultColor = pow(ResultColor, vec3(1.0f / 2.2f));

	FragColor = vec4(ResultColor, 1.0);
	//FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}