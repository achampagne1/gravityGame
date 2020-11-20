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
void onStartUp();
int roundUp(int numToRound, int multiple);
int roundDown(int numToRound, int multiple);

//object declerations
GLFWwindow* window;

//variables
int x = 200;
int y = 200;
int appleLoc[2] = { x,y };
int direction = 0;
int stepSize = 20;
bool start = false;
static double limitFPS = 1.0 / 15.0;
double lastTime = glfwGetTime(), timer = lastTime;
double deltaTime = 0, nowTime = 0;
int frames = 0, updates = 0;
std::vector<std::shared_ptr<VertexData>> models;

int main(void)
{
    initWindow();
    loadModels();
    onStartUp();
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

void onStartUp() {
    srand(time(0));
    appleLoc[0] = roundUp(rand() % 620,20);
    appleLoc[1] = roundUp(rand() % 460,20);
    models.at(1)->move(appleLoc[0],appleLoc[1]);
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
            if(y<460)
                y += stepSize;
            break;
        case(1):
            if (x < 620)
                x += stepSize;
            break;
        case(2):
            if (y > 0)
                y -= stepSize;
            break;
        case(3):
            if (x > 0)
                x -= stepSize;
            break;
    }
    std::cout << x << " " << y << std::endl;
    std::cout << appleLoc[0] << " " << appleLoc[1] << std::endl;
}

void render() {
    for(int i=0; i<models.size();i++)
        models.at(i)->render();
}

void update() {
    if (start)
        changeLocation();
    models.at(0)->move(x, y);
    if (x == appleLoc[0] && y == appleLoc[1]) {
        appleLoc[0] = roundUp(rand() % 620, 20);
        appleLoc[1] = roundUp(rand() % 460, 20);
        models.at(1)->move(roundUp(appleLoc[0], 20), roundUp(appleLoc[1], 20));
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

int roundUp(int numToRound, int multiple)
{
    if (multiple == 0)
        return numToRound;

    int remainder = numToRound % multiple;
    if (remainder == 0)
        return numToRound;

    return numToRound + multiple - remainder;
}

int roundDown(int numToRound, int multiple)
{
    if (multiple == 0)
        return numToRound;

    int remainderInverseSorta = multiple-(numToRound % multiple);
    if (remainderInverseSorta == 0)
        return numToRound;

    return numToRound - multiple + remainderInverseSorta;
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