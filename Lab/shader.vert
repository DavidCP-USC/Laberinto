#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex;

out vec3 Normal;
out vec3 FragPos; 
out vec2 TexCoord;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

void main()
{
   gl_Position = projection * view * transform * vec4(aPos, 1.0f); // Esta variable es la posicion del fragmento en coordenadas del planeta
   Normal = mat3(transpose(inverse(transform))) * aColor; // Esta variable es la normal del fragmento en coordenadas del planeta
   FragPos = vec3(transform * vec4(aPos, 1.0)); // Esta variable es la posicion del fragmento en coordenadas del planeta
   TexCoord = vec2(aTex.x,aTex.y); // Esta variable es la coordenada de textura del fragmento
}