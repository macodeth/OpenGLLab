#version 330 core

uniform vec3 viewPos;

struct Material {
	sampler2D diffuse1;
	sampler2D specular1;
	float shininess;
};

uniform Material material;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	float constant;
	float linear;
	float quadratic;
};

uniform Light light;  

in vec3 Normal;
in vec3 FragPos;  
in vec2 TexCoord;

void main() {
	// ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse1, TexCoord));
	// diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse1, TexCoord));
	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * vec3(texture(material.specular1, TexCoord)));
	
	float d = distance(light.position, FragPos);
	float attenuation = 1.0 / (light.constant + d * light.linear + d * d * light.quadratic);

	gl_FragColor = vec4((ambient + diffuse + specular) * attenuation, 1.0f);
}