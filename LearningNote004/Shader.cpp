#include "Shader.h"

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

	GLint LinkSuccess;
	glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &LinkSuccess);
	if (!LinkSuccess)
	{
		GLint LogLength;
		glGetProgramiv(m_ShaderProgram, GL_INFO_LOG_LENGTH, &LogLength);
		if (LogLength > 0)
		{
			GLchar* pInfoLog = new GLchar[LogLength];
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
	glUniform1i(glGetUniformLocation(m_ShaderProgram, vUniformName.c_str()), (int)vValue);
}

//**********************************************************************************
//FUNCTION:
void CShader::setInt(const std::string & vUniformName, int vValue) const
{
	glUniform1i(glGetUniformLocation(m_ShaderProgram, vUniformName.c_str()), vValue);
}

//**********************************************************************************
//FUNCTION:
void CShader::setFloat(const std::string & vUniformName, float vValue) const
{
	glUniform1f(glGetUniformLocation(m_ShaderProgram, vUniformName.c_str()), vValue);
}

//**********************************************************************************
//FUNCTION:
void CShader::setVec2(const std::string & vUniformName, const glm::vec2 & vValue) const
{
	glUniform2fv(glGetUniformLocation(m_ShaderProgram, vUniformName.c_str()), 1, &vValue[0]);
}
//**********************************************************************************
//FUNCTION:
void CShader::setVec2(const std::string & vUniformName, float x, float y) const
{
	glUniform2f(glGetUniformLocation(m_ShaderProgram, vUniformName.c_str()), x, y);
}

//**********************************************************************************
//FUNCTION:
void CShader::setVec3(const std::string & vUniformName, const glm::vec3 & vValue) const
{
	glUniform3fv(glGetUniformLocation(m_ShaderProgram, vUniformName.c_str()), 1, &vValue[0]);
}

//**********************************************************************************
//FUNCTION:
void CShader::setVec3(const std::string & vUniformName, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(m_ShaderProgram, vUniformName.c_str()), x, y, z);
}

//**********************************************************************************
//FUNCTION:
void CShader::setVec4(const std::string & vUniformName, const glm::vec4 & vValue) const
{
	glUniform4fv(glGetUniformLocation(m_ShaderProgram, vUniformName.c_str()), 1, &vValue[0]);
}

//**********************************************************************************
//FUNCTION:
void CShader::setVec4(const std::string & vUniformName, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(m_ShaderProgram, vUniformName.c_str()), x, y, z, w);
}

//**********************************************************************************
//FUNCTION:
void CShader::setMat2(const std::string & vUniformName, const glm::mat2 & vValue) const
{
	glUniformMatrix2fv(glGetUniformLocation(m_ShaderProgram, vUniformName.c_str()), 1, GL_FALSE, &vValue[0][0]);
}

//**********************************************************************************
//FUNCTION:
void CShader::setMat3(const std::string & vUniformName, const glm::mat3 & vValue) const
{
	glUniformMatrix3fv(glGetUniformLocation(m_ShaderProgram, vUniformName.c_str()), 1, GL_FALSE, &vValue[0][0]);
}

//**********************************************************************************
//FUNCTION:
void CShader::setMat4(const std::string & vUniformName, const glm::mat4 & vValue) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgram, vUniformName.c_str()), 1, GL_FALSE, &vValue[0][0]);
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
	GLuint ShaderID = 0;
	switch (vShaderType)
	{
	case VERTEX_SHADER:					 ShaderID = glCreateShader(GL_VERTEX_SHADER); break;
	case FRAGMENT_SHADER:				 ShaderID = glCreateShader(GL_FRAGMENT_SHADER); break;
	case TESSELLATION_CONTROL_SHADER:	 ShaderID = glCreateShader(GL_TESS_CONTROL_SHADER); break;
	case TESSELLATION_EVALUATION_SHADER: ShaderID = glCreateShader(GL_TESS_EVALUATION_SHADER); break;
	case GEOMETRY_SHADER:				 ShaderID = glCreateShader(GL_GEOMETRY_SHADER); break;
	default: break;
	}
	m_ShaderVector.push_back(ShaderID);
	const GLchar* ShaderSource = vShaderCode.c_str();
	glShaderSource(ShaderID, 1, &ShaderSource, NULL);
	glCompileShader(ShaderID);

	GLint CompileSuccess;
	glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &CompileSuccess);
	if (!CompileSuccess)
	{
		GLint LogLength;
		glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &LogLength);
		if (LogLength > 0)
		{
			GLchar* pInfoLog = new GLchar[LogLength];
			glGetShaderInfoLog(ShaderID, LogLength, &LogLength, pInfoLog);
			std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << pInfoLog << std::endl;
		}
	}
}
