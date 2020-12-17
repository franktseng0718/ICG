#define GLM_ENABLE_EXPERIMENTAL

#include "Object.h"
#include "FreeImage.h"
#include "glew.h"
#include "freeglut.h"
#include "shader.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include <iostream>
#include <string>
#include <math.h>
#include <stb_image.h>
using namespace std;

void shaderInit();
void bindbufferInit();
void textureInit();
void display();
void idle();
void reshape(GLsizei w, GLsizei h);
void keyboard(unsigned char key, int x, int y);
void DrawLightPoint();
void DrawUmbreon();
void LoadTexture(unsigned int&, const char*);

GLuint program, Phongprogram, Toonprogram, Edgeprogram;
GLuint VAO, VBO[3];
unsigned int ToonTexture, modeltexture;
float windowSize[2] = { 600, 600 };
float angle = 0.0f;
glm::vec3 WorldLightPos = glm::vec3(2, 5, 5);
glm::vec3 WorldCamPos = glm::vec3(7.5, 5.0, 7.5);

Object* model = new Object("Umbreon.obj");

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(windowSize[0], windowSize[1]);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("hw3");

	glewInit();
	shaderInit();
	bindbufferInit();
	textureInit();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}

void shaderInit() {
	GLuint vert = createShader("Shaders/Phongshading.vert", "vertex");
	GLuint frag = createShader("Shaders/Phongshading.frag", "fragment");
	Phongprogram = createProgram(vert, frag);
	GLuint vert1 = createShader("Shaders/Toonshading.vert", "vertex");
	GLuint frag1 = createShader("Shaders/Toonshading.frag", "fragment");
	Toonprogram = createProgram(vert1, frag1);
	GLuint vert2 = createShader("Shaders/edge.vert", "vertex");
	GLuint frag2 = createShader("Shaders/edge.frag", "fragment");
	Edgeprogram = createProgram(vert2, frag2);
	//// TODO: ////
	// create the shaders and programs you need.

	program = Phongprogram;
}

void bindbufferInit() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(3, VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model->positions[0]) * model->positions.size(), &model->positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model->normals[0]) * model->normals.size(), &model->normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model->texcoords[0]) * model->texcoords.size(), &model->texcoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void textureInit() {
	LoadTexture(modeltexture, "Umbreon.jpg");
}

glm::mat4 getV()
{
	// set camera position and configuration
	return glm::lookAt(glm::vec3(WorldCamPos.x, WorldCamPos.y, WorldCamPos.z), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

glm::mat4 getP()
{
	// set perspective view
	float fov = 45.0f;
	float aspect = windowSize[0] / windowSize[1];
	float nearDistance = 1.0f;
	float farDistance = 1000.0f;
	return glm::perspective(glm::radians(fov), aspect, nearDistance, farDistance);
}

void display() {
	//Clear the buffer
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawLightPoint();
	DrawUmbreon();
	angle += 0.5;
	glutSwapBuffers();
}

void reshape(GLsizei w, GLsizei h) {
	windowSize[0] = w;
	windowSize[1] = h;
}

void LoadTexture(unsigned int& texture, const char* tFileName) {
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(tFileName, &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'a':
	{
		glm::vec3 Zoomin = -WorldLightPos;
		glm::vec3 Left = glm::cross(glm::vec3(0, 1, 0), Zoomin);
		Left = glm::normalize(Left);
		WorldLightPos += Left;
		break;
	}
	case 's':
	{
		glm::vec3 Zoomin = -WorldLightPos;
		glm::vec3 Right = glm::cross(Zoomin, glm::vec3(0, 1, 0));
		Right = glm::normalize(Right);
		WorldLightPos += Right;
		break;
	}
	case 'z':
	{
		glm::vec3 Zoomin = -WorldLightPos;
		Zoomin = glm::normalize(Zoomin);
		WorldLightPos += Zoomin;
		break;
	}
	case 'x':
	{
		glm::vec3 Zoomout = WorldLightPos;
		Zoomout = glm::normalize(Zoomout);
		WorldLightPos += Zoomout;
		break;
	}
	case 'r':
	{
		WorldLightPos = glm::vec3(2, 5, 5);
		break;
	}
	case '1':
	{
		program = Phongprogram;
		//// TODO: ////
		// switch to the program which you want to use
		break;
	}
	case '2':
	{
		program = Toonprogram;
		//// TODO: ////
		// switch to the program which you want to use
		break;
	}
	case '3':
	{
		program = Edgeprogram;
		//// TODO: ////
		// switch to the program which you want to use
		break;
	}
	default:
	{
		break;
	}
	}
}

void idle() {
	glutPostRedisplay();
}

void DrawUmbreon()
{
	glUseProgram(program);

	glm::mat4 M(1.0f);
	M = glm::rotate(M, glm::radians(angle), glm::vec3(0, 1, 0));
	//M = glm::translate(M, glm::vec3(0, 0, 0));

	GLuint ModelMatrixID = glGetUniformLocation(program, "M");
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &M[0][0]);

	glm::mat4 V = getV();
	ModelMatrixID = glGetUniformLocation(program, "V");
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &V[0][0]);

	glm::mat4 P = getP();
	ModelMatrixID = glGetUniformLocation(program, "P");
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &P[0][0]);

	glm::mat4 normalmatrix = transpose(inverse(M));
	//std::cout << glm::to_string(normalmatrix) << std::endl;
	ModelMatrixID = glGetUniformLocation(program, "normalmatrix");
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &normalmatrix[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, modeltexture);
	glUniform1i(glGetUniformLocation(program, "texture"), 0);

	///// TODO: ////
	GLuint WorldLightPosID = glGetUniformLocation(program, "WorldLightPos");
	glUniform3fv(WorldLightPosID, 1, &WorldLightPos[0]);

	GLuint WorldCamPosID = glGetUniformLocation(program, "WorldCamPos");
	glUniform3fv(WorldCamPosID, 1, &WorldCamPos[0]);

	GLuint Ka = glGetUniformLocation(program, "Ka");
	glUniform3f(Ka, 1, 1, 1);

	GLuint Kd = glGetUniformLocation(program, "Kd");
	glUniform3f(Kd, 1, 1, 1);

	GLuint Ks = glGetUniformLocation(program, "Ks");
	glUniform3f(Ks, 1, 1, 1);

	GLuint La = glGetUniformLocation(program, "La");
	glUniform3f(La, 0.2, 0.2, 0.2);

	GLuint Ld = glGetUniformLocation(program, "Ld");
	glUniform3f(Ld, 0.8, 0.8, 0.8);

	GLuint Ls = glGetUniformLocation(program, "Ls");
	glUniform3f(Ls, 0.5, 0.5, 0.5);

	GLuint gloss = glGetUniformLocation(program, "gloss");
	glUniform1i(gloss, 25);

	gloss = glGetUniformLocation(program, "cutoff");
	glUniform1f(gloss, 0.9);

	gloss = glGetUniformLocation(program, "edgefalloff");
	glUniform1i(gloss, 3);
	// Pass all variable to shaders and trigger by Uniform (like:WorldLightPos, WorldCamPos, Ka, La ......etc)

	glBindVertexArray(VAO);
	glDrawArrays(GL_QUADS, 0, 4 * model->fNum);
	glBindVertexArray(0);
	glActiveTexture(0);
	glUseProgram(0);
}

void DrawLightPoint() {
	// viewport transformation
	glViewport(0, 0, windowSize[0], windowSize[1]);

	// projection transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)windowSize[0] / (GLfloat)windowSize[1], 1.0, 1000.0);

	// viewing and modeling transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(WorldCamPos.x, WorldCamPos.y, WorldCamPos.z,// eye
		0.0, 0.0, 0.0,     // center
		0.0, 1.0, 0.0);    // up
	glPushMatrix();
	glTranslatef(WorldLightPos.x, WorldLightPos.y, WorldLightPos.z);
	glColor3f(1.0f, 1.0f, 0.0f);
	glutSolidSphere(0.5f, 50.0f, 50.0f);
	glPopMatrix();
}