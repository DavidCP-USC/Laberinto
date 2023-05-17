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
	//Función para crear el laberinto como tal
	void crearLaberinto(Shader shaderProgram, unsigned int VAOMuro);
private:
	//Salto de linea
	static const unsigned int salto = 11;
	//Array donde se guardan el layout del mapa
	std::vector<char> layout = {};
	//Archivo de donde se lee el mapa
	std::ifstream mapFile;
	//Función que comprueba si el layout está bien inicializado
	void checkLayout();
};