#include "DiegofiedShader.h"

namespace
{
    const char* label = "DiegofiedShader";
}

using namespace Shader::SubComponent;

Diegofied::Diegofied(const char* vertexPath, const char* fragmentPath)
{
    create(vertexPath, fragmentPath);
}

bool Diegofied::create(const char* vertexShaderPath, const char* fragmentShaderPath)
{
    bool createdProgramSuccessfully = true;

   // Retrieve the vertex & fragment source code from respective file paths
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // Catch excpetions
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        
    try 
    {
        // Open files
        vShaderFile.open(vertexShaderPath);
        fShaderFile.open(fragmentShaderPath);
            
        // Read files
        std::stringstream vShaderStream, fShaderStream;            
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();		
            
        // Close files
        vShaderFile.close();
        fShaderFile.close();
           
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();			
    }
    catch (std::ifstream::failure& e)
    {
        createdProgramSuccessfully = false;
        std::cout << label << "::" << __FUNCTION__ << "Failed to read either fragment or vertex shader files" << std::endl;
    }
    
    // Treat as C strings
    const char* vShaderCode = vertexCode.c_str();
    const char * fShaderCode = fragmentCode.c_str();
    
    ////////////////////////////////////////////////////
    // Fragment Shader Source Phase
    ////////////////////////////////////////////////////
    unsigned int vertex;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, ShaderCompilationUnits::VERTEX);

    ////////////////////////////////////////////////////
    // Fragment Shader Source Phase
    ////////////////////////////////////////////////////
    unsigned int fragment;
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, ShaderCompilationUnits::FRAGMENT);

    ////////////////////////////////////////////////////
    // Shader Linking Phase
    ////////////////////////////////////////////////////   
    m_programHandle = glCreateProgram();
    glAttachShader(m_programHandle, vertex);
    glAttachShader(m_programHandle, fragment);
    glLinkProgram(m_programHandle);
    checkCompileErrors(m_programHandle, ShaderCompilationUnits::PROGRAM);
    
    // NO need to have handles for the vertex and fragment shaders open,
    // as we have compiled them into their own program now.
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return createdProgramSuccessfully;
}

////////////////////////////////////////////////////////////////////////////////
void Diegofied::operator=(Diegofied diegofiedShader)
{
    diegofiedShader.m_programHandle = m_programHandle;
}

////////////////////////////////////////////////////////////////////////////////
void Diegofied::use() const
{
    glUseProgram(m_programHandle);
}

////////////////////////////////////////////////////////////////////////////////
void Diegofied::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(m_programHandle, name.c_str()), (int)value);
}

////////////////////////////////////////////////////////////////////////////////
void Diegofied::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(m_programHandle, name.c_str()), value); 
}

////////////////////////////////////////////////////////////////////////////////
void Diegofied::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(m_programHandle, name.c_str()), value); 
}

////////////////////////////////////////////////////////////////////////////////
void Diegofied::setVec2(const std::string &name, const Shader::Types::Vector2f& value) const
{
    glUniform2fv(glGetUniformLocation(m_programHandle, name.c_str()), 1, &value[0]);
}

////////////////////////////////////////////////////////////////////////////////
void Diegofied::setVec2(const std::string &name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(m_programHandle, name.c_str()), x, y); 
}

////////////////////////////////////////////////////////////////////////////////
void Diegofied::setVec3(const std::string &name, const Shader::Types::Vector3f& value) const
{
    glUniform3fv(glGetUniformLocation(m_programHandle, name.c_str()), 1, &value[0]); 
}

////////////////////////////////////////////////////////////////////////////////
void Diegofied::setVec3(const std::string &name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(m_programHandle, name.c_str()), x, y, z); 
}

////////////////////////////////////////////////////////////////////////////////
void Diegofied::setVec4(const std::string &name, const Shader::Types::Vector4f& value) const
{
    glUniform4fv(glGetUniformLocation(m_programHandle, name.c_str()), 1, &value[0]); 
}

////////////////////////////////////////////////////////////////////////////////
void Diegofied::setVec4(const std::string &name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(m_programHandle, name.c_str()), x, y, z, w); 
}

////////////////////////////////////////////////////////////////////////////////
void Diegofied::setMat2(const std::string &name, const Shader::Types::Matrix2f& mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(m_programHandle, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

////////////////////////////////////////////////////////////////////////////////
void Diegofied::setMat3(const std::string &name, const Shader::Types::Matrix3f& mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(m_programHandle, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

////////////////////////////////////////////////////////////////////////////////
void Diegofied::setMat4(const std::string &name, const Shader::Types::Matrix4f& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_programHandle, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

////////////////////////////////////////////////////////////////////////////////
void Diegofied::checkCompileErrors(unsigned int& compHandle, ShaderCompilationUnits compType)
{   
    int  compilationResult;
    char compilationErrorBuffer[512];

    // Convert enum to string
    std::string compTypeString = "";
    switch(compType)
    {
        case ShaderCompilationUnits::VERTEX:
        {
            compTypeString = "Vertex";
            break;
        }
        case ShaderCompilationUnits::FRAGMENT:
        {
            compTypeString = "Fragment";
            break;
        }
        case ShaderCompilationUnits::PROGRAM:
        {
            compTypeString = "Program";
            break;
        }
        default:
        {
            std::cerr << "Unknown Shader Compilation Unit of " << static_cast<int>(compType) << std::endl;
        }
    }

    if (compTypeString != "")
    {
        glGetShaderiv(compHandle, GL_COMPILE_STATUS, &compilationResult);
        if (!compilationResult)
        {
            glGetProgramInfoLog(compHandle, 1024, NULL, compilationErrorBuffer);
            std::cout << label << "::" << __FUNCTION__ << ": Compilation failed for " << compTypeString << std::endl;
            std::cout << std::endl;
            std::cout << compilationErrorBuffer;
            std::cout << "---------------------------------------------------" << std::endl;
        }
    }
}