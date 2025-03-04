#version 330 core
in vec2 TexCoord;

uniform float mixParam;
uniform sampler2D texture0;
uniform sampler2D texture1;

void main() {
	gl_FragColor = mix(texture(texture0, TexCoord), texture(texture1, TexCoord), mixParam);
}