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
void update();
void render();
void onStartUp();
void createModel(std::string modelPath, int x, int y);
int roundUp(int numToRound, int multiple);
int roundDown(int numToRound, int multiple);

//object declerations
GLFWwindow* window;

//variables
static double limitFPS = 1.0 / 15.0;
double lastTime = glfwGetTime(), timer = lastTime;
double deltaTime = 0, nowTime = 0;
int frames = 0, updates = 0;
std::vector<std::shared_ptr<VertexData>> models;

int main(void)
{
    initWindow();
    onStartUp();
    createModel("models/circleRes40Rad100.json",220,140, 1); //optimize to see if res40 is too much
    createModel("models/square.json", 310, 400, 0);
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
}

void createModel(std::string modelPath,int x,int y, int locked) {
    std::shared_ptr<VertexData> model{ new VertexData(modelPath.c_str(),640,480,locked) };
    model->move(x, y);
    models.push_back(model);
}

void render() {
    for(int i=0; i<models.size();i++)
        models.at(i)->render();
}

void update() {

}

void initWindow() {
    if (!glfwInit())
        exit(EXIT_FAILURE);

    window = glfwCreateWindow(640, 480, "Gravity Game", NULL, NULL);

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
        
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS){
       
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS){
        
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS){
        
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