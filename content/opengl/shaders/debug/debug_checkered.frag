// simple texture fragment debug shader

#version 400

// sets precision

//precision highp float;

uniform uint chsize;
uniform vec4 col1;
uniform vec4 col2;

in vec2 TexCoord;

out vec4 outputColor;

void main()
{
 	float fmodResult = mod(floor(chsize * TexCoord.s) + floor(chsize * TexCoord.t), 2.0);
	if (fmodResult < 1.0) outputColor = col1;
	else outputColor= col2;
}

