#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>

using namespace Xinwuya;

CShader::~CShader()
{
	for (unsigned int i : m_ShaderVector)
		glDeleteShader(i);
}

//****************************************************************************************
//FUNCTION:
void CShader::addShader(const std::string & vShaderPath, const EShaderType vShaderType)
{
	std::string ShaderCode;
	__dumpLoadShaderFile(vShaderPath, ShaderCode);
	__createShader(ShaderCode, vShaderType);
}

//****************************************************************************************
//FUNCTION:
void CShader::createShaderProgram()
{
	m_ShaderProgram = glCreateProgram();
	for (unsigned int i : m_ShaderVector)
		glAttachShader(m_ShaderProgram, i);
	glLinkProgram(m_ShaderProgram);

	GLint LinkSuccess;
	glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &LinkSuccess);
	if (!LinkSuccess)
	{
		GLint LogLength;
		glGetProgramiv(m_ShaderProgram, GL_INFO_LOG_LENGTH, &LogLength);
		if (LogLength > 0)
		{
			auto* pInfoLog = new GLchar[LogLength];
			glGetProgramInfoLog(m_ShaderProgram, LogLength, &LogLength, pInfoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << pInfoLog << std::endl;
		}
	}
	
}

//****************************************************************************************
//FUNCTION:
void CShader::useShaderProgram()
{
	glUseProgram(m_ShaderProgram);
}

//****************************************************************************************
//FUNCTION:
unsigned int CShader::getShaderProgram() const
{
	return m_ShaderProgram;
}

//**********************************************************************************
//FUNCTION:
void CShader::setBool(const std::string & vUniformName, bool vValue) const
{
	GLuint Location = glGetUniformLocation(m_ShaderProgram, vUniformName.c_str());
	_ASSERT(Location >= 0);
	glUniform1i(Location, (int)vValue);
}

//**********************************************************************************
//FUNCTION:
void CShader::setInt(const std::string & vUniformName, int vValue) const
{
	GLuint Location = glGetUniformLocation(m_ShaderProgram, vUniformName.c_str());
	_ASSERT(Location >= 0); 
	glUniform1i(Location, vValue);
}

//**********************************************************************************
//FUNCTION:
void CShader::setFloat(const std::string & vUniformName, float vValue) const
{
	GLuint Location = glGetUniformLocation(m_ShaderProgram, vUniformName.c_str());
	_ASSERT(Location >= 0);
	glUniform1f(Location, vValue);
}

//**********************************************************************************
//FUNCTION:
void CShader::setVec2(const std::string & vUniformName, const glm::vec2 & vValue) const
{
	GLuint Location = glGetUniformLocation(m_ShaderProgram, vUniformName.c_str());
	_ASSERT(Location >= 0);
	glUniform2fv(Location, 1, &vValue[0]);
}
//**********************************************************************************
//FUNCTION:
void CShader::setVec2(const std::string & vUniformName, float x, float y) const
{
	GLuint Location = glGetUniformLocation(m_ShaderProgram, vUniformName.c_str());
	_ASSERT(Location >= 0);
	glUniform2f(Location, x, y);
}

//**********************************************************************************
//FUNCTION:
void CShader::setVec3(const std::string & vUniformName, const glm::vec3 & vValue) const
{
	GLuint Location = glGetUniformLocation(m_ShaderProgram, vUniformName.c_str());
	_ASSERT(Location >= 0);
	glUniform3fv(Location, 1, &vValue[0]);
}

//**********************************************************************************
//FUNCTION:
void CShader::setVec3(const std::string & vUniformName, float x, float y, float z) const
{
	GLuint Location = glGetUniformLocation(m_ShaderProgram, vUniformName.c_str());
	_ASSERT(Location >= 0);
	glUniform3f(Location, x, y, z);
}

//**********************************************************************************
//FUNCTION:
void CShader::setVec4(const std::string & vUniformName, const glm::vec4 & vValue) const
{
	GLuint Location = glGetUniformLocation(m_ShaderProgram, vUniformName.c_str());
	_ASSERT(Location >= 0);
	glUniform4fv(Location, 1, &vValue[0]);
}

//**********************************************************************************
//FUNCTION:
void CShader::setVec4(const std::string & vUniformName, float x, float y, float z, float w) const
{
	GLuint Location = glGetUniformLocation(m_ShaderProgram, vUniformName.c_str());
	_ASSERT(Location >= 0);
	glUniform4f(Location, x, y, z, w);
}

//**********************************************************************************
//FUNCTION:
void CShader::setMat2(const std::string & vUniformName, const glm::mat2 & vValue) const
{
	GLuint Location = glGetUniformLocation(m_ShaderProgram, vUniformName.c_str());
	_ASSERT(Location >= 0);
	glUniformMatrix2fv(Location, 1, GL_FALSE, &vValue[0][0]);
}

//**********************************************************************************
//FUNCTION:
void CShader::setMat3(const std::string & vUniformName, const glm::mat3 & vValue) const
{
	GLuint Location = glGetUniformLocation(m_ShaderProgram, vUniformName.c_str());
	_ASSERT(Location >= 0);
	glUniformMatrix3fv(Location, 1, GL_FALSE, &vValue[0][0]);
}

//**********************************************************************************
//FUNCTION:
void CShader::setMat4(const std::string & vUniformName, const glm::mat4 & vValue) const
{
	GLuint Location = glGetUniformLocation(m_ShaderProgram, vUniformName.c_str());
	_ASSERT(Location >= 0);
	glUniformMatrix4fv(Location, 1, GL_FALSE, &vValue[0][0]);
}

//****************************************************************************************
//FUNCTION:
void CShader::__dumpLoadShaderFile(const std::string & vShaderPath, std::string & voShaderCode)
{
	std::fstream FileStream;
	FileStream.exceptions(std::ifstream::badbit);
	try
	{
		FileStream.open(vShaderPath);
		std::stringstream StringStream;
		StringStream << FileStream.rdbuf();
		FileStream.close();
		voShaderCode = StringStream.str();
	}
	catch (std::ifstream::failure&)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}
}

//****************************************************************************************
//FUNCTION:
void CShader::__createShader(const std::string & vShaderCode, const EShaderType vShaderType)
{
	GLuint ShaderID = 0;
	switch (vShaderType)
	{
	case EShaderType::VERTEX_SHADER:				  ShaderID = glCreateShader(GL_VERTEX_SHADER); break;
	case EShaderType::FRAGMENT_SHADER:				  ShaderID = glCreateShader(GL_FRAGMENT_SHADER); break;
	case EShaderType::TESSELLATION_CONTROL_SHADER:	  ShaderID = glCreateShader(GL_TESS_CONTROL_SHADER); break;
	case EShaderType::TESSELLATION_EVALUATION_SHADER: ShaderID = glCreateShader(GL_TESS_EVALUATION_SHADER); break;
	case EShaderType::GEOMETRY_SHADER:				  ShaderID = glCreateShader(GL_GEOMETRY_SHADER); break;
	default: break;
	}
	m_ShaderVector.push_back(ShaderID);
	const GLchar* ShaderSource = vShaderCode.c_str();
	glShaderSource(ShaderID, 1, &ShaderSource, nullptr);
	glCompileShader(ShaderID);

	GLint CompileSuccess;
	glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &CompileSuccess);
	if (!CompileSuccess)
	{
		GLint LogLength;
		glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &LogLength);
		if (LogLength > 0)
		{
			auto* pInfoLog = new GLchar[LogLength];
			glGetShaderInfoLog(ShaderID, LogLength, &LogLength, pInfoLog);
			std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << pInfoLog << std::endl;
		}
	}
}
