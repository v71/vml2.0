// simple per vertex color debug shader

#version 400

// layout locations

layout (location = 0) in vec4 VertexPosition;
layout (location = 1) in vec3 VertexNormal;		    // not used in this shader
layout (location = 2) in vec4 VertexColorCoord;
layout (location = 3) in vec2 VertexTexCoord;		// not used in this shader

// sets precision

//precision highp float;

// uniforms

uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewProjectionMatrix;

// set smoothing for out color 
smooth out vec4 fragColor;

uniform mat4 lightSpaceMatrix;

// vertex shader

void main()
{
    gl_Position = lightSpaceMatrix * ModelViewMatrix * VertexPosition;
}