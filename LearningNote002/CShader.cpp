#include <glad/glad.h>
#include "CShader.h"

CShader::~CShader()
{
	for (int i = 0; i < m_ShaderVector.size(); ++i)
		glDeleteShader(m_ShaderVector[i]);
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
	for (int i = 0; i < m_ShaderVector.size(); ++i)
		glAttachShader(m_ShaderProgram, m_ShaderVector[i]);
	glLinkProgram(m_ShaderProgram);

	int Success;
	char InfoLog[512];
	glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &Success);
	if (!Success)
	{
		glGetProgramInfoLog(m_ShaderProgram, 512, NULL, InfoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << InfoLog << std::endl;
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
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
}

//****************************************************************************************
//FUNCTION:
void CShader::__createShader(const std::string & vShaderCode, const EShaderType vShaderType)
{
	GLuint ShaderID;
	switch (vShaderType)
	{
	case VertexShader				 : ShaderID = glCreateShader(GL_VERTEX_SHADER		  ); break;
	case FragmentShader				 : ShaderID = glCreateShader(GL_FRAGMENT_SHADER		  ); break;
	case TessellationControlShader	 : ShaderID = glCreateShader(GL_TESS_CONTROL_SHADER	  ); break;
	case TessellationEvaluationShader: ShaderID = glCreateShader(GL_TESS_EVALUATION_SHADER); break;
	case GeometryShader				 : ShaderID = glCreateShader(GL_GEOMETRY_SHADER		  ); break;
	default: break;
	}
	m_ShaderVector.push_back(ShaderID);
	const GLchar* ShaderSource = vShaderCode.c_str();
	glShaderSource(ShaderID, 1, &ShaderSource, NULL);
	glCompileShader(ShaderID);

	int Success;
	char InfoLog[512];
	glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &Success);
	if (!Success)
	{
		glGetShaderInfoLog(ShaderID, 512, NULL, InfoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << InfoLog << std::endl;
	}
}
