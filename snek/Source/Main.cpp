#define STB_IMAGE_IMPLEMENTATION
//includes
#include "shaderLoader.h"
#include "stb_image.h"
#include "vertexData.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//function prototypes
static void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void initWindow();
void destroy();

//temp
float vertices[] = { 
    0.5f, -0.5f, 0.0f,  0.5f, 0.5f, 0.5f,   // bottom right
    -0.5f, -0.5f, 0.0f,  0.25f, 0.25f, 0.25f,  // bottom left
    -0.5f,  0.5f, 0.0f,  0.5f, 0.5f, 0.5f,  // top left
    0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f,   // top right 
};

float indices[] = {
    0, 1, 2,   // first triangle
    2, 3, 0 
};

//object declerations
GLFWwindow* window;

int main(void)
{
    initWindow();
    std::unique_ptr<VertexData> square{ new VertexData(vertices,sizeof(vertices),indices,sizeof(indices))};
    std::unique_ptr <Shader> shader{ new Shader("Shaders/3.3.shader.vs", "Shaders/3.3.shader.fs")};
 
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.5f, 1.0f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shader->use();
        square->render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    destroy();
}

void destroy() {
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void initWindow() {
    if (!glfwInit())
        exit(EXIT_FAILURE);

    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress); //important
    glfwSwapInterval(1);

    glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}