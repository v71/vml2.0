#version 400

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec4 aInstanceParam;  // (RGB) light color and (A) is light radius
layout (location = 3) in mat4 aInstanceMatrix;

out vec3 lightColor;
out vec3 lightPosition;
out float lightRadius;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	lightColor = aInstanceParam.rgb;
	lightRadius = aInstanceParam.w;
	// extract light position from the instance model matrix
	lightPosition = vec3(aInstanceMatrix[3]);
    gl_Position = projection * view * aInstanceMatrix * vec4(lightRadius * aPos, 1.0);
}

