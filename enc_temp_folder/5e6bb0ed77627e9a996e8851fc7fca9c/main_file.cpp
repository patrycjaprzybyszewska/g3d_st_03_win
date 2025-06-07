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
bool visible2 = true;
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "allmodels.h"
#include "cone.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include <ctime>
#include <cstdlib>
// Inicjalizacja generatora liczb losowych
//zmienna gobalna
GLuint back;

float speed = 0; //Prędkość kątowa obrotu obiektu
int count = 0;
Models::Sphere limfocyt(0.5, 36, 36);
Models::Cone receptor;
std::vector<glm::vec3> receptorPositions;
glm::vec3 randomPosition(float min, float max) {
	return glm::vec3(
		min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min))),
		min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min))),
		0.0f // zakładamy ruch w 2D
	);
}

glm::vec3 position = randomPosition(-1.0f, 1.0f);
glm::vec3 velocity1(0.01f, 0.008f, 0.0f);
glm::vec3 position2 = randomPosition(1.0f, 1.0f);
glm::vec3 velocity2(-0.01f, -0.006f, 0.0f);

//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}
const int gridSize = 10; // 10x10
const float worldMin = -1.0f;
const float worldMax = 1.0f;
//load i read texture

//Procedura obsługi klawiatury
void key_callback(GLFWwindow* window, int key,
	int scancode, int action, int mods) {

	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_LEFT) speed = -PI; //Jeżeli wciśnięto klawisz "w lewo" ustaw prędkość na -PI
		if (key == GLFW_KEY_RIGHT) speed = PI; //Jeżeli wciśnięto klawisz "w prawo" ustaw prędkość na PI
	}

	if (action == GLFW_RELEASE) {
		speed = 0; //Jeżeli puszczono klawisz ustaw prędkość obrotu na zero
	}

}
/*
void drawReceptors(const glm::vec3& center) {
	for (const auto& dir : receptorPositions) {
		glm::vec3 pos = center + dir * 0.5f; // 0.5 = promień limfocytu

		glm::mat4 M = glm::translate(glm::mat4(1.0f), pos);
		M = glm::scale(M, glm::vec3(0.1f)); // Mały cube

		glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M));
		glUniform4f(spLambert->u("color"), 1.0f, 0.0f, 0.0f, 1.0f); // Czerwony receptor

		receptor.drawSolid();
	}
}*/
#include <glm/gtx/quaternion.hpp> 


void drawReceptors(const glm::vec3& center) {
	for (const auto& dir : receptorPositions) {
		glm::vec3 normDir = glm::normalize(dir);
		float sphereRadius = 0.5f;
		float coneHeight = 0.1f;

		// Pozycja: przesunięcie od środka o promień + połowa wysokości stożka
		glm::vec3 pos = center + normDir * (sphereRadius + coneHeight * 0.5f);

		// Rotacja od osi Y do kierunku normDir
		glm::vec3 up(0.0f, 1.0f, 0.0f);
		glm::quat rotation = glm::rotation(up, normDir);
		glm::mat4 rotationMatrix = glm::toMat4(rotation);

		// Macierz modelu
		glm::mat4 M = glm::translate(glm::mat4(1.0f), pos);
		M *= rotationMatrix;
		M = glm::scale(M, glm::vec3(0.1f)); // Skalowanie receptorów

		glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M));
		glUniform4f(spLambert->u("color"), 1.0f, 0.0f, 0.0f, 1.0f); // Czerwony
		//
		receptor.drawSolid();
	}
}
void drawSecondLimfocyt() {
	glm::mat4 Ms = glm::translate(glm::mat4(1.0f), position2);
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(Ms));
	glUniform4f(spLambert->u("color"), 0.0f, 1.0f, 0.0f, 1.0f); // Zielony limfocyt
	if (visible2 == true) {
		limfocyt.drawSolid();
		drawReceptors(position2);
	}

}
GLuint ReadTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);
	unsigned width, height;
	std::vector<unsigned char> image;


	unsigned error = lodepng::decode(image, width, height, filename);

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}
void LoadTexture(glm::mat4 P, glm::mat4 V, glm::mat4 M, GLuint tex) {
	spTextured->use();

	glUniformMatrix4fv(spTextured->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spTextured->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(spTextured->u("M"), 1, false, glm::value_ptr(M));

	glEnableVertexAttribArray(spTextured->a("vertex"));
	glVertexAttribPointer(spTextured->a("vertex"), 4, GL_FLOAT, false, 0, Models::cube.vertices);

	glEnableVertexAttribArray(spTextured->a("texCoord"));
	glVertexAttribPointer(spTextured->a("texCoord"), 2, GL_FLOAT, false, 0, Models::cube.texCoords);

	glEnableVertexAttribArray(spTextured->a("normal"));
	glVertexAttribPointer(spTextured->a("normal"), 3, GL_FLOAT, false, 0, Models::cube.normals);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(spTextured->u("tex"), 0);

	glDrawArrays(GL_TRIANGLES, 0, Models::cube.vertexCount);

	glDisableVertexAttribArray(spTextured->a("vertex"));
	glDisableVertexAttribArray(spTextured->a("texCoord"));
	glDisableVertexAttribArray(spTextured->a("normal"));
}

//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	initShaders();
	receptorPositions = {
	glm::vec3(1, 0, 0),
	glm::vec3(-1, 0, 0),
	glm::vec3(0, 1, 0),
	glm::vec3(0, -1, 0),
	glm::vec3(0, 0, 1),
	glm::vec3(0, 0, -1),
	glm::normalize(glm::vec3(1, 1, 1)),
	glm::normalize(glm::vec3(-1, -1, -1))
	};
	
	glEnable(GL_DEPTH_TEST);
	back = ReadTexture("krew.png");
	

	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(0, 0, 0, 1);//Ustaw czarny kolor czyszczenia ekranu
	glEnable(GL_DEPTH_TEST); //Włącz test głębokości pikseli
	glfwSetKeyCallback(window, key_callback); //Zarejestruj procedurę obsługi klawiatury
}


void drawLimfocyt() {
	glm::mat4 Ms = glm::translate(glm::mat4(1.0f), position);//Macierz słońca to macierz jednostkowa
	

	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(Ms));  //Załadowanie macierzy modelu do programu cieniującego
	glUniform4f(spLambert->u("color"), 1, 1, 0, 1);


	limfocyt.drawSolid();
	drawReceptors(position);
}
//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
	freeShaders();
	//gl delete texture
	//************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************//pamiec tesktur - sprobkowac 
}
glm::ivec2 getGridCell(const glm::vec3& pos) {
	float cellSize = (worldMax - worldMin) / gridSize;
	int x = static_cast<int>((pos.x - worldMin) / cellSize);
	int y = static_cast<int>((pos.y - worldMin) / cellSize);

	//glDeleteTextures(1, &back);
	return glm::ivec2(x, y);
}

//glm::vec3 velocity1, velocity2;
float r = 0.05f; // zasięg kroku

void updatePosition() {
	float fi1 = ((float)(rand()) / RAND_MAX) * 2.0f * PI;
	velocity1 = glm::vec3(r * cos(fi1), r * sin(fi1), 0.0f);
	position += velocity1;

	float fi2 = ((float)(rand()) / RAND_MAX) * 2.0f * PI;
	velocity2 = glm::vec3(r * cos(fi2), r * sin(fi2), 0.0f);
	position2 += velocity2;
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window, float angle) {
	//************Tutaj umieszczaj kod rysujący obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Wyczyszczenie bufora kolorów i bufora głębokości
	//position += velocity;
//	position2 += velocity2;
	/*
	int n = 10000;
	int k = 3;
	float r = 0.1f;
	position = glm::vec3(0.0f);

	position2 = glm::vec3(3.0f, 3.0f, 0.0f);
	for (int i = 0; i < n; i++) {
		float fi = (rand() % k) * 2 * PI / k;
		position.x += r * cos(fi);
		position.y += r * sin(fi);
		position2.x += r * cos(fi);
		position2.y += r * sin(fi);
	}*/
	updatePosition();
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		visible2 = true;
	}
	glm::mat4 M5 = glm::mat4(1.0f);
	M5 = glm::translate(M5, glm::vec3(0.0f, 0.0f, 1.0f));
	M5 = glm::rotate(M5, 3.14f / 2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	M5 = glm::scale(M5, glm::vec3(1.0f, 0.01f, 1.0f));
	
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M5));
	Models::cube.drawSolid();
	
	float radius = 0.5f;
	bool collisionX = position.x + radius >= position2.x - radius &&
		position2.x + radius >= position.x - radius;


	bool collisionY = position.y + radius >= position2.y - radius &&
		position2.y + radius >= position.y - radius;

	if (collisionX && collisionY) {
		visible2 = false;
	}


	glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 50.0f); //Wyliczenie macierzy rzutowania
	glm::mat4 V = glm::lookAt(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Wyliczenie macierzy widoku
	LoadTexture(P, V, M5, back);
	spLambert->use();//Aktywacja programu cieniującego
	glUniformMatrix4fv(spLambert->u("P"), 1, false, glm::value_ptr(P)); //Załadowanie macierzy rzutowania do programu cieniującego
	glUniformMatrix4fv(spLambert->u("V"), 1, false, glm::value_ptr(V)); //Załadowanie macierzy widoku do programu cieniującego


	drawLimfocyt();
	if (visible2 == true) {
		drawSecondLimfocyt();
	}
	// Rysowanie stożka
	//glm::mat4 M_cone = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.2f, 0.0f)); // Nad limfocytem
	//M_cone = glm::scale(M_cone, glm::vec3(0.2f)); // Zmniejsz stożek

	//glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M_cone));
	//glUniform4f(spLambert->u("color"), 0.2f, 0.8f, 1.0f, 1.0f); // Jasnoniebieski

	//Models::cone.drawSolid(true); // Smooth shading = true


	//Skopiowanie bufora ukrytego do widocznego. Z reguły ostatnie polecenie w procedurze drawScene.
	glfwSwapBuffers(window);
}

/*
void updatePosition() {
	static int step = 0;
	if (step >= 10000) return;
	int dir = rand() % k;
	float fi = dir * 2 * PI / k;
	float dx = r * cos(fi);
	float dy = r * sin(fi);
	position.x += dx;
	position.y += dy;
	step++;
}
*/
int main(void)
{

	srand(time(NULL));
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		//read texture
		exit(EXIT_FAILURE);
	}
	GLuint tex = ReadTexture("bricks.png");
	fprintf(stderr, "Ładowanie tekstury krew.png");

	window = glfwCreateWindow(500, 500, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

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


	float angle = 0; //Aktualny kąt obrotu obiektu
	glfwSetTime(0); //Wyzeruj timer
	//Główna pętla
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		angle += speed * glfwGetTime(); //Oblicz przyrost kąta po obrocie
		glfwSetTime(0); //Wyzeruj timer
		drawScene(window, angle); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
//ruchy bronwna , perlinga

//thre.js

/*	glm::mat4 Ms = glm::translate(glm::mat4(1.0f), position);//Macierz słońca to macierz jednostkowa
	//spTest->use();
//	GLuint timeLoc = spTest->u("time");
//	glUniform1f(timeLoc, glfwGetTime());
	glUniformMatrix4fv(spTest->u("M"), 1, false, glm::value_ptr(Ms));  //Załadowanie macierzy modelu do programu cieniującego
	glUniform4f(spTest->u("color"), 1, 1, 0, 1);

*/

// 