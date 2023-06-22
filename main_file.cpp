/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "lodepng.h"
#include "allmodels.h"
#include "shaderprogram.h"
#include "steering.h"
#include "obj_loader.hpp"
#include "object.hpp"
#include "read_texture.h"

//globals
float speed_y = 0.0f, speed_x = 0.0f, speed_z = 0.0f;
float speed = 0; //Prędkość kątowa obrotu obiektu
ShaderProgram* sp;
float aspectRatio = 1;

//objects
auto suzanne_data = load_obj("ModelFiles/suzanne.obj");
auto house_data = load_obj("ModelFiles/HOUSE_LGH_TIR_1.obj");
object suzanne;
object house;


//texture
GLuint tex;

//Color Textures
GLuint color_tex_house_metal;
GLuint color_tex_house_wood;
GLuint color_tex_house_roof;
GLuint color_tex_house_white_metal;
GLuint color_tex_house_glass;

//Normals Textures
GLuint normal_tex_house_metal;
GLuint normal_tex_house_wood;
GLuint normal_tex_house_roof;
GLuint normal_tex_house_white_metal;
GLuint normal_tex_house_glass;

//Metallic Textures
GLuint metallic_tex_house_metal;
GLuint metallic_tex_house_wood;
GLuint metallic_tex_house_roof;
GLuint metallic_tex_house_white_metal;
GLuint metallic_tex_house_glass;


//Roughness Textures
GLuint roughness_tex_house_metal;
GLuint roughness_tex_house_wood;
GLuint roughness_tex_house_roof;
GLuint roughness_tex_house_white_metal;
GLuint roughness_tex_house_glass;


//auto resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height);


//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void windowResizeCallback(GLFWwindow* window, int width, int height) {
	if (height == 0) return;
	aspectRatio = (float)width / (float)height;
	glViewport(0, 0, width, height);
}


//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
    initShaders();	
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(0, 0, 0, 1);//Ustaw czarny kolor czyszczenia ekranu

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glEnable(GL_DEPTH_TEST);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetWindowSizeCallback(window, windowResizeCallback);

	float maxAnisotropy;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);

	printf("%f", maxAnisotropy);

	float col[] = { 1.0f,0.0f,0.0f,1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, col);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	/*
	//read all textures
	//diffuse
	color_tex_house_metal = read_texture("ModelFiles/HOUSE_Texture/Metal/metal_basecolor.png");
	color_tex_house_wood = read_texture("ModelFiles/HOUSE_Texture/wood/Substance_graph_diffuse.png");
	color_tex_house_roof = read_texture("ModelFiles/HOUSE_Texture/roof/Roof_basecolor_red.png");
	color_tex_house_white_metal = read_texture("ModelFiles/HOUSE_Texture/white metal/white_meetal_basecolor.png");
	color_tex_house_glass = read_texture("ModelFiles/HOUSE_Texture/glass_faceced/glass_faceted_basecolor_blue.png");
	//normal
	normal_tex_house_metal = read_texture("ModelFiles/HOUSE_Texture/Metal/metal_normal.png");
	normal_tex_house_wood = read_texture("ModelFiles/HOUSE_Texture/wood/Substance_graph_normal.png");
	normal_tex_house_roof = read_texture("ModelFiles/HOUSE_Texture/roof/Roof_normal.png");
	normal_tex_house_white_metal = read_texture("ModelFiles/HOUSE_Texture/white metal/white_meetal_normal.png");
	normal_tex_house_glass = read_texture("ModelFiles/HOUSE_Texture/glass_faceced/glass_faceted_normal.png");
	//metallic
	metallic_tex_house_metal = read_texture("ModelFiles/HOUSE_Texture/Metal/metal_metallic.png");
	metallic_tex_house_wood = read_texture("ModelFiles/HOUSE_Texture/wood/Substance_graph_metallic.png");
	metallic_tex_house_roof = read_texture("ModelFiles/HOUSE_Texture/roof/Roof_metallic.png");
	metallic_tex_house_white_metal = read_texture("ModelFiles/HOUSE_Texture/white metal/white_meetal_metallic.png");
	metallic_tex_house_glass = read_texture("ModelFiles/HOUSE_Texture/glass_faceced/glass_faceted_metallic.png");
	//roughness
	roughness_tex_house_metal = read_texture("ModelFiles/HOUSE_Texture/Metal/metal_roughness.png");
	roughness_tex_house_wood = read_texture("ModelFiles/HOUSE_Texture/wood/Substance_graph_roughness.png");
	roughness_tex_house_roof = read_texture("ModelFiles/HOUSE_Texture/roof/Roof_roughness.png");
	roughness_tex_house_white_metal = read_texture("ModelFiles/HOUSE_Texture/white metal/white_meetal_roughness.png");
	roughness_tex_house_glass = read_texture("ModelFiles/HOUSE_Texture/glass_faceced/glass_faceted_roughness.png");
	*/


	//suzanne.loadObj("ModelFiles/suzanne.obj" , "./ModelFiles/");
	//suzanne.bindBuffers();
	sp = new ShaderProgram("v_simplest.glsl", NULL, "f_simplest.glsl");
	suzanne = object(suzanne_data, { "metal.png", "metal_spec.png" }, sp);
	//tex = read_texture("metal.png");

	house = object(house_data, { "ModelFiles/HOUSE_Texture/Metal/metal_basecolor.png", "ModelFiles/HOUSE_Texture/Metal/metal_metallic.png", "ModelFiles/HOUSE_Texture/Metal/metal_roughness.png", "ModelFiles/HOUSE_Texture/Metal/metal_normal.png" }, sp);
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
    freeShaders();
    //************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************
	glDeleteTextures(1, &tex);

	/*
	//delete all textures
	//diffuse
	glDeleteTextures(1, &color_tex_house_metal);
	glDeleteTextures(1, &color_tex_house_wood);
	glDeleteTextures(1, &color_tex_house_roof);
	glDeleteTextures(1, &color_tex_house_white_metal);
	glDeleteTextures(1, &color_tex_house_glass);
	//normal
	glDeleteTextures(1, &normal_tex_house_metal);
	glDeleteTextures(1, &normal_tex_house_wood);
	glDeleteTextures(1, &normal_tex_house_roof);
	glDeleteTextures(1, &normal_tex_house_white_metal);
	glDeleteTextures(1, &normal_tex_house_glass);
	//metallic
	glDeleteTextures(1, &metallic_tex_house_metal);
	glDeleteTextures(1, &metallic_tex_house_wood);
	glDeleteTextures(1, &metallic_tex_house_roof);
	glDeleteTextures(1, &metallic_tex_house_white_metal);
	glDeleteTextures(1, &metallic_tex_house_glass);
	//roughness
	glDeleteTextures(1, &roughness_tex_house_metal);
	glDeleteTextures(1, &roughness_tex_house_wood);
	glDeleteTextures(1, &roughness_tex_house_roof);
	glDeleteTextures(1, &roughness_tex_house_white_metal);
	glDeleteTextures(1, &roughness_tex_house_glass);
	*/
}

Models::Teapot teapot;



//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window, glm::mat4 Camera) {
	//************Tutaj umieszczaj kod rysujący obraz******************l
	glClearColor(BG_RED, BG_GREEN, BG_BLUE, 1); //Kolor czyszczący (wypełnienia okna) ustawiono na niebieski
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Wyczyszczenie bufora kolorów i bufora głębokości

	// Model
	glm::mat4 M = glm::mat4(1.0f);
	//M = glm::rotate(M, glm::radians(time_change), glm::vec3(0.0f, 1.0f, 0.0f));
	// Widok
	glm::mat4 V = Camera;

	// Perspektywa
	glm::mat4 P = glm::perspective(
		glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

	sp->use();//Aktywacja programu cieniującego
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P)); //Załadowanie macierzy rzutowania do programu cieniującego
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V)); //Załadowanie macierzy widoku do programu cieniującego
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));


	//suzanne.draw();
	house.draw();


	glfwSwapBuffers(window); //Przerzuć tylny bufor na przedni
}


int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "SteampunkOpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}



	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjujące



	glfwSetTime(0); //Wyzeruj timer
	//Główna pętla
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		cameraPos += objSpeed * (float)glfwGetTime();
		glm::mat4 camera = glm::lookAt(
			cameraPos, // im at
			cameraFront + cameraPos, // look to point
			cameraUp); // angle of top

		glfwSetTime(0);
		drawScene(window, camera); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
