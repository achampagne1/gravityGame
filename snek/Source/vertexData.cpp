#include "vertexData.h"

VertexData::VertexData(const char* modelPath) {
    std::ifstream modelFile;
    std::stringstream modelStream;
    std::string modelData;
    modelFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        modelFile.open(modelPath);
        modelStream << modelFile.rdbuf();
        modelData = modelStream.str();
        // close file handlers
        modelFile.close();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::MODEL::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    std::string substr;
    std::getline(modelStream, substr, ','); 
    int numVertices = stoi(substr);
    float* vertices = new float[numVertices*6];
    std::getline(modelStream, substr, '\n');
    int numIndices = stoi(substr);
    int* indices = new int[numIndices];
    int step = 0;
    for (int i = 0; i < numVertices * 6; i++) {
        if(step!=5)
            std::getline(modelStream, substr, ',');
        else
            std::getline(modelStream, substr, '\n');
        vertices[i] = stof(substr);
        if (step == 5)
            step = 0;
        else
            step++;
    }
    step = 0;
    for (int i = 0; i < numIndices; i++) {
        if (step != 2)
            std::getline(modelStream, substr, ',');
        else
            std::getline(modelStream, substr, '\n');
        indices[i] = stoi(substr);
        if (step == 2)
            step = 0;
        else
            step++;
    }
    
    //binds id
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);


    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, numVertices*6*4, vertices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices*4, indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void VertexData::render() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void VertexData::destroy() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}