#version 330 core
layout (location = 0) in vec3 Pos;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 inTexCoord;

out vec3 Color;
out vec2 TexCoord;

uniform mat4 transform;

void main() {
	gl_Position = transform * vec4(Pos, 1.0);
	Color = inColor;
	TexCoord = inTexCoord;
}