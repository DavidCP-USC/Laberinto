/*
* Laberinto
* Hecho por David Corral Pazos y Adrian Ovidiu Cocean
*/

/*INCLUDES NECESARIOS PARA USAR OPENGL 3.3*/
#include <glad.h>
#include <glfw3.h>
#include <stdio.h>
#include <math.h> 
#include <learnopengl/shader_m.h>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Mapa.h"

//Este include es del cubo que usaremos para hacer las paredes del laberinto
//#include "Cubo.h"

//Función para redimensionar la pantalla
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//Función para procesar teclado
void processInput(GLFWwindow* window, Shader shaderProgram);

//Función para hacer muro
extern int crearCubo();

//Parámetros de cámara
float DISTANCIA = 100;
float incrementoVista = 0.00025;
float incrementoMovimiento = 0.05;
float incrementoVistaHorizontal = 0.05;
float alpha = 0;
float beta = 0;
float movimientoX = -2;
float movimientoY = 0;
int camara = 1;
//Tamaños de ventana por defecto
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 800;

//VAO DEL CUBO DE MURO
unsigned int VAOMuro;

void camaraLibre(Shader shaderProgram) {
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glm::mat4 view;
	view = glm::mat4();
	view = glm::lookAt(glm::vec3(movimientoX, movimientoY, 0),
		glm::vec3(movimientoX + 2 ,
			movimientoY + beta,
			alpha * DISTANCIA),
		glm::vec3(0.0f, 0.0f, 1.0f));
	shaderProgram.setMat4("view", view);

	glm::mat4 projection;
	projection = glm::mat4();
	projection = glm::perspective(glm::radians(100.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 2000.0f);
	shaderProgram.setMat4("projection", projection);

}

void myCamara(Shader shaderProgram) {
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	glm::mat4 view;
	view = glm::mat4();
	view = glm::lookAt(glm::vec3(0.0f, -2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shaderProgram.setMat4("view", view);

	glm::mat4 projection;
	projection = glm::mat4();
	projection = glm::perspective(glm::radians(100.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 2000.0f);
	shaderProgram.setMat4("projection", projection);
}

void seleccionCamara(Shader shaderProgram) {
	switch (camara) {
	case 1:
		camaraLibre(shaderProgram);
		break;
	case 2:
		myCamara(shaderProgram);
		break;

	}
}

void openGlInit() {
	//Habilito aqui el depth en vez de arriba para los usuarios de linux y mac mejor arriba
	//Incializaciones varias
	glClearDepth(1.0f); //Valor z-buffer
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST); // z-buffer
	glEnable(GL_CULL_FACE); //ocultacion caras back
	glCullFace(GL_BACK);
}

void Display(GLFWwindow* window, Shader shaderProgram, Mapa* mapa)
{
	//Color de background de pantalla por defecto
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	seleccionCamara(shaderProgram);
	//Usamos la cámara todo el rato
	//camaraLibre(shaderProgram);

	mapa->crearLaberinto(shaderProgram, VAOMuro);
}

int main()
{
	//Inicializar glfw
	glfwInit();
	//Versión de OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Usamos el core profile a partir de OpenGl 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Creamos la ventana
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Laberinto", NULL, NULL);
	//Check de errores
	if (window == NULL)
	{
		std::cout << "Error al crear ventana de GLFW" << std::endl;
		glfwTerminate();
		return -1;
	}

	//Ahora mostramos la ventana
	glfwMakeContextCurrent(window);
	//Hacemos set de la función para redimensionar la ventana
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Cargamos glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Error al iniciar GLAD" << std::endl;
		return -1;
	}

	openGlInit();

	//Usamos nuestro shader
	Shader shaderProgram("shader.vert", "shader.frag");

	//Creamos un cubo
	VAOMuro = crearCubo();

	//Usamos shader
	shaderProgram.use();

	//Aquí estoy haciendo el laberinto
	Mapa* mapa = new Mapa();

	//Loop de renderizado
	while (!glfwWindowShouldClose(window))
	{
		//Activar shader
		shaderProgram.use();
		//Hacemos setup del input
		processInput(window, shaderProgram);
		//La función de display
		Display(window, shaderProgram, mapa);
		//Hacer swapping de buffers
		glfwSwapBuffers(window);
		//Activar eventos de E/S como ratón, teclado, etc.
		glfwPollEvents();
	}
	//Liberar el VAO
	glDeleteVertexArrays(1, &VAOMuro);
	//Acabar el programa
	glfwTerminate();
	return 0;
}

//Función de procesamiento del teclado
void processInput(GLFWwindow* window, Shader shader)
{
	//Si pulsamos ESC, salimos del programa
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		beta += incrementoVistaHorizontal;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		beta -= incrementoVistaHorizontal;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		alpha += incrementoVista;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		alpha -= incrementoVista;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movimientoX -= incrementoMovimiento;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movimientoX += incrementoMovimiento;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movimientoY -= incrementoMovimiento;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		movimientoY += incrementoMovimiento;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//Redimensionar el viewport
	glViewport(0, 0, width, height);
}