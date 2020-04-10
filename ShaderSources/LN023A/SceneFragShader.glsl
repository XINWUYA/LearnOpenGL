#version 430 core
out vec4 gl_FragColor;

in vec2 oTexCoord;
in vec3 oNormal;
in vec3 oFragPos;

uniform sampler2D uPositionTex;
uniform sampler2D uNormalTex;
uniform sampler2D uAlbedoSpecTex;

uniform vec3 uLightPos;
uniform vec3 uCameraPos;
uniform float uIntensity = 10.0f;

void main()
{
	vec3 FragPos = texture(uPositionTex, oTexCoord).xyz;
	vec3 Normal = texture(uNormalTex, oTexCoord).xyz;
	vec4 AlbedoSpec = texture(uAlbedoSpecTex, oTexCoord);

	vec3 LightDir = uLightPos - FragPos;
	vec3 ViewDir = normalize(uCameraPos - FragPos);

	float DistanceFrag2Light_2 = dot(LightDir, LightDir);

	vec3 AmbientColor = 0.3f * AlbedoSpec.rgb;
	vec3 DiffuseColor = max(dot(normalize(LightDir), Normal), 0.0f) * AlbedoSpec.rgb * uIntensity / (1.0f + DistanceFrag2Light_2);
	
	gl_FragColor = vec4(AmbientColor + DiffuseColor, 1.0f);
}


//#version 430 core
//
//out vec4 gl_FragColor;
//
//struct SMaterial
//{
//	sampler2D m_Diffuse0;
//	sampler2D m_Specular0;
//	sampler2D m_Ambient0;
//	float m_Shininess;
//};
//
//struct SDirectLight
//{
//	vec3 m_Direction;
//
//	vec3 m_Ambient;
//	vec3 m_Diffuse;
//	vec3 m_Specular;
//};
//
//struct SPointLight
//{
//	vec3 m_Position;
//
//	vec3 m_Ambient;
//	vec3 m_Diffuse;
//	vec3 m_Specular;
//
//	float m_Constant;
//	float m_Liner;
//	float m_Quadratic;
//};
//
//struct SSpotLight
//{
//	vec3 m_Position;
//	vec3 m_Direction;
//	float m_InnerCutOffAngle;
//	float m_OuterCutOffAngle;
//
//	vec3 m_Ambient;
//	vec3 m_Diffuse;
//	vec3 m_Specular;
//
//	float m_Constant;
//	float m_Liner;
//	float m_Quadratic;
//};
//
//#define NUM_POINT_LIGHT 4
//
//in vec2 oTexCoord;
//in vec3 oNormal;
//in vec3 oFragPos;
//
//uniform vec3 uCameraPos;
//
//uniform SMaterial Material;
//uniform SDirectLight DirectLight;
//uniform SPointLight PointLights[NUM_POINT_LIGHT];
//uniform SSpotLight SpotLight;
//uniform sampler2D uAlbedoTex;
//uniform sampler2D uNormalTex;
//
//vec3 calculateDirectLight(SDirectLight vLight, vec3 vNormal, vec3 vViewDir)
//{
//	vec3 Ambient = vLight.m_Ambient * vec3(texture(Material.m_Diffuse0, oTexCoord));
//
//	vec3 LightDir = normalize(-vLight.m_Direction);
//	float Diff = max(dot(vNormal, LightDir), 0.0f);
//	vec3 Diffuse = vLight.m_Diffuse * (Diff * vec3(texture(Material.m_Diffuse0, oTexCoord)));
//
//	vec3 ReflectDir = reflect(-LightDir, vNormal);
//	float Spec = pow(max(dot(vViewDir, ReflectDir), 0.0f), Material.m_Shininess);
//	vec3 Specular = vLight.m_Specular * (Spec * vec3(texture(Material.m_Specular0, oTexCoord)));
//
//	return Ambient + Diffuse + Specular;
//}
//
//vec3 calculatePointLight(SPointLight vLight, vec3 vNormal, vec3 vFragPos, vec3 vViewDir)
//{
//	vec3 Ambient = vLight.m_Ambient * vec3(texture(Material.m_Diffuse0, oTexCoord));
//
//	vec3 LightDir = normalize(vLight.m_Position - vFragPos);
//	float Diff = max(dot(vNormal, LightDir), 0.0f);
//	vec3 Diffuse = vLight.m_Diffuse * (Diff * vec3(texture(Material.m_Diffuse0, oTexCoord)));
//
//	vec3 ReflectDir = reflect(-LightDir, vNormal);
//	float Spec = pow(max(dot(vViewDir, ReflectDir), 0.0f), Material.m_Shininess);
//	vec3 Specular = vLight.m_Specular * (Spec * vec3(texture(Material.m_Specular0, oTexCoord)));
//
//	float LightDistance = length(vLight.m_Position - vFragPos);
//	float Attenuation = 1.0f / (vLight.m_Constant + vLight.m_Liner * LightDistance + vLight.m_Quadratic * LightDistance * LightDistance);
//
//	return (Ambient + Diffuse + Specular) * Attenuation;
//}
//
//vec3 calculateSpotLight(SSpotLight vLight, vec3 vNormal, vec3 vFragPos, vec3 vViewDir)
//{
//	vec3 Ambient = vLight.m_Ambient * vec3(texture(Material.m_Diffuse0, oTexCoord));
//
//	vec3 LightDir = normalize(vLight.m_Position - vFragPos);
//	float Diff = max(dot(vNormal, LightDir), 0.0f);
//	vec3 Diffuse = vLight.m_Diffuse * (Diff * vec3(texture(Material.m_Diffuse0, oTexCoord)));
//
//	vec3 ReflectDir = reflect(-LightDir, vNormal);
//	float Spec = pow(max(dot(vViewDir, ReflectDir), 0.0f), Material.m_Shininess);
//	vec3 Specular = vLight.m_Specular * (Spec * vec3(texture(Material.m_Specular0, oTexCoord)));
//
//	float LightDistance = length(vLight.m_Position - vFragPos);
//	float Attenuation = 1.0f / (vLight.m_Constant + vLight.m_Liner * LightDistance + vLight.m_Quadratic * LightDistance * LightDistance);
//
//	float Theta = dot(LightDir, normalize(-vLight.m_Direction));
//	float Intensity = clamp((Theta - vLight.m_OuterCutOffAngle) / (vLight.m_InnerCutOffAngle - vLight.m_OuterCutOffAngle), 0.0f, 1.0f);
//
//	return (Ambient + Diffuse + Specular) * Attenuation * Intensity;
//}
//
//void main()
//{
//	vec3 Normal = normalize(oNormal);
//	vec3 ViewDir = normalize(uCameraPos - oFragPos);
//	vec3 Result = calculateDirectLight(DirectLight, Normal, ViewDir);
//	for (int i = 0; i < NUM_POINT_LIGHT; i++)
//		Result += calculatePointLight(PointLights[i], Normal, oFragPos, ViewDir);
//
//	Result += calculateSpotLight(SpotLight, Normal, oFragPos, ViewDir);
//
//	//gl_FragColor = vec4(Result, 1.0f);
//	gl_FragColor = vec4(texture(uNormalTex, oTexCoord).rgb, 1.0f);
//	//Test gl_FragCoord
//	/*if(gl_FragCoord.x < 640)
//		gl_FragColor = vec4(Result * vec3(1.0f, 0.0f, 0.0f), 1.0f);
//	else
//		gl_FragColor = vec4(Result * vec3(0.0f, 0.0f, 1.0f), 1.0f);*/
//}