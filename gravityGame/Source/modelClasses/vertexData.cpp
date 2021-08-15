#include "vertexData.h"

VertexData::VertexData() {

}

VertexData::VertexData(const VertexData& vertexData) {
    shader = std::make_unique<Shader>(*vertexData.shader);
    VAO = vertexData.VAO;
    width = vertexData.width;
    height = vertexData.height;
    rotation = vertexData.rotation; 
    avg = vertexData.avg;
    avgModel = vertexData.avgModel;
    gravity = vertexData.gravity;
    trans = vertexData.trans;
    indicesSizeCollision = vertexData.indicesSizeCollision;
    verticesSizeCollision = vertexData.verticesSizeCollision;
    textureVertices = vertexData.textureVertices;
    textureIndices = vertexData.textureIndices;
    collisionVertices = vertexData.collisionVertices;
    collisionVerticesUpdated = vertexData.collisionVerticesUpdated;
    collisionIndices = vertexData.collisionIndices;
}

std::vector<std::shared_ptr<AnimationData>> VertexData::generateObject(const char* modelPath, int width, int height) {
    using json = nlohmann::json;
    this->width = width;
    this->height = height;
    int nrChannels;
    std::unique_ptr<ConvertToFloat> conversion{ new ConvertToFloat(width, height) };
    std::unique_ptr<LoadFile> file{ new LoadFile() };
    std::string jsonString;
    jsonString = file->load(modelPath).str();
    json jf = json::parse(jsonString);

    for (int i = 0; i < jf["textureVertices"].size(); i++) 
        textureVertices.push_back(jf["textureVertices"][i]);

    for (int i = 0; i < jf["textureIndices"].size(); i++)  // responsible for just the texture indices
        textureIndices.push_back(jf["textureIndices"][i]);

    for (int i = 0; i < jf["collisionVertices"].size(); i++) {
        collisionVertices.push_back(jf["collisionVertices"][i]);
        collisionVerticesUpdated = collisionVertices;
    }

    for (int i = 0; i < jf["collisionIndices"].size(); i++)
        collisionIndices.push_back(jf["collisionIndices"][i]);

    computeAverage(collisionVertices);
    textureVertices = conversion->format(textureVertices);
    collisionVertices = conversion->format(collisionVertices);

    this->gravity = jf["gravity"];

    //binds id
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, textureVertices.size() * 8 * sizeof(float), &textureVertices.front(), GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, textureIndices.size() * 4, &textureIndices.front(), GL_STATIC_DRAW);
    //animatons
    std::vector<std::shared_ptr<AnimationData>> animationDataVec;
    for (int i = 0; i < jf["animations"].size(); i++) {
        std::shared_ptr<AnimationData> temp{ new AnimationData };
        int framesSize = 0;
        temp->setType(jf["animations"][i]["type"]);
        framesSize = jf["animations"][i]["frames"].size();
        temp->setFramesSize(framesSize);
        for (int j = 0; j < framesSize; j++) {
            unsigned int tempId = 0;
            glGenTextures(1, &tempId);
            glBindTexture(GL_TEXTURE_2D, tempId);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
            glEnableVertexAttribArray(2);
            std::string texturePathString = to_string(jf["animations"][i]["frames"][j]);
            texturePathString.erase(0, 1);
            texturePathString.erase(texturePathString.size() - 1);
            stbi_set_flip_vertically_on_load(true);
            unsigned char* data = stbi_load(texturePathString.c_str(), &width, &height, &nrChannels, 0);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(data);
            temp->setFrame(j, tempId);
        }
        for (int j = 0; j < jf["animations"][i]["order"].size(); j++)
            temp->setOrder(jf["animations"][i]["order"][j]);
        animationDataVec.push_back(temp);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return animationDataVec;
}

void VertexData::render(int animationFrame) {
    shader->use();
    unsigned int transformLoc = glGetUniformLocation(shader->ID, "location");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
    glBindTexture(GL_TEXTURE_2D, animationFrame);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, textureIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void VertexData::move(glm::vec2 input) {
    avg = input + avgModel;
    std::unique_ptr<ConvertToFloat> conversion{ new ConvertToFloat(width,height) };
    conversion->convertToGlobal(&input);
    moveVertices(input);
    glm::mat4 temp = glm::mat4(1.0f);
    //temp = glm::rotate(temp, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
    temp = glm::translate(temp, glm::vec3(input.x, input.y, 0.0f));
    trans = temp;
}

glm::vec2 VertexData::getAvg() {
    return avg;
}

glm::vec2 VertexData::getAvgModel() {
    return avgModel;
}

float VertexData::getGravity() {
    return gravity;
}

int VertexData::getTexturesSize() {
    return currentAnimationType->getFramesSize();
}

std::shared_ptr<AnimationData> VertexData::getCurrentAnimation() {
    return currentAnimationType;
}

void VertexData::rotate(glm::vec2 direction) {
    rotation = atan(-direction[0] / direction[1]);
}

void VertexData::computeAverage(std::vector<float>input) { 
    for (int i = 0; i < input.size() / 8; i++) {
        avgModel.x += input[i * 8];
        avgModel.y += input[i * 8 + 1];
    }
    avgModel.x /= input.size() / 8;
    avgModel.y /= input.size() / 8;
}

void VertexData::moveVertices(glm::vec2 coordinates) {
    for (int i = 0; i < collisionVertices.size() / 8; i++) {
        collisionVerticesUpdated[i * 8] = collisionVertices[i * 8] + coordinates.x;
        collisionVerticesUpdated[i * 8 + 1] = collisionVertices[i * 8 + 1] + coordinates.y;
    }
}

void VertexData::mirrorSprite() {
    for (int i = 0; i <= 3; i++) {
        textureVertices[i * 8 + 6] = abs(1 - textureVertices[i * 8 + 6]);
    }
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, textureVertices.size() * 8 * sizeof(float), &textureVertices.front(), GL_STATIC_DRAW);
}

VertexData::~VertexData() {
    //glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);
}