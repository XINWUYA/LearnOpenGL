#version 430 core

out vec2 FragColor;

in vec2 o_TexCoords;

const float PI = 3.14159265359;

float RadicalInverseVanDerCorpus(uint vBits)
{
	vBits = (vBits << 16u) | (vBits >> 16u);
	vBits = ((vBits & 0x55555555u) << 1u) | ((vBits & 0xAAAAAAAAu) >> 1u);
	vBits = ((vBits & 0x33333333u) << 2u) | ((vBits & 0xCCCCCCCCu) >> 2u);
	vBits = ((vBits & 0x0F0F0F0Fu) << 4u) | ((vBits & 0xF0F0F0F0u) >> 4u);
	vBits = ((vBits & 0x00FF00FFu) << 8u) | ((vBits & 0xFF00FF00u) >> 8u);
	return float(vBits) * 2.3283064365386963e-10; // / 0x100000000
}

vec2 Hammersley(uint vIndex, uint vTotalNum)
{
	return vec2(float(vIndex) / float(vTotalNum), RadicalInverseVanDerCorpus(vIndex));
}

vec3 ImportanceSampleGGX(vec2 vXi, vec3 vNormal, float vRoughness)
{
	float a = vRoughness * vRoughness;
	float Phi = 2.0f * PI * vXi.x;
	float CosTheta = sqrt((1.0 - vXi.y) / (1.0 + (a*a - 1.0) * vXi.y));
	float SinTheta = sqrt(1.0 - CosTheta * CosTheta);

	// from spherical coordinates to cartesian coordinates
	vec3 H;
	H.x = cos(Phi) * SinTheta;
	H.y = sin(Phi) * SinTheta;
	H.z = CosTheta;

	// from tangent-space vector to world-space sample vector
	vec3 Up = abs(vNormal.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
	vec3 Tangent = normalize(cross(Up, vNormal));
	vec3 Bitangent = cross(vNormal, Tangent);

	vec3 SampleVec = Tangent * H.x + Bitangent * H.y + vNormal * H.z;
	return normalize(SampleVec);
}

float GeometrySchlickGGX(float vNdotV, float vRoughness)
{
	float a = vRoughness;
	float k = (a * a) / 2.0;

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

vec2 IntegrateBRDF(float vNormalDotViewDir, float vRoughness)
{
	vec3 V;
	V.x = sqrt(1.0f - vNormalDotViewDir * vNormalDotViewDir);
	V.y = 0.0f;
	V.z = vNormalDotViewDir;

	float A = 0.0f;
	float B = 0.0f;
	vec3 N = vec3(0.0f, 0.0f, 1.0f);
	const uint SAMPLE_COUNT = 1024u;
	for (uint i = 0u; i < SAMPLE_COUNT; ++i)
	{
		vec2 Xi = Hammersley(i, SAMPLE_COUNT);
		vec3 H = ImportanceSampleGGX(Xi, N, vRoughness);
		vec3 L = normalize(2.0f * dot(V, H) * H - V);

		float NdotL = max(L.z, 0.0f);
		float NdotH = max(H.z, 0.0f);
		float VdotH = max(dot(V, H), 0.0f);

		if (NdotL > 0.0f)
		{
			float G = GeometrySmith(N, V, L, vRoughness);
			float G_Vis = (G * VdotH) / (NdotH * vNormalDotViewDir);
			float Fc = pow(1.0f - VdotH, 5.0f);

			A += (1.0f - Fc) * G_Vis;
			B += Fc * G_Vis;
		}
	}

	A /= float(SAMPLE_COUNT);
	B /= float(SAMPLE_COUNT);

	return vec2(A, B);
}

void main()
{
	vec2 IntegrateBRDF = IntegrateBRDF(o_TexCoords.x, o_TexCoords.y);
	FragColor = IntegrateBRDF;
}