#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace Shader
{

    namespace Types
    {
        /* 
            Important Notes
            -----------------
            - In an ideal world, we'd have a class below that maps to whatever
              our Mathematical Graphics API (GLM, etc) deems as a 'Vector2f' and 
              extend this functionality throughout our devices. Seeing as
              I have a very close coupling with my Mathematical Graphics API (GLM),
              I'll typedef these away for now, but in production, we ought to have some 
              macros that resolve these 'types' to their respective Graphics API
        */

        typedef glm::vec2 Vector2f;
        typedef glm::vec3 Vector3f;
        typedef glm::vec4 Vector4f;
        typedef glm::mat2 Matrix2f;
        typedef glm::mat3 Matrix3f;
        typedef glm::mat4 Matrix4f;
    }

    namespace SubComponent
    {
        enum ShaderCompilationUnits
        {
            VERTEX,
            FRAGMENT,
            PROGRAM
        };

        class Base
        {
            public:
                Base() = default;
                virtual ~Base() = default;

                virtual void operator=(Base& baseShader);
                virtual bool create(const char* vertexShader, const char* fragmentShader);
                virtual void use() const;
                virtual void setBool(const std::string &name, bool value) const;
                virtual void setInt(const std::string &name, int value) const;
                virtual void setFloat(const std::string &name, float value) const;
                virtual void setVec2(const std::string &name, const Shader::Types::Vector2f& value) const;
                virtual void setVec2(const std::string &name, float x, float y) const;
                virtual void setVec3(const std::string &name, const Shader::Types::Vector3f& value) const;
                virtual void setVec3(const std::string &name, float x, float y, float z) const;
                virtual void setVec4(const std::string &name, const Shader::Types::Vector4f& value) const;
                virtual void setVec4(const std::string &name, float x, float y, float z, float w) const;
                virtual void setMat2(const std::string &name, const Shader::Types::Matrix2f& mat) const;
                virtual void setMat3(const std::string &name, const Shader::Types::Matrix3f& mat) const;
                virtual void setMat4(const std::string &name, const Shader::Types::Matrix4f& mat) const;
            protected:
                virtual void checkCompileErrors(unsigned int& compHandle, ShaderCompilationUnits compType);
        };
    }
}