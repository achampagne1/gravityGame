#include "vertexData.h"

VertexData::VertexData(const char* modelPath, int width, int height) {
    this->width = width;
    this->height = height;
    std::unique_ptr<ConvertToFloat> conversion{ new ConvertToFloat(width, height) };
    std::unique_ptr<LoadFile> file{ new LoadFile() };
    std::stringstream modelStream;
    std::string substr;
    modelStream = file->load(modelPath);
    std::getline(modelStream, substr, ','); 
    int numVertices = stoi(substr);
    float* vertices = new float[numVertices*8];
    std::getline(modelStream, substr, '\n');
    int numIndices = stoi(substr);
    int* indices = new int[numIndices];
    int step = 0;
    for (int i = 0; i < numVertices * 8; i++) {
        if(step!=7)
            std::getline(modelStream, substr, ',');
        else
            std::getline(modelStream, substr, '\n');
        vertices[i] = stof(substr);
        if (step == 7)
            step = 0;
        else
            step++;
    }
    step = 0;
    for (int i = 0; i < numIndices; i++) {
        if (step == 2) {
            std::getline(modelStream, substr, '\n');
            step = 0;
        }
        else {
            std::getline(modelStream, substr, ',');
            step++;
        }
        indices[i] = stoi(substr);
    }
    
    conversion->format(vertices, numVertices * 8 * sizeof(float));
    //binds id
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);


    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, numVertices*8*sizeof(float), vertices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices*4, indices, GL_STATIC_DRAW);
    //texture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void VertexData::render() {
    shader->use();
    unsigned int transformLoc = glGetUniformLocation(shader->ID, "location");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));


    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void VertexData::move(int x, int y) {
    float coor[2] = { float(x),float(y) };
    std::unique_ptr<ConvertToFloat> conversion{ new ConvertToFloat(width,height) };
    conversion->convertToGlobal(coor);
    glm::mat4 temp = glm::mat4(1.0f);
    temp = glm::translate(temp, glm::vec3(coor[0],coor[1], 0.0f));
    trans = temp;
}

void VertexData::rotate(int deg) {

}

void VertexData::destroy() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}