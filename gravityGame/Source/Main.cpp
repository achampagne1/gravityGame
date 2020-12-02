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
glm::vec2 calculateDirection(std::shared_ptr<VertexData> model, std::vector<std::shared_ptr<VertexData>> references);
void createModel(std::string modelPath, int x, int y, int locked);
int roundUp(int numToRound, int multiple);
int roundDown(int numToRound, int multiple);

//object declerations
GLFWwindow* window;

//variables
static double limitFPS = 1.0 / 25.0;
double lastTime = glfwGetTime(), timer = lastTime;
double deltaTime = 0, nowTime = 0;
int frames = 0, updates = 0;
std::vector<std::shared_ptr<VertexData>> models;
int pos[2] = { 325,125 };
float velocity[2] = { -5,0 }; //units per frame
float gravity = 2; //units per frame per frame

int main(void)
{
    initWindow();
    onStartUp();
    createModel("models/circleRes20Rad10.json",300,300, 1); //optimize to see if res40 is too much
    createModel("models/circleRes20Rad10.json", 350, 50, 1); //optimize to see if res40 is too much
    createModel("models/square.json", pos[0], pos[1], 0);
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

glm::vec2 calculateDirection(std::shared_ptr<VertexData> model, std::vector<std::shared_ptr<VertexData>> references) {
    float xComponant = 0;
    float yComponant = 0;
    for (int i = 0; i < references.size(); i++) {
        xComponant += references.at(i)->getAvgX() - model->getAvgX();
        yComponant += references.at(i)->getAvgY() - model->getAvgY();
    }
    xComponant /= references.size();
    yComponant /= references.size();
    if (xComponant == 0 && yComponant == 0)
        return glm::vec2(xComponant, yComponant);
    else
        return glm::normalize(glm::vec2(xComponant, yComponant));
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
    std::vector<std::shared_ptr<VertexData>> references;
    references.push_back(models.at(0));
    references.push_back(models.at(1));
    glm::vec2 directionOfAcceleration = calculateDirection(models.at(2), references);
    float deltaVelocity[2] = { directionOfAcceleration[0] * gravity, directionOfAcceleration[1] * gravity };
    velocity[0] += deltaVelocity[0];
    velocity[1] += deltaVelocity[1];
    pos[0] += velocity[0];
    pos[1] += velocity[1];
    models.at(2)->move(pos[0], pos[1]);
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