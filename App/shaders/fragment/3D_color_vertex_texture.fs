#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 ourColor;

// Conditional flags, we always assume these
// are disabled, and must be enabled, by the renderer
uniform bool isTextureUsed = false;
uniform bool isColorUsed = false;

uniform sampler2D texture1;

void main()
{
    if (isTextureUsed && isColorUsed)
    {
        FragColor = texture(texture1, TexCoord) * vec4(ourColor, 1.0);
    }
    else if (isTextureUsed && !isColorUsed)
    {
        FragColor = texture(texture1, TexCoord);
    }
    else if (isColorUsed)
    {
        FragColor = vec4(ourColor, 1.0);
    }
    else
    {
        // Do nothing, there will always exist atleast a color   
    }
}