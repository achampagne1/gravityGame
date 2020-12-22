//includes
#include "convertToFloat.h"
#include "model.h"
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
void createModel(std::string modelPath, int x, int y,float gravity, int locked);

//object declerations
GLFWwindow* window;
std::unique_ptr<GravityEngine> gravityEngine{ new GravityEngine };


//variables
std::vector<std::shared_ptr<Model>> models;

static double limitFPS = 1.0 / 30.0;
double lastTime = glfwGetTime(), timer = lastTime;
double deltaTime = 0, nowTime = 0;
int frames = 0, updates = 0;
float pos[2] = { 200,300 };
float pos2[2] = { 200,100 };
float pos3[2] = { 200,100 };

int main(void)
{
    initWindow();
    onStartUp();
    createModel("models/circleRes20Rad10.json", pos[0], pos[1], 1, 0);
    createModel("models/circleRes40Rad100.json",pos2[0],pos2[1],100, 1); //optimize to see if res40 is too much
    //createModel("models/circleRes40Rad100.json",pos3[0], pos3[1], 100, 1); //optimize to see if res40 is too much
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

void createModel(std::string modelPath,int x,int y,float gravity, int locked) {
    int windowSize[2] = { 640,480 };
    float coor[2] = { x,y };
    float velocity[2] = { 0,0 };
    std::shared_ptr<Model> model{ new Model(modelPath.c_str(),windowSize,coor,velocity,gravity,locked) };
    models.push_back(model);
}

void render() {
    for(int i=0; i<models.size();i++)
        models.at(i)->render();
}

void update() {
    
    std::vector<std::shared_ptr<Model>> references;
    references.push_back(models.at(1));
    //references.push_back(models.at(2));
    models.at(0)->calculateGravity(references);
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