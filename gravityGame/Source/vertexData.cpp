#include "vertexData.h"

VertexData::VertexData() {

}

VertexData::VertexData(const VertexData& data) {
    VAO = data.VAO;
    VBO = data.VBO;
    EBO = data.EBO;
    width = data.width;
    height = data.height;
    trans = data.trans;
}

void VertexData::generateObject(const char* modelPath, int width, int height, float gravity, int locked) {
    using json = nlohmann::json;
    this->width = width;
    this->height = height;
    this->gravity = gravity;
    std::unique_ptr<ConvertToFloat> conversion{ new ConvertToFloat(width, height) };
    std::unique_ptr<LoadFile> file{ new LoadFile() };
    std::string jsonString;
    jsonString = file->load(modelPath).str();
    json jf = json::parse(jsonString);
    indicesSize = jf["indices"].size();
    verticesSize = jf["vertices"].size();
    vertices = new float[verticesSize * 8];
    indices = new int[indicesSize];
    std::cout << verticesSize << " " << verticesSize << std::endl;
    for (int i = 0; i < verticesSize; i++)
        vertices[i] = jf["vertices"][i];
    for (int i = 0; i < indicesSize; i++)
        indices[i] = jf["indices"][i];

    computeAverage(vertices, verticesSize / 8);
    conversion->format(vertices, verticesSize);
    //binds id
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);


    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesSize * 8 * sizeof(float), vertices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize * 4, indices, GL_STATIC_DRAW);
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
    glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void VertexData::move(float x, float y) {
    this->x = x;
    this->y = y;
    float coor[2] = { x,y };
    xAvgGlobal = x + xAvgModel;
    yAvgGlobal = y + yAvgModel;
    std::unique_ptr<ConvertToFloat> conversion{ new ConvertToFloat(width,height) };
    conversion->convertToGlobal(coor);
    glm::mat4 temp = glm::mat4(1.0f);
    temp = glm::translate(temp, glm::vec3(coor[0], coor[1], 0.0f));
    trans = temp;
}

int VertexData::getX() {
    return x;
}

int VertexData::getY() {
    return y;
}

float VertexData::getAvgX() {
    return xAvgGlobal;
}

float VertexData::getAvgY() {
    return yAvgGlobal;
}

float VertexData::getGravity() {
    return gravity;
}

void VertexData::rotate(glm::vec2 direction) {
    rotation = atan(direction[1] / direction[0]);
}

void VertexData::computeAverage(float model[], int size) {
    for (int i = 0; i < size; i++) {
        xAvgModel += model[i * 8];
        yAvgModel += model[i * 8 + 1];
    }
    xAvgModel /= size;
    yAvgModel /= size;
}

void VertexData::destroy() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}