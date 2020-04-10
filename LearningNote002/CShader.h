#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

enum EShaderType
{
	VertexShader,
	FragmentShader,
	TessellationControlShader,
	TessellationEvaluationShader,
	GeometryShader
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

private:
	void __dumpLoadShaderFile(const std::string& vShaderPath, std::string& voShaderCode);
	void __createShader(const std::string& vShaderCode, const EShaderType vShaderType);

	unsigned int m_ShaderProgram;
	std::vector<unsigned int> m_ShaderVector;
};