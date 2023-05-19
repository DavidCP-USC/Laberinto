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
#define PI 3.14159265

//Función para hacer muro
extern int crearCubo();
//Función para hacer el suelo
extern int crearSuelo();

//Parámetros de cámara
glm::vec3 cameraPosition = glm::vec3(-95.0f, 0.0f, 95.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float yaw = -90.0f;
float pitch = 0.0f;

//Variables para controlar el estado de las teclas
bool isKeyPressedW = false;
bool isKeyPressedA = false;
bool isKeyPressedS = false;
bool isKeyPressedD = false;

//Tamaños de ventana por defecto
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 800;

//VAO DEL CUBO DE MURO
unsigned int VAOMuro;

//VAO DEL DEL SUELO
unsigned int VAOSuelo;

//Función de procesamiento del teclado
void handleInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	float cameraSpeed = 0.25f;

	// Detectar cuando se presiona o se libera una tecla
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_W:
			isKeyPressedW = true;
			break;
		case GLFW_KEY_A:
			isKeyPressedA = true;
			break;
		case GLFW_KEY_S:
			isKeyPressedS = true;
			break;
		case GLFW_KEY_D:
			isKeyPressedD = true;
			break;
		}
	}
	else if (action == GLFW_RELEASE)
	{
		switch (key)
		{
		case GLFW_KEY_W:
			isKeyPressedW = false;
			break;
		case GLFW_KEY_A:
			isKeyPressedA = false;
			break;
		case GLFW_KEY_S:
			isKeyPressedS = false;
			break;
		case GLFW_KEY_D:
			isKeyPressedD = false;
			break;
		}
	}

	// Mover la cámara continuamente en función del estado de las teclas
	if (isKeyPressedW)
		cameraPosition += cameraSpeed * cameraFront;
	if (isKeyPressedS)
		cameraPosition -= cameraSpeed * cameraFront;
	if (isKeyPressedA)
		cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (isKeyPressedD)
		cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	//Si pulsamos ESC, salimos del programa
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void handleMouse(GLFWwindow* window, double xpos, double ypos)
{
	//Registramos la primera posición del cursor, junto con el centro de la pantalla
	static bool firstMouse = true;
	static double lastX = SCR_WIDTH / 2.0;
	static double lastY = SCR_HEIGHT / 2.0;

	//Ahora hacemos que la última posición del ratón en x e y se actualice la primera vez para poder ir empezando
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	//El offset se calcula como la última posición del ratón menos la actual o viceversa dependiendo del movimiento que queremos hacer
	float sensitivity = 0.1f;
	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos;

	//Se actualizan las variables
	lastX = xpos;
	lastY = ypos;

	//Se modifica un poco el offset
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	//Este offset es el movimiento, por lo que se le suma al pitch y al yaw
	yaw += xOffset;
	pitch += yOffset;

	//Para evitar saltos bruscos
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	//Actualizamos la cámara
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

void camaraLibre(Shader shaderProgram) {
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glm::mat4 view;
	view = glm::mat4();
	view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
	shaderProgram.setMat4("view", view);

	glm::mat4 projection;
	projection = glm::mat4();
	projection = glm::perspective(glm::radians(100.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 2000.0f);
	shaderProgram.setMat4("projection", projection);
}

void myIluminacion(Shader shaderProgram) {
	// LUZ AMBIENTE
	// Color de los objetos
	shaderProgram.setVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
	// Color de la luz
	shaderProgram.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
	// LUZ DIFUSA
	// Posicion de la fuente de luz
	shaderProgram.setVec3("lightPos", cameraPosition + cameraFront);
	// LUZ ESPECULAR
	// Direccion a la que mira la fuente de luz
	shaderProgram.setVec3("viewPos", cameraPosition + cameraFront);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//Redimensionar el viewport
	glViewport(0, 0, width, height);
}

void openGlInit() {
	//Incializaciones varias
	glClearDepth(1.0f);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void dibujarSuelo(Shader shaderProgram, unsigned int VAOSuelo) {
	//Dibujar suelo
	glm::mat4 transform = glm::mat4();
	transform = glm::translate(transform, glm::vec3(-80.0f, -3.5f, 80.0f));
	transform = glm::rotate(transform, (float)(glm::radians(180.0f)), glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::scale(transform, glm::vec3(70.0f, 0.0f, 70.0f));
	shaderProgram.setMat4("transform", transform);
	shaderProgram.setVec3("ourColor", glm::vec3(0.0f, 0.0f, 1.0f));
	glBindVertexArray(VAOSuelo);
	//Renderizar el cuadrado
	glDrawArrays(GL_TRIANGLES, 0, 6);
	
}

void Display(GLFWwindow* window, Shader shaderProgram, Mapa* mapa)
{
	//Color de background de pantalla por defecto
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camaraLibre(shaderProgram);
	myIluminacion(shaderProgram);
	/*glm::mat4 transform = glm::mat4();
	transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, -2.0f));
	shaderProgram.setMat4("transform", transform);
	shaderProgram.setVec3("ourColor", glm::vec3(1.0f, 0.0f, 0.0f));
	glBindVertexArray(VAOMuro);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, -3.0f));
	shaderProgram.setMat4("transform", transform);
	shaderProgram.setVec3("ourColor", glm::vec3(1.0f, 0.0f, 0.0f));
	glBindVertexArray(VAOMuro);
	glDrawArrays(GL_TRIANGLES, 0, 36);*/

	mapa->crearLaberinto(shaderProgram, VAOMuro);
	//printf("%lf %lf %lf\n", cameraPosition.x, cameraPosition.y, cameraPosition.z);
	dibujarSuelo(shaderProgram, VAOSuelo);
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
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

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

	//Configuración de los callbacks de GLFW
	glfwSetKeyCallback(window, handleInput);
	glfwSetCursorPosCallback(window, handleMouse);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

	//Creamos el suelo
	VAOSuelo = crearSuelo();

	//Usamos shader
	shaderProgram.use();

	//Aquí estoy haciendo el laberinto
	Mapa* mapa = new Mapa();

	//Loop de renderizado
	while (!glfwWindowShouldClose(window))
	{
		//Activar shader
		shaderProgram.use();
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