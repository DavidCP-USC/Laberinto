#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <learnopengl/shader_m.h>

class Mapa
{
public:
	//Constructor del mapa
	Mapa();
	//Funci�n para crear el laberinto como tal
	void crearLaberinto(Shader shaderProgram, unsigned int VAOMuro);
private:
	//Salto de linea
	static const unsigned int salto = 11;
	//Array donde se guardan el layout del mapa
	std::vector<char> layout = {};
	//Archivo de donde se lee el mapa
	std::ifstream mapFile;
	//Funci�n que comprueba si el layout est� bien inicializado
	void checkLayout();
};