#version 330 core

uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;


in vec3 Normal;
in vec3 FragPos;  

void main() {
	// ambient
	float ambient_strength = 0.1;
	vec3 ambient = ambient_strength * lightColor;
	// diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
	// specular
	float specular_strength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specular_strength * spec * lightColor;

	gl_FragColor = vec4((ambient + diffuse + specular) * objectColor, 1.0f);
}