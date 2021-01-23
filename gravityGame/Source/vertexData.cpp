#include "vertexData.h"

//TODO: seperate the collison box from the texture models

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
    int nrChannels;
    std::unique_ptr<ConvertToFloat> conversion{ new ConvertToFloat(width, height) };
    std::unique_ptr<LoadFile> file{ new LoadFile() };
    std::string jsonString;
    jsonString = file->load(modelPath).str();
    json jf = json::parse(jsonString);
    indicesSize = jf["indices"].size();
    verticesSize = jf["vertices"].size();
    vertices = new float[verticesSize * 8];
    verticesUpdated = new float[verticesSize * 8];
    indices = new int[indicesSize];
    for (int i = 0; i < verticesSize; i++) {
        vertices[i] = jf["vertices"][i];
        verticesUpdated[i] = vertices[i];
    }
    for (int i = 0; i < indicesSize; i++)
        indices[i] = jf["indices"][i];

    computeAverage(vertices, verticesSize / 8);
    conversion->format(vertices, verticesSize);

    std::string texturePathString = to_string(jf["texturePath"]);
    texturePathString.erase(0, 1);
    texturePathString.erase(texturePathString.size() - 1);
    //binds id
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);
    glGenTextures(1, &texture);

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
    glBindTexture(GL_TEXTURE_2D, texture);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(texturePathString.c_str(), &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void VertexData::render() {
    shader->use();
    unsigned int transformLoc = glGetUniformLocation(shader->ID, "location");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

    glBindTexture(GL_TEXTURE_2D, texture);
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
    moveVertices(coor[0], coor[1]);
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

void VertexData::moveVertices(float x, float y) {
    for (int i = 0; i < verticesSize / 8; i++) {
        verticesUpdated[i*8] = vertices[i*8] + x;
        verticesUpdated[i*8 + 1] = vertices[i*8 + 1] + y;
    }
}

void VertexData::destroy() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}