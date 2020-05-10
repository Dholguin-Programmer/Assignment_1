#pragma once

#include "BaseShader.h"

namespace Shader
{
    namespace SubComponent
    {
        class Diegofied : public Shader::SubComponent::Base
        {
            public:
                Diegofied() = default;
                Diegofied(const char* vertexShaderPath, const char* fragmentShaderPath);
                ~Diegofied() = default;
                
                void operator=(Diegofied diegofiedShader);
                bool create(const char* vertexShaderPath, const char* fragmentShaderPath) override;
                void use() const override;
                void setBool(const std::string &name, bool value) const override;
                void setInt(const std::string &name, int value) const override;
                void setFloat(const std::string &name, float value) const override;
                void setVec2(const std::string &name, const Shader::Types::Vector2f& value) const override;
                void setVec2(const std::string &name, float x, float y) const override;
                void setVec3(const std::string &name, const Shader::Types::Vector3f& value) const override;
                void setVec3(const std::string &name, float x, float y, float z) const override;
                void setVec4(const std::string &name, const Shader::Types::Vector4f& value) const override;
                void setVec4(const std::string &name, float x, float y, float z, float w) const override;
                void setMat2(const std::string &name, const Shader::Types::Matrix2f& mat) const override;
                void setMat3(const std::string &name, const Shader::Types::Matrix3f& mat) const override;
                void setMat4(const std::string &name, const Shader::Types::Matrix4f& mat) const override;
                unsigned int m_programHandle;
            protected:
                void checkCompileErrors(unsigned int& compHandle, ShaderCompilationUnits compType) override;

            private:
        };
    }
}