#pragma once

// Standard C++ includes
#include <vector>
#include <map>

// 3rd Party includes
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

// SubComponent includes
#include "BaseRenderer.h"
#include "DiegofiedShader.h"

namespace Renderer
{
    namespace SubComponent
    {
        namespace Types
        {
            struct OpenGLBuffers
            {
                // This struct defines our OpenGL buffers, handles, vertice, and index data.
                // Also contains other 3D details
                OpenGLBuffers() : m_vertexArray(0),
                                  m_vertexBuffer(0),
                                  m_elementBuffer(0),
                                  m_renderType(Renderer::Types::RenderingType::STATIC),
                                  m_numUniqueIndices(0),
                                  m_numVertices(0),
                                  m_numDimensions(0),
                                  m_vertices(std::vector<float>()),
                                  m_indices(std::vector<float>())
                {
                }

                // Handles
                unsigned int m_vertexArray;   // Vertex Array Object (VAO)
                unsigned int m_vertexBuffer;  // Vertex Buffer Object (VBO)
                unsigned int m_elementBuffer; // Element Buffer Ojbect (EBO)
                
                // Misc Info
                Renderer::Types::RenderingType m_renderType;
                unsigned int m_numUniqueIndices;
                unsigned int m_numVertices;
                unsigned int m_numDimensions;
                unsigned int m_numAttributeBuffers;

                // Containers
                std::vector<float> m_vertices;
                std::vector<float> m_indices;
            };

            // This struct defines our 3D World Objects
            struct RenderObject
            {
                RenderObject() : m_mlbInfo("NULL", "NULL", "NULL", "NULL"),
                                 m_type(Renderer::Types::GeometricObjects::SQUARE),
                                 m_scale(),
                                 m_position(),
                                 m_colorType(Renderer::Types::Colors::RED),
                                 m_openGlBuffer(),
                                 m_texture(0),
                                 m_textureUsed(false),
                                 m_fragmentShader(""),
                                 m_vertexShader("")
                {
                }

                Renderer::Types::MlbInfo m_mlbInfo;        // MLB Information
                Renderer::Types::GeometricObjects m_type;  // Geometric Object Type
                Renderer::Types::Vector3f m_scale;         // 3D Scale
                Renderer::Types::Vector3f m_position;      // 3D Position
                Renderer::Types::Vector3f m_colorType;     // 3D Color
                Types::OpenGLBuffers m_openGlBuffer;       // OpenGL Buffers
                unsigned int m_texture;                    // Texture Handle

                // Flags (These could be abstracted)               
                bool m_textureUsed;

                std::string m_fragmentShader;              // Fragment Shader Path
                std::string m_vertexShader;                // Vertex Shader Path
            };

            // This struct defines our 2D Text Object
            struct TextObject
            {
                TextObject() : m_text("NULL"),
                               m_scale(),
                               m_position(),
                               m_colorType(Renderer::Types::Colors::RED),
                               m_openGlBuffer(),
                               m_fragmentShader("NULL"),
                               m_vertexShader("NULL")
                {
                }
                
                std::string m_text;                    // Text to be draw
                Renderer::Types::Vector3f m_scale;     // 3D Scale
                Renderer::Types::Vector3f m_position;  // 3D Position
                Renderer::Types::Vector3f m_colorType; // 3D Color

                Types::OpenGLBuffers m_openGlBuffer;   // OpenGL Buffers

                std::string m_fragmentShader;          // Fragment Shader Path
                std::string m_vertexShader;            // Vertex Shader Path
            };

            // This struct defines the components needed to draw text onto our screen
            struct Character 
            {
                unsigned int TextureID;  // ID handle of the glyph texture
                glm::ivec2   Size;       // Size of glyph
                glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
                unsigned int Advance;    // Offset to advance to next glyph
            };
        }

        class Diegofied : Renderer::SubComponent::Base
        {
            public:
                // This is the constructor for our specific Renderer implementation
                Diegofied(Window::Device::Manager& manager);

                // This is the destructor for our specific Renderer implementation
                ~Diegofied();
                
                /* 
                    From Renderer::SubComponent::Base
                */

                // This function is a blocking call that draws a variety of objects based upon either
                // statically created or dynamic objects (See RenderingType)
                void drawToScreen(bool exitCondition) override;

                // This function attempts to load a texture given a path and a handle
                bool loadTexture(const char* texturePath, unsigned int& textureHandle) override;
                
                // This function initalizes this sub component
                bool initSubComponent() override;
                
                // This function handles creation of view space objects only
                bool createViewSpaceObject(const char* text,
                                           Renderer::Types::Vector3f color,
                                           Renderer::Types::Vector3f position,
                                           Renderer::Types::Vector3f scale,
                                           const char* vertexShaderPath,
                                           const char* fragmentShaderPath) override;

                // This function handles creation of world space (3D) objects
                bool createGeometricObject(Renderer::Types::MlbInfo mlbInfo,
                                           Renderer::Types::GeometricObjects type,
                                           Renderer::Types::RenderingType renderType,
                                           Renderer::Types::Vector3f color,
                                           Renderer::Types::Textures textureType,
                                           const char* texturePath,
                                           Renderer::Types::Vector3f position,
                                           Renderer::Types::Vector3f scale,
                                           const char* vertexShaderPath,
                                           const char* fragmentShaderPath) override;

                // Vector of all the static 3D rendering objects we will be rendering
                std::vector<Types::RenderObject> m_staticRenderObjects;

                // Vector of all the dynamic 3D rendering objects we will be renderering
                std::vector<Types::RenderObject> m_dynamicRenderObjects;

                // Vector of text objects (2D View space only) that we will be rendering
                std::vector<Types::TextObject> m_textToRender;

            protected:
                // This function determines whether this specific implementation supports creation
                // of 2D/3D objects
                bool isGeometricObjectSupported(Renderer::Types::GeometricObjects type) override;
                
                // This function determines whether this specific implementation supports handling
                // of the specific shader     
                bool isShaderSupported(const char* path) override;

                // This helper is used to determine with any object that is renderable to our screen, whether
                // it has a specified color being used or not
                bool isTextureSupported(const char* path) override;

            private:
                // This map handles converting raw ASCII character details into TTF information
                std::map<char, Types::Character> m_characterMapping;

                // This map converts a string to one of the supported vertex shader programs our renderer supports
                std::map<std::string, Shader::SubComponent::Diegofied*> m_vertexShaderLookupTable;

                // This map converts a string to one of the supported fragment shader programs our renderer supports
                std::map<std::string, Shader::SubComponent::Diegofied*> m_fragmentShaderLookupTable;

                // This helper function handles using our character mapping to convert a Text object that
                // contains a string (ASCII only) and a number of other properties, and drawing it to our screen
                void RenderText(Shader::SubComponent::Diegofied& shader, Types::TextObject obj);

                // This helper function populates a object to be rendered with it's corressponding 
                // OpenGL buffers based upon the type of 3D object we wish to create
                bool populatePrimitiveTypesVerticeData(Types::RenderObject& obj);

                // This helper function populates the 3D object OpenGL related buffers
                void populateOpenGlBuffers(Types::RenderObject& obj);
    
                // This helper function converts the Base shader into a Shader that our specific implementation
                // supports (i.e DiegofiedShader)
                Shader::SubComponent::Diegofied* convertBaseShader(Shader::SubComponent::Base* shader);
        };
    }
}