//includes
#include "convertToFloat.h"
#include "vertexData.h"
#include <iostream>
#include <vector> 
#include <time.h>

//function prototypes
static void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void initWindow();
void destroy();
void changeLocation();
void update();
void render();
void loadModels();

//object declerations
GLFWwindow* window;

//variables
int x = 200;
int y = 200;
int appleLoc[2];
int direction = 0;
int stepSize = 5;
bool start = false;
static double limitFPS = 1.0 / 30.0;
double lastTime = glfwGetTime(), timer = lastTime;
double deltaTime = 0, nowTime = 0;
int frames = 0, updates = 0;
std::vector<std::shared_ptr<VertexData>> models;

int main(void)
{
    initWindow();
    loadModels();
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        nowTime = glfwGetTime();
        deltaTime += (nowTime - lastTime) / limitFPS;
        lastTime = nowTime;
        while (deltaTime >= 1.0) {
            update();
            updates++;
            deltaTime--;
        }
        render();
        frames++;

        if (glfwGetTime() - timer > 1.0) {
            timer++;
            updates = 0, frames = 0;
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    destroy();
}

void loadModels() {
    std::shared_ptr<VertexData> square{ new VertexData("models/snakeHead.md",640,480) };
    std::shared_ptr<VertexData> apple{ new VertexData("models/snakeHead.md",640,480) };
    models.push_back(square);
    models.push_back(apple);
}

void changeLocation() {
    switch (direction) {
        case(0):
            y += stepSize;
            break;
        case(1):
            x += stepSize;
            break;
        case(2):
            y -= stepSize;
            break;
        case(3):
            x -= stepSize;
            break;
    }
}

void render() {
    for(int i=0; i<models.size();i++)
        models.at(i)->render();
}

void update() {
    if (start)
        changeLocation();
    models.at(0)->move(x, y);
    appleLoc[0] = rand() % 620;
    appleLoc[1] = rand() % 460;
    std::cout << appleLoc[1] << std::endl;
    models.at(1)->move(appleLoc[0], appleLoc[1]);
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
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        direction = 0;
        start = true;
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS){
        direction = 2;
        start = true;
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS){
        direction = 3;
        start = true;
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS){
        direction = 1;
        start = true;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void destroy() {
    for (int i = 0; i < models.size(); i++)
        models.at(i)->destroy();
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}