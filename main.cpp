//aracers

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "robot.h"
#include "wall.h"
#include "cst.h"


#define W1 34
#define W2 30


static const char * vs_source[] =
	{
		
		"#version 410 core										\n"
		"layout (location = 0) in vec3 aPos;					\n"
		"uniform mat4 projection;								\n"
		"uniform mat4 model;									\n"
		"uniform mat4 view;										\n"
		"out vec3 cMod;										\n"

		"void main()											\n"
		"{														\n"
		"	cMod = vec3(1.0);									\n"
		"	if(aPos.x == 0.6) {									\n"
		"		cMod = vec3(0.0);								\n"
		"	}													\n"
		"	gl_Position = projection * model * vec4(aPos, 1.0);	\n"
		"														\n"
		"}                                                  	\n"
	};


static const char * fs_source[] =
	{
		"#version 410 core										\n"
		"out vec4 FragColor;										\n"
		"in vec3 cMod;										\n"
		"uniform vec3 objectColor;										\n"

		"void main()											\n"
		"{														\n"
		"	                                          			\n"
		"	FragColor = vec4(objectColor * cMod, 1.0); 	           	\n"
		"} 														\n"
	};

namespace {
	unsigned int fragmentShader, vertexShader;
	unsigned int robotVAO, robotVBO, wallVAO, wallVBO, bulVAO, bulVBO;
	unsigned int projLoc, modelLoc, colorLoc;
	GLFWwindow* window;
	Robot r1, r2;
	Wall w1[W1], w2[W2];
};

void processInput(GLFWwindow *window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


int main() {
	srand(time(NULL));
	glfwInit();
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	
	window = glfwCreateWindow(WIDTH, HEIGHT, "AspiRacers", NULL, NULL);
	glfwSetKeyCallback(window, key_callback);
	

	glfwMakeContextCurrent(window);
	
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);


	
	float vertices[] = {
        0.000000, 0.000000,
		0.600000, 0.000000,
        0.475528, 0.154509,
        0.404509, 0.293893,
        0.293893, 0.404509,
        0.154508, 0.475528,
        -0.000000, 0.500000,
        -0.154509, 0.475528,
        -0.293893, 0.404509,
        -0.404508, 0.293893,
        -0.475528, 0.154509,
        -0.500000, 0.000000,
        -0.475528, -0.154508,
        -0.404509, -0.293892,
        -0.293893, -0.404508,
        -0.154509, -0.475528,
        -0.000000, -0.500000,
        0.154508, -0.475528,
        0.293893, -0.404509,
        0.404508, -0.293893,
        0.475528, -0.154508,
        0.600000, 0.000000
	};
	
	float b_vertices[] = {
		0.000000, 0.000000,
        0.200000, 0.000000,
		0.190211, 0.061803,
		0.161803, 0.117557,
		0.117557, 0.161803,
		0.061803, 0.190211,
		-0.000000, 0.200000,
		-0.061803, 0.190211,
		-0.117557, 0.161803,
		-0.161803, 0.117557,
		-0.190211, 0.061803,
		-0.200000, 0.000000,
		-0.190211, -0.061803,
		-0.161803, -0.117557,
		-0.117557, -0.161803,
		-0.061804, -0.190211,
		-0.000000, -0.200000,
		0.061803, -0.190211,
		0.117557, -0.161803,
		0.161803, -0.117557,
		0.190211, -0.061803,
		0.200000, 0.000000
	};
    
    //for(float f = 0; f <= 2*M_PI; f += M_PI/10.0) {
        //printf("%f, %f,\n", 0.2*cos(f), 0.2*sin(f));
    
    //}
	
	float wall_v[] = {
		2.0, 1.0,
		1.0, 15.0,
		3.0, 25.0,
		5.0, 28.0,
		7.0, 29.0,
		8.5, 27.0,
		10.0, 24.0,
		12.0, 15.0,
		13.0, 12.0,
		16.0, 11.0,
		18.0, 16.0,
		16.0, 21.0,
		15.0, 24.0,
		16.0, 28.0,
		18.0, 28.0,
		22.0, 27.0,
		24.0, 26.0,
		28.0, 26.0,
		33.0, 28.0,
		43.0, 26.0,
		44.0, 23.0,
		45.0, 18.0,
		44.0, 10.0,
		45.0, 3.0,
		42.0, 2.0,
		35.0, 2.0,
		32.0, 4.0,
		31.0, 9.0,
		30.0, 10.0,
		25.0, 10.0,
		24.0, 9.0,
		21.0, 4.0,
		17.0, 3.0,
		12.0, 1.0,
		2.0, 1.0,
		
		5.0, 5.0,
		12.0, 5.0,
		18.0, 7.0,
		21.0, 10.0,
		24.0, 13.0,
		30.0, 13.0,
		34.0, 12.0,
		36.0, 7.0,
		37.0, 5.0,
		40.0, 5.0,
		41.0, 7.0,
		40.0, 13.0,
		41.0, 18.0,
		39.0, 22.0,
		34.0, 23.0,
		30.0, 21.0,
		24.0, 21.0,
		20.0, 24.0,
		19.0, 23.0,
		21.0, 18.0,
		22.0, 14.0,
		20.0, 10.0,
		15.0, 7.0,
		12.0, 8.0,
		10.0, 10.0,
		8.0, 16.0,
		7.0, 23.0,
		6.5, 25.0,
		6.0, 25.0,
		5.0, 15.0,
		5.0, 5.0,
		


		
		
		
		
	};

	
	glGenBuffers(1, &(robotVBO));
	glGenVertexArrays(1, &(robotVAO)); 
	glGenBuffers(1, &(wallVBO));
	glGenVertexArrays(1, &(wallVAO)); 
	glGenBuffers(1, &(bulVBO));
	glGenVertexArrays(1, &(bulVAO)); 
	
	glBindVertexArray(robotVAO);
	glBindBuffer(GL_ARRAY_BUFFER, robotVBO); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glBindVertexArray(bulVAO);
	glBindBuffer(GL_ARRAY_BUFFER, bulVBO); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(b_vertices), b_vertices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glBindVertexArray(wallVAO);
	glBindBuffer(GL_ARRAY_BUFFER, wallVBO); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(wall_v), wall_v, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	

	int success;
	char infoLog[512];
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, vs_source, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};
		
	
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, fs_source, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	};
	
	unsigned int shaderProgram = glCreateProgram();
	
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	
	glUseProgram(shaderProgram);
	
	glLineWidth(5.0f);
	
	projLoc = glGetUniformLocation(shaderProgram, "projection");
	modelLoc = glGetUniformLocation(shaderProgram, "model");
	colorLoc = glGetUniformLocation(shaderProgram, "objectColor");
	
	r2.init(glm::vec2(3.9f, 2.5f), M_PI/2, glm::vec3(0.3f, 0.0f, 1.0f), "Blue");
	r1.init(glm::vec2(2.5f, 2.5f), M_PI/2, glm::vec3(0.1f, 0.7f, 0.3f), "Green");
	int id = 0;
	for(int i = 0; i < W1; i++) {
		w1[i].init(glm::vec2(wall_v[id*2], wall_v[id*2 + 1]), glm::vec2(wall_v[id*2 + 2], wall_v[id*2 + 3]), id);
		id++;
	}
	id++;
	for(int i = 0; i < W2; i++) {
		w2[i].init(glm::vec2(wall_v[id*2], wall_v[id*2 + 1]), glm::vec2(wall_v[id*2 + 2], wall_v[id*2 + 3]), id);
		id++;
	}

	glm::mat4 projection;
	projection = glm::ortho(0.0f, W_WIDTH, 0.0f, W_HEIGHT, -1.0f, 1.0f);
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	while(!glfwWindowShouldClose(window)) {
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		processInput(window);
		
        

		for(int i = 0; i < W1; i++) {
			w1[i].collide(&r1.c);
			w1[i].collide(&r2.c);
			for(int j = 0; j < BULLETS; j++) {
				w1[i].collide(&r1.bullets[j].c);
				w1[i].collide(&r2.bullets[j].c);
			}
			w1[i].draw();
		}
		
		for(int i = 0; i < W2; i++) {
			w2[i].collide(&r1.c);
			w2[i].collide(&r2.c);
			for(int j = 0; j < BULLETS; j++) {
				w2[i].collide(&r1.bullets[j].c);
				w2[i].collide(&r2.bullets[j].c);
			}
			w2[i].draw();
		}
		
		collide(&r1.c, &r2.c);
		for(int j = 0; j < BULLETS; j++) {
			collide(&r1.c, &r2.bullets[j].c);
			collide(&r2.c, &r1.bullets[j].c);
		}
        r1.walls();
        r2.walls();
		r1.update();
		r2.update();
		r1.draw();
		r2.draw();
		
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	r1.end();
	r2.end();
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader); 
	glfwTerminate();
	printf("finish\n");
	
	
	return 0;
}

void draw_robot(glm::vec2 pos, float angle, glm::vec3 color) {
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(pos, 0.0));
	model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
	glUniform3fv(colorLoc, 1, glm::value_ptr(color));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(robotVAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 50);
}

void draw_bullet(glm::vec2 pos, glm::vec3 color) {
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(pos, 0.0));
	glUniform3fv(colorLoc, 1, glm::value_ptr(color));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(bulVAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 50);
}

void draw_wall(int id) {
	//printf("draw %d\n", id);
	glm::mat4 model;
	model = glm::mat4();
	glUniform3f(colorLoc, 0.0f, 0.0f, 0.0f);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(wallVAO);
	glDrawArrays(GL_LINES, id, 2);
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        r1.move_forward();
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        r1.move_back();
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        r1.move_left();
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        r1.move_right();
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        r2.move_forward();
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        r2.move_back();
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        r2.move_left();
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        r2.move_right();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(window, GL_TRUE); 
	if (key == GLFW_KEY_RIGHT_SHIFT && action == GLFW_PRESS) {
		//printf("fire\n");
		r2.fire(); 
	}
	if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS) {
		//printf("fire\n");
		r1.fire(); 
	}
}



