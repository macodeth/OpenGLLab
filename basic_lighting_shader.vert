#version 330 core
layout (location = 0) in vec3 Pos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 textCoord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;

void main() {
	gl_Position = projection * view * model * vec4(Pos, 1.0f);
	Normal = mat3(transpose(inverse(model))) * inNormal;
    FragPos = vec3(model * vec4(Pos, 1.0));
	TexCoord = textCoord;
}