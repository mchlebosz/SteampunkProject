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
#include "read_texture.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

//constexpr const char* obj_path = "ModelFiles/domek.obj";
//constexpr const char* tex_path = "ModelFiles/House_base_color.png"; //Diffuse - color of the object
//constexpr const char* normal_path = "ModelFiles/House_bump.png"; // Normal - bumps
//constexpr const char* height_path =  "ModelFiles/Med/Med_6-2_Height.jpg"; // Height - elevation
//constexpr const char* roughness_path = "ModelFiles/Med/Med_6-2_Roughness.jpg"; // Roughness - glossiness
//constexpr const char* specular_path = "ModelFile/House_Roughness.png"; // Specular - highlights

//constexpr const char* obj_path = "ModelFiles/Med/Med_6-2.obj";
//constexpr const char* tex_path = "ModelFiles/Med/Med_6-2_BaseColor.jpg"; //Diffuse - color of the object
//constexpr const char* normal_path = "ModelFiles/Med/Med_6-2_Normal.jpg"; // Normal - bumps
//constexpr const char* height_path =  "ModelFiles/Med/Med_6-2_Height.jpg"; // Height - elevation
//constexpr const char* roughness_path = "ModelFiles/Med/Med_6-2_Roughness.jpg"; // Roughness - glossiness
//constexpr const char* specular_path = "ModelFile/House_Roughness.png"; // Specular - highlights

constexpr const char* obj_path = "ModelFiles/Warehouse/object.obj";
constexpr const char* tex_path = "ModelFiles/Warehouse/texture.png"; //Diffuse - color of the object
constexpr const char* normal_path = "ModelFiles/Warehouse/normal.png"; // Normal - bumps
constexpr const char* occlusion_path =  "ModelFiles/Warehouse/occlusion.png"; // Occlusion - shadows
// 
//constexpr const char* roughness_path = "ModelFiles/Med/Med_6-2_Roughness.jpg"; // Roughness - glossiness
//constexpr const char* specular_path = "ModelFile/House_Roughness.png"; // Specular - highlights

Assimp::Importer importer;

const aiScene* scene = importer.ReadFile(obj_path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
auto mesh = scene->mMeshes[0];
std::vector<float> verticesv;
std::vector<float> normalsv;
std::vector<float> colorsv;
std::vector<float> texCoordsv;
GLuint tex0;
GLuint tex1;
GLuint tex2;

//globals
float speed_y = 0.0f, speed_x = 0.0f, speed_z = 0.0f;
float speed = 0; //Prędkość kątowa obrotu obiektu
float aspectRatio = 16.0f / 9.0f; //Stosunek wymiarów widoku
ShaderProgram* sp;

//auto resize
void windowResizeCallback(GLFWwindow* window, int width, int height) {
	if (height == 0) return;
	aspectRatio = (float)width / (float)height;
	glViewport(0, 0, width, height);
}

//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
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

	//suzanne.loadObj("ModelFiles/suzanne.obj" , "./ModelFiles/");
	//suzanne.bindBuffers();
	sp = new ShaderProgram("v_simplest.glsl", NULL, "f_simplest.glsl");
	tex0 = read_texture(tex_path);
	//tex1 = read_texture(tex_path);
	tex1 = read_texture(normal_path);
	tex2 = read_texture(occlusion_path);

	//suzanne = object(suzanne_data, { tex_path, normal_path }, sp);

	for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
		// Vertices
		verticesv.push_back(mesh->mVertices[i].x);
		verticesv.push_back(mesh->mVertices[i].y);
		verticesv.push_back(mesh->mVertices[i].z);
		verticesv.push_back(1.0f);

		normalsv.push_back(mesh->mNormals[i].x);
		normalsv.push_back(mesh->mNormals[i].y);
		normalsv.push_back(mesh->mNormals[i].z);
		normalsv.push_back(0.0f);
		
		// Colors (Assuming you have per-vertex colors)
		if (mesh->HasVertexColors(0)) {
			colorsv.push_back(mesh->mColors[0][i].r);
			colorsv.push_back(mesh->mColors[0][i].g);
			colorsv.push_back(mesh->mColors[0][i].b);
			colorsv.push_back(1.0f);
		}

		if (mesh->HasTextureCoords(0)) {
			texCoordsv.push_back(mesh->mTextureCoords[0][i].x);
			texCoordsv.push_back(mesh->mTextureCoords[0][i].y);
		}
	}
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
    freeShaders();
    //************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************
	glDeleteTextures(1, &tex0);
	glDeleteTextures(1, &tex1);
	glDeleteTextures(1, &tex2);
	delete sp;
}


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

	auto mesh = scene->mMeshes[0];
	float* vertices = verticesv.data();
	float* colors = colorsv.data();
	float* normals = normalsv.data();
	float* texCoords = texCoordsv.data();


	glEnableVertexAttribArray(sp->a("vertex"));  //Włącz przesyłanie danych do atrybutu vertex
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, vertices); //Wskaż tablicę z danymi dla atrybutu vertex

	glEnableVertexAttribArray(sp->a("color"));  //Włącz przesyłanie danych do atrybutu color
	glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, colors); //Wskaż tablicę z danymi dla atrybutu color

	glEnableVertexAttribArray(sp->a("normal"));  //Włącz przesyłanie danych do atrybutu normal
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, normals); //Wskaż tablicę z danymi dla atrybutu normal

	glEnableVertexAttribArray(sp->a("texCoord0"));  //Włącz przesyłanie danych do atrybutu texCoord
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, texCoords); //Wskaż tablicę z danymi dla atrybutu texCoord

	glUniform1i(sp->u("textureMap0"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex0);

	glUniform1i(sp->u("textureMap1"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex1);

	glUniform1i(sp->u("textureMap2"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, tex2);

	glDrawArrays(GL_TRIANGLES, 0, mesh->mNumVertices); //Narysuj obiekt

	glDisableVertexAttribArray(sp->a("vertex"));  //Wyłącz przesyłanie danych do atrybutu vertex
	glDisableVertexAttribArray(sp->a("color"));  //Wyłącz przesyłanie danych do atrybutu color
	glDisableVertexAttribArray(sp->a("normal"));  //Wyłącz przesyłanie danych do atrybutu normal
	glDisableVertexAttribArray(sp->a("texCoord0"));  //Wyłącz przesyłanie danych do atrybutu texCoord0

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
