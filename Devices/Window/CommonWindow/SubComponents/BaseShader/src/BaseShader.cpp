#include "BaseShader.h"

namespace
{
    const char* label = "BaseShader";
}

using namespace Shader::SubComponent;

////////////////////////////////////////////////////////////////////////////////
void Base::operator=(Base& baseShader)
{
    std::cout << "Override this functionality : " << __PRETTY_FUNCTION__ << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
bool Base::create(const char* vertexShader, const char* fragmentShader)
{
    std::cout << "Override this functionality : " << __PRETTY_FUNCTION__ << std::endl;

    return false;
}

////////////////////////////////////////////////////////////////////////////////
void Base::use() const
{
    std::cout << "Override this functionality : " << __PRETTY_FUNCTION__ << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
void Base::setBool(const std::string &name, bool value) const
{
    std::cout << "Override this functionality : " << __PRETTY_FUNCTION__ << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
void Base::setInt(const std::string &name, int value) const
{
    std::cout << "Override this functionality : " << __PRETTY_FUNCTION__ << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
void Base::setFloat(const std::string &name, float value) const
{
    std::cout << "Override this functionality : " << __PRETTY_FUNCTION__ << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
void Base::setVec2(const std::string &name, const Shader::Types::Vector2f& value) const
{
    std::cout << "Override this functionality : " << __PRETTY_FUNCTION__ << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
void Base::setVec2(const std::string &name, float x, float y) const
{
    std::cout << "Override this functionality : " << __PRETTY_FUNCTION__ << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
void Base::setVec3(const std::string &name, const Shader::Types::Vector3f& value) const
{
    std::cout << "Override this functionality : " << __PRETTY_FUNCTION__ << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
void Base::setVec3(const std::string &name, float x, float y, float z) const
{
    std::cout << "Override this functionality : " << __PRETTY_FUNCTION__ << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
void Base::setVec4(const std::string &name, const Shader::Types::Vector4f& value) const
{
    std::cout << "Override this functionality : " << __PRETTY_FUNCTION__ << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
void Base::setVec4(const std::string &name, float x, float y, float z, float w) const
{
    std::cout << "Override this functionality : " << __PRETTY_FUNCTION__ << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
void Base::setMat2(const std::string &name, const Shader::Types::Matrix2f& mat) const
{
    std::cout << "Override this functionality : " << __PRETTY_FUNCTION__ << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
void Base::setMat3(const std::string &name, const Shader::Types::Matrix3f& mat) const
{
    std::cout << "Override this functionality : " << __PRETTY_FUNCTION__ << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
void Base::setMat4(const std::string &name, const Shader::Types::Matrix4f& mat) const
{
    std::cout << "Override this functionality : " << __PRETTY_FUNCTION__ << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
void Base::checkCompileErrors(unsigned int& compHandle, ShaderCompilationUnits compType)
{   
    std::cout << "Override this functionality : " << __PRETTY_FUNCTION__ << std::endl;
}