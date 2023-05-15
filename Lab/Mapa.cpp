#include "Mapa.h"
#include <glfw3.h>
#include <glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Mapa::Mapa() : mapFile("map.txt")
{
	std::string str;
	if (mapFile.is_open())
	{
		while (char c = mapFile.get())
		{
			if (c == '\n')
			{
				continue;
			}
			if (c == EOF)
				break;
			layout.push_back(c);
		}
	}
}

void Mapa::checkLayout()
{
	for (int i = 0; i < layout.size(); i++)
	{
		std::cout << "Layout[" << i << "]: " << layout[i] << std::endl;
	}
}

void Mapa::crearLaberinto(Shader shaderProgram, unsigned int VAOMuro)
{
	//Necesito las coordenadas de cada uno de los cubos que van a formar el muro, se van a ir colocando en un orden preestablecido
	glm::vec3 coordenadas = { -100.0f, 0.0f, 100.0f };
	//Estos factores me sirven para escalar los cubos sobre el eje x y sobre el eje z para colocarlos uno al lado del otro
	glm::vec3 escala = { 5.0f, 7.0f, 5.0f };
	//Hay que pasar por el layout
	glBindVertexArray(VAOMuro);
	for (int i = 0; i < layout.size(); i++)
	{
		if(layout[i] == '1')
		{
			//Matriz de transformación
			glm::mat4 transform = glm::mat4();
			/*Primero escalamos los cubos para que parezcan unos muros, después los colocamos, el orden es scale-translate*/
			transform = glm::translate(transform, coordenadas);
			transform = glm::scale(transform, escala);
			//Ponemos la matriz en el shader
			shaderProgram.setMat4("transform", transform);
			//Renderizar el cubo
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		//Ahora que nos hemos movido, hay que cambiar las coordenadas
		coordenadas.x += escala.x;
		//Cada 10 repeticiones hay que baja en el eje z por el salto
		if(i != 0 && i % salto == 0)
			coordenadas.z -= escala.z;
	}
}

