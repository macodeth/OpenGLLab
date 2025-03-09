#version 330 core

uniform vec3 viewPos;

uniform sampler2D image;

in vec2 TexCoord;

void main() {
	gl_FragColor = texture(image, TexCoord);
}