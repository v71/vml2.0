#version 400

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gDiffuse;
layout (location = 3) out vec4 gSpecular;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 diffuseCol;
uniform vec4 specularCol;

void main()
{    
    // store the fragment position vector in the first gbuffer texture
    gPosition = FragPos;
    // also store the per-fragment normals into the gbuffer
    gNormal = normalize(Normal);
    // the diffuse per-fragment color
    gDiffuse = diffuseCol;
    // and the specular per-fragment color
    gSpecular = specularCol;
}