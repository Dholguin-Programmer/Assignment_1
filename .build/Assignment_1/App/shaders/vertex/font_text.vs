#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 projection;

void main()
{
    // For now we'll just assign our Z value such that it doesn't conflict with 
    // any other objects (not very dynamic but sake of getting assignment done)
    gl_Position = projection * vec4(vertex.x, vertex.y, -10.0, 1.0);
    TexCoords = vertex.zw;
}

