#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <GLM/glm.hpp>

enum EShaderType
{
	VERTEX_SHADER,
	FRAGMENT_SHADER,
	TESSELLATION_CONTROL_SHADER,
	TESSELLATION_EVALUATION_SHADER,
	GEOMETRY_SHADER
};

class CShader
{
public:
	CShader() {}
	~CShader();
	void addShader(const std::string& vShaderPath, const EShaderType vShaderType);
	void createShaderProgram();
	void useShaderProgram();
	unsigned int getShaderProgram() const;

	//Uniform Functions
	void setBool (const std::string& vUniformName, bool vValue) const;
	void setInt	 (const std::string& vUniformName, int vValue) const;
	void setFloat(const std::string& vUniformName, float vValue) const;

	void setVec2(const std::string& vUniformName, const glm::vec2& vValue) const;
	void setVec2(const std::string& vUniformName, float x, float y) const;
	void setVec3(const std::string& vUniformName, const glm::vec3& vValue) const;
	void setVec3(const std::string& vUniformName, float x, float y, float z) const;
	void setVec4(const std::string& vUniformName, const glm::vec4& vValue) const;
	void setVec4(const std::string& vUniformName, float x, float y, float z, float w) const;
	void setMat2(const std::string& vUniformName, const glm::mat2& vValue) const;
	void setMat3(const std::string& vUniformName, const glm::mat3& vValue) const;
	void setMat4(const std::string& vUniformName, const glm::mat4& vValue) const;

private:
	void __dumpLoadShaderFile(const std::string& vShaderPath, std::string& voShaderCode);
	void __createShader(const std::string& vShaderCode, const EShaderType vShaderType);

	unsigned int m_ShaderProgram;
	std::vector<unsigned int> m_ShaderVector;
};