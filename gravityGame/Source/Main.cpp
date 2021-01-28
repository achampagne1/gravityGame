//includes
#define STB_IMAGE_IMPLEMENTATION //needed for image loading 
#include "convertToFloat.h"
#include "model.h"
#include <iostream>
#include <vector> 
#include "delay.h"


//function prototypes
static void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void initWindow();
void destroy();
void update();
void render();
void onStartUp();
void createModel(std::string modelPath, int x, int y,float v[2], float gravity, int locked);

//object declerations
GLFWwindow* window;
std::shared_ptr<Delay> delay;


//variables
std::vector<std::shared_ptr<Model>> models;
std::shared_ptr<MovementEngine> movementEnginePtr;

static double limitFPS = 1.0 / 30.0;
double lastTime = glfwGetTime(), timer = lastTime;
double deltaTime = 0, nowTime = 0;
int frames = 0, updates = 0;
float velocit[2] = { 0,0 };
float velocit2[2] = { 0,0 };
float velocit3[2] = { 0,0 };
float velocit4[2] = { 0,0 };
float pos[2] = {380,220 };
float pos2[2] = { -300,0 };
float pos3[2] = { 280,300 };
bool run = false;

int main(void)
{
    initWindow();
    onStartUp();
    createModel("models/sky.json", 0, 0, velocit4, 1, 0);
    createModel("models/spaceman.json", pos[0], pos[1],velocit, 1, 0);
    createModel("models/spacewoman.json", pos3[0], pos3[1], velocit3, 1, 0);
    createModel("models/planet1.json",pos2[0],pos2[1],velocit2,10000, 1); //optimize to see if res40 is too much
    movementEnginePtr = models.at(1)->getMovementPointer(); //gets movement pointer from the player which is the first object in the models vector
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

void createModel(std::string modelPath,int x,int y,float v[2], float gravity, int locked) {
    int windowSize[2] = { 800,460 };
    float coor[2] = { x,y };
    std::shared_ptr<Model> model{ new Model(modelPath.c_str(),windowSize,coor,v,gravity,locked) };
    models.push_back(model);
}

void render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    for(int i=0; i<models.size();i++)
        models.at(i)->render();
}

void update() {
    if (run) {
        std::vector<std::shared_ptr<Model>> references;
        references.push_back(models.at(3));
        float* newOffset = models.at(1)->calculateVelocity(references); //janky way of doing it, but this is for transfering the data in that array to a new array so the original array does not get modified
        float newOffsetTemp[2] = { newOffset[0],newOffset[1] };
        newOffsetTemp[0] = -newOffset[0];
        newOffsetTemp[1] = -newOffset[1];

        for (int i = 2; i < models.size(); i++) {
            models.at(i)->rotate(glm::vec2(0, 1));
            models.at(i)->move(newOffsetTemp);
            //things need to be rotated as well
        }

        newOffset = models.at(2)->calculateVelocity(references);
        models.at(2)->move(newOffset);
    };
}

void initWindow() {
    if (!glfwInit())
        exit(EXIT_FAILURE);

    window = glfwCreateWindow(800, 450, "Gravity Game", NULL, NULL);

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
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        if (run)
            run = false;
        else
            run = true;
    }
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS){
   
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS){
        movementEnginePtr->setDirection(1);
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS){
        movementEnginePtr->setDirection(3);
    }
    if (action == GLFW_RELEASE) {
        movementEnginePtr->setDirection(4);
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