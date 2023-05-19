#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;  


uniform vec3 ourColor;
uniform vec3 viewPos; 
uniform vec3 lightPos; 
uniform vec3 lightColor;

void main()
{
    // Ambiente
    float ambientI = 0.25;
    vec3 ambient = ambientI * lightColor;
  	
    // Difusa
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

	// Especular
	float specularStrength = 1.0;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
	vec3 specular = specularStrength * spec * lightColor;
            
   	vec3 result = (ambient + diffuse + specular) * ourColor; // Luz final
    FragColor = vec4(result, 1.0);
} 
