#version 330 core
in vec3 outColor;
out vec4 gl_FragColor;

void main() {
	gl_FragColor = vec4(outColor, 1.0);
}