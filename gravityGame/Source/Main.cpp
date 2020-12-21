//includes
#include "convertToFloat.h"
#include "gravityEngine.h"
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
static double limitFPS = 1.0 / 30.0;
double lastTime = glfwGetTime(), timer = lastTime;
double deltaTime = 0, nowTime = 0;
int frames = 0, updates = 0;
std::vector<std::shared_ptr<VertexData>> models;
float pos[2] = { 200.0,275.0 };
float velocity[2] = { 1,0 }; //units per frame

int main(void)
{
    initWindow();
    onStartUp();
    createModel("models/circleRes20Rad10.json", pos[0], pos[1], 1, 0);
    createModel("models/circleRes20Rad10.json",300,200,100, 1); //optimize to see if res40 is too much
    createModel("models/circleRes20Rad10.json", 200, 300, 100, 1); //optimize to see if res40 is too much
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
    std::shared_ptr<VertexData> model{ new VertexData(modelPath.c_str(),640,480,gravity,locked) };
    model->move(x, y);
    models.push_back(model);
}

void render() {
    for(int i=0; i<models.size();i++)
        models.at(i)->render();
}

void update() {
    
    std::vector<std::shared_ptr<VertexData>> references;
    references.push_back(models.at(1));
    //references.push_back(models.at(2));
    glm::vec2 deltaVelocity = gravityEngine->getDeltaVelocity(models.at(0), references);
    velocity[0] += deltaVelocity[0];
    velocity[1] += deltaVelocity[1];
    pos[0] += velocity[0];
    pos[1] += velocity[1];
    models.at(0)->move(pos[0], pos[1]);
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