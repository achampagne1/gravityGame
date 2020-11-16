#define STB_IMAGE_IMPLEMENTATION
//includes
#include "stb_image.h"
#include "convertToFloat.h"
#include "shaderLoader.h"
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
void mergeArray(float* input, int size);

//object declerations
GLFWwindow* window;

//temp
float data[12] = {
    // positions
    20.0, 20.0, 0.0,  // bottom right
    40.0, 40.0, 0.0, // bottom left
    40.0,  20.0, 0.0, // top left
    20.0, 40.0, 0.0,  // top right
};
float colors[12] = {
    0.0, 0.0, 0.0,
    0.0, 0.0, 0.0,
    0.0, 0.0, 0.0,
    0.0, 0.0, 0.0
};
unsigned int indices[6] = {  // note that we start from 0!
    0, 2, 3,   // first triangle
    1, 2, 3
};

float vertices[24];


int main(void)
{
    initWindow();
    std::unique_ptr<ConvertToFloat> convert{ new ConvertToFloat(640,480) };
    std::unique_ptr <Shader> shader{ new Shader("Shaders/3.3.shader.vs", "Shaders/3.3.shader.fs") };

    convert->convertToGlobal(data, sizeof(data) / sizeof(data[0]));
    convert->convertFromRGB(colors, sizeof(colors) / sizeof(colors[0]));

    mergeArray(vertices, 24);

    std::unique_ptr <VertexData> square{ new VertexData(vertices,sizeof(vertices),indices,sizeof(indices)) };

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shader->use();
        square->render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    destroy();
    //square->destroy();
}

void mergeArray(float* input, int size) {
    int dataCount = 0;
    int colorCount = 0;
    for (int i = 0; i < size; i++) {
        int place = i % 6;
        if (place < 3) {
            input[i] = data[dataCount];
            dataCount++;
        }
        else {
            input[i] = colors[colorCount];
            colorCount++;
        }
    }
}

void initWindow() {
    if (!glfwInit())
        exit(EXIT_FAILURE);

    window = glfwCreateWindow(640, 480, "Snek", NULL, NULL);

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

void destroy() {
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}