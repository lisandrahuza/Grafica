//
//  main.cpp
//  OpenGL_Shader_Example_step1
//
//  Created by CGIS on 02/11/16.
//  Copyright � 2016 CGIS. All rights reserved.
//

#if defined (__APPLE__)
#define GLFW_INCLUDE_GLCOREARB
#else
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/glm.hpp>//core glm functionality
#include <glm/gtc/matrix_transform.hpp>//glm extension for generating common transformation matrices
#include <glm/gtc/type_ptr.hpp>

#include <string>

#include "Shader.hpp"
#include "Camera.hpp"
#include "Model3D.hpp"
#include "SkyBox.hpp"

int glWindowWidth = 800;
int glWindowHeight = 600;
int retina_width, retina_height;
GLFWwindow* glWindow = NULL;

const unsigned int SHADOW_WIDTH = 2048;
const unsigned int SHADOW_HEIGHT = 2048;

glm::mat4 model;
glm::mat4 model1;
glm::mat4 model2;
glm::mat4 model3;
GLint modelLoc;

glm::mat3 normalMatrix;
GLuint normalMatrixLoc;

glm::mat4 projection;
GLint projLoc;

glm::vec3 lightDir;
GLuint lightDirLoc;
glm::vec3 lightColor;
GLuint lightColorLoc;

//gps::Camera myCamera(glm::vec3(0.0f, 5.0f, 15.0f), glm::vec3(0.0f, 5.0f, -10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//gps::Camera myCamera(glm::vec3(0.0f, -3.0f, 15.0f), glm::vec3(0.0f, 5.0f, -10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
gps::Camera myCamera(glm::vec3(0.0f, -20.0f, 60.0f), glm::vec3(0.0f, -20.0f, 50.0f), glm::vec3(0.0f, 1.0f, 0.0f));


float cameraSpeed = 0.8f;

bool pressedKeys[1024];
float angle = 0.0f;
float scale = 1.0f;
float translate_x = 0.0f;
float translate_y = 0.0f;
float translate_z = 0.0f;
float translate_mouse_x = 0.0f;

gps::Model3D myModel;
gps::Model3D myModel1;
gps::Model3D myModel2;
gps::Model3D myModel3;
gps::Shader myCustomShader;

GLuint verticesVBO;
GLuint verticesEBO;
GLuint objectVAO;
GLuint texture;



bool animatePresentation = true;
double presentationStartTime;
float presentationTranslateX = 0.0f;
float presentationTranslateY = 0.0f;
float presentationScale = 1.0f;
float presentationRotationAngle = 0.0f;

bool isAnimationPaused = true;
float anglePresentation = true;

gps::SkyBox skyBox;
gps::Shader skyBoxShader;


void windowResizeCallback(GLFWwindow* window, int width, int height) {

	fprintf(stdout, "window resized to width: %d , and height: %d\n", width, height);
	glViewport(0, 0, width, height);
	glfwGetFramebufferSize(glWindow, &retina_width, &retina_height);
	//initialize the projection matrix
	projection = glm::perspective(glm::radians(55.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
	//send matrix data to shader
	projLoc = glGetUniformLocation(myCustomShader.shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {

		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024) {

		if (action == GLFW_PRESS) {
			pressedKeys[key] = true;
		}
		else if (action == GLFW_RELEASE) {
			pressedKeys[key] = false;
		}
	}
}

void processMovement()
{

	if (pressedKeys[GLFW_KEY_Q]) {
		angle += 1.0f;
	}

	if (pressedKeys[GLFW_KEY_E]) {
		angle -= 1.0f;
	}

	if (pressedKeys[GLFW_KEY_W]) {
		myCamera.move(gps::MOVE_FORWARD, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_S]) {
		myCamera.move(gps::MOVE_BACKWARD, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_A]) {
		myCamera.move(gps::MOVE_LEFT, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_D]) {
		myCamera.move(gps::MOVE_RIGHT, cameraSpeed);
	}
	if (pressedKeys[GLFW_KEY_Z]) {

		scale *= 1.005f;
	}
	if (pressedKeys[GLFW_KEY_C]) {

		scale /= 1.005f;
	}


	if (pressedKeys[GLFW_KEY_RIGHT]) {

		translate_x += 1.0f;
	}
	if (pressedKeys[GLFW_KEY_LEFT]) {

		translate_x -= 1.0f;
	}
	if (pressedKeys[GLFW_KEY_UP]) {

	
		translate_y += 1.0f;
	}
	if (pressedKeys[GLFW_KEY_DOWN]) {


		translate_y -= 1.0f;

	}
	if (pressedKeys[GLFW_KEY_N]) {


		translate_z += 1.0f;
	}
	if (pressedKeys[GLFW_KEY_M]) {


		translate_z -= 1.0f;

	}

	if (glfwGetMouseButton(glWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		translate_mouse_x += 1.0f;
		printf("st");
	}

	if (glfwGetMouseButton(glWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		translate_mouse_x -= 1.0f;
	}
	if (pressedKeys[GLFW_KEY_P]) {
		isAnimationPaused = !isAnimationPaused;
	}


	//solid
	if (pressedKeys[GLFW_KEY_H]) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	}
	//wireframe
	if (pressedKeys[GLFW_KEY_J]) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	//poligonal
	if (pressedKeys[GLFW_KEY_K]) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	}
	//smooth
	if (pressedKeys[GLFW_KEY_L]) {
		glShadeModel(GL_SMOOTH);

	}
}

bool initOpenGLWindow() {

	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//window scaling for HiDPI displays
	glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

	// for multisampling/antialising
	glfwWindowHint(GLFW_SAMPLES, 4);

	glWindow = glfwCreateWindow(glWindowWidth, glWindowHeight, "OpenGL Texturing", NULL, NULL);

	if (!glWindow) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return false;
	}

	glfwSetWindowSizeCallback(glWindow, windowResizeCallback);
	glfwSetKeyCallback(glWindow, keyboardCallback);
	glfwSetCursorPosCallback(glWindow, mouseCallback);
	//glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwMakeContextCurrent(glWindow);

	glfwSwapInterval(1);

#if not defined (__APPLE__)
	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();
#endif

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	//for RETINA display
	glfwGetFramebufferSize(glWindow, &retina_width, &retina_height);

	return true;
}

GLuint ReadTextureFromFile(const char* file_name) {

	int x, y, n;
	int force_channels = 4;
	unsigned char* image_data = stbi_load(file_name, &x, &y, &n, force_channels);

	if (!image_data) {
		fprintf(stderr, "ERROR: could not load %s\n", file_name);
		return false;
	}
	// NPOT check
	if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) {
		fprintf(
			stderr, "WARNING: texture %s is not power-of-2 dimensions\n", file_name
		);
	}

	int width_in_bytes = x * 4;
	unsigned char* top = NULL;
	unsigned char* bottom = NULL;
	unsigned char temp = 0;
	int half_height = y / 2;

	for (int row = 0; row < half_height; row++) {

		top = image_data + row * width_in_bytes;
		bottom = image_data + (y - row - 1) * width_in_bytes;

		for (int col = 0; col < width_in_bytes; col++) {

			temp = *top;
			*top = *bottom;
			*bottom = temp;
			top++;
			bottom++;
		}
	}

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_SRGB, //GL_SRGB,//GL_RGBA,
		x,
		y,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		image_data
	);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}


float delta = 0.0f;
float delta2 = 0.0f;

double lastTimeStamp = glfwGetTime()+32400.0;
void renderScene()
{

	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, retina_width, retina_height);

	//initialize the model matrix
	model = glm::mat4(1.0f);
	model1 = glm::mat4(1.0f);
	model2 = glm::mat4(1.0f);
	model3 = glm::mat4(1.0f);

	if (!isAnimationPaused) {
		angle+= 1.6;
	}

	modelLoc = glGetUniformLocation(myCustomShader.shaderProgram, "model");
	myCustomShader.useShaderProgram();


	//initialize the view matrix
	glm::mat4 view = myCamera.getViewMatrix();
	//send matrix data to shader
	GLint viewLoc = glGetUniformLocation(myCustomShader.shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	model = glm::scale(model, glm::vec3(scale, scale, scale));
	model = glm::translate(model, glm::vec3(translate_x, translate_y, translate_z));

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "diffuseTexture"), 0);
	glBindTexture(GL_TEXTURE_2D, texture);

	normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
	normalMatrixLoc = glGetUniformLocation(myCustomShader.shaderProgram, "normalMatrix");
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	//set the light direction (direction towards the light)
	lightDir = glm::vec3(-0.2f, -1.0f, 0.0f);
	lightDirLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightPos");
	glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view)) * lightDir));

	//set light color
	lightColor = glm::vec3(2.0f, 2.0f, 2.0f); //white light
	lightColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor");
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

	GLint pointLightPosLoc = glGetUniformLocation(myCustomShader.shaderProgram, "pointLightPos");
	GLint pointLightColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "pointLightColor");

	glm::vec3 pointLightPos = glm::vec3(-11.707f, 5.137f, -24.4f);
	glm::vec3 pointLightColor = glm::vec3(5.0f, 5.0f, 5.0f);

	glUniform3fv(pointLightPosLoc, 1, glm::value_ptr(pointLightPos));
	glUniform3fv(pointLightColorLoc, 1, glm::value_ptr(pointLightColor));


	glBindVertexArray(objectVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	myModel.Draw(myCustomShader);


	float oscillation = glm::sin(delta * glm::pi<float>()) * 15.0f;

	model1 = glm::mat4(1.0f);  // Reset model1 matrix
	model1 = glm::translate(model1, glm::vec3(oscillation, 0, 0));

	delta += 0.001;

	model1 = glm::rotate(model1, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	model1 = glm::rotate(model1, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model1 = glm::scale(model1, glm::vec3(scale, scale, scale));
	model1 = glm::translate(model1, glm::vec3(translate_x, translate_y, translate_z));

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1));

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "diffuseTexture"), 0);
	glBindTexture(GL_TEXTURE_2D, texture);

	normalMatrix = glm::mat3(glm::inverseTranspose(view * model1));
	normalMatrixLoc = glGetUniformLocation(myCustomShader.shaderProgram, "normalMatrix");
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	//set the light direction (direction towards the light)
	lightDir = glm::vec3(-0.2f, -1.0f, 0.0f);
	lightDirLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightPos");
	glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view)) * lightDir));

	//set light color
	lightColor = glm::vec3(2.0f, 2.0f, 2.0f); //white light
	lightColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor");
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

	glBindVertexArray(objectVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	for (int i = 0; i < 500; i++)
		myModel1.Draw(myCustomShader);

	float oscillation1 = glm::sin(delta2 * glm::pi<float>()) * 20.0f;

	model2 = glm::mat4(1.0f);  
	model2 = glm::translate(model2, glm::vec3( 0, 0,oscillation1 ));

	delta2 += 0.004;


	model2 = glm::rotate(model2, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	model2 = glm::rotate(model2, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	model2 = glm::scale(model2, glm::vec3(scale, scale, scale));
	model2 = glm::translate(model2, glm::vec3(translate_x, translate_y, translate_z));

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "diffuseTexture"), 0);
	glBindTexture(GL_TEXTURE_2D, texture);

	normalMatrix = glm::mat3(glm::inverseTranspose(view * model2));
	normalMatrixLoc = glGetUniformLocation(myCustomShader.shaderProgram, "normalMatrix");
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	//set the light direction (direction towards the light)
	lightDir = glm::vec3(-0.2f, -1.0f, 0.0f);
	lightDirLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightPos");
	glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view)) * lightDir));

	//set light color
	lightColor = glm::vec3(2.0f, 2.0f, 2.0f); //white light
	lightColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor");
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

	glBindVertexArray(objectVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	for (int i = 0; i < 500; i++)
		myModel2.Draw(myCustomShader);


	model3 = glm::mat4(1.0f);

	model3 = glm::rotate(model3, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	model3 = glm::rotate(model3, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model3 = glm::scale(model3, glm::vec3(scale, scale, scale));
	model3 = glm::translate(model3, glm::vec3(translate_x, translate_y, translate_z));
	model3 = glm::translate(model3, glm::vec3(translate_mouse_x,0, 0));

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model3));

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "diffuseTexture"), 0);
	glBindTexture(GL_TEXTURE_2D, texture);

	normalMatrix = glm::mat3(glm::inverseTranspose(view * model3));
	normalMatrixLoc = glGetUniformLocation(myCustomShader.shaderProgram, "normalMatrix");
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	//set the light direction (direction towards the light)
	lightDir = glm::vec3(-0.2f, -1.0f, 0.0f);
	lightDirLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightPos");
	glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view)) * lightDir));

	//set light color
	lightColor = glm::vec3(2.0f, 2.0f, 2.0f); //white light
	lightColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor");
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

	glBindVertexArray(objectVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	for (int i = 0; i < 500; i++)
		myModel3.Draw(myCustomShader);


}


void cleanup() {

	// GPU cleanup
	glDeleteBuffers(1, &verticesVBO);
	glDeleteBuffers(1, &verticesEBO);
	glDeleteVertexArrays(1, &objectVAO);
	glDeleteTextures(1, &texture);

	glfwDestroyWindow(glWindow);
	//close GL context and any other GLFW resources
	glfwTerminate();
}

void initSkybox() {
	std::vector<const GLchar*> faces;
	faces.push_back("skybox/right.tga");
	faces.push_back("skybox/left.tga");
	faces.push_back("skybox/top.tga");
	faces.push_back("skybox/bottom.tga");
	faces.push_back("skybox/back.tga");
	faces.push_back("skybox/front.tga");
	skyBox.Load(faces);
}

int main(int argc, const char* argv[]) {

	if (!initOpenGLWindow()) {
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CCW); // GL_CCW for counter clock-wise

	skyBoxShader.loadShader("shaders/skyboxShader.vert", "shaders/skyboxShader.frag");
	initSkybox();
	myCustomShader.loadShader("shaders/shaderStart.vert", "shaders/shaderStart.frag");
	myCustomShader.useShaderProgram();





	//initialize the projection matrix
	projection = glm::perspective(glm::radians(55.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
	//send matrix data to shader
	projLoc = glGetUniformLocation(myCustomShader.shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));



	
	//texture = ReadTextureFromFile("textures/hazard2.png");

	myModel1.LoadModel("objects/minge.obj", "textures/");
	myModel2.LoadModel("objects/drona.obj", "textures/");
	myModel3.LoadModel("objects/drona2.obj", "textures/");

	myModel.LoadModel("objects/scena2.obj", "textures/");
	presentationStartTime = glfwGetTime();




	while (!glfwWindowShouldClose(glWindow)) {
		processMovement();
		float currentTime = glfwGetTime();
		float deltaTime = currentTime - lastTimeStamp;
		lastTimeStamp = currentTime;


		renderScene();

		glDepthFunc(GL_LEQUAL);
		skyBoxShader.useShaderProgram();

		glm::mat4 skyboxView = glm::mat4(glm::mat3(myCamera.getViewMatrix())); 
		GLint skyboxViewLoc = glGetUniformLocation(skyBoxShader.shaderProgram, "view");
	    glUniformMatrix4fv(skyboxViewLoc, 1, GL_FALSE, glm::value_ptr(skyboxView));

	    GLint skyboxProjLoc = glGetUniformLocation(skyBoxShader.shaderProgram, "projection");
		glUniformMatrix4fv(skyboxProjLoc, 1, GL_FALSE, glm::value_ptr(projection));

		skyBox.Draw(skyBoxShader, myCamera.getViewMatrix(), projection);

		glDepthFunc(GL_LESS);

		glfwPollEvents();
		glfwSwapBuffers(glWindow);
	}

	cleanup();

	return 0;
}
