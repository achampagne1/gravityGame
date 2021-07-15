#include "vertexData.h"

VertexData::VertexData() {

}

VertexData::VertexData(const VertexData& vertexData) {
    shader = std::make_unique<Shader>(*vertexData.shader);
    VAO = vertexData.VAO;
    width = vertexData.width;
    height = vertexData.height;
    x = vertexData.x;
    y = vertexData.y;
    rotation = vertexData.rotation;
    xAvgModel = vertexData.xAvgModel;
    yAvgModel = vertexData.yAvgModel;
    xAvgGlobal = vertexData.xAvgGlobal;
    yAvgGlobal = vertexData.yAvgGlobal;
    gravity = vertexData.gravity;
    trans = vertexData.trans;
    indicesSizeTexture = vertexData.indicesSizeTexture;
    verticesSizeTexture = vertexData.verticesSizeTexture;
    indicesSizeCollision = vertexData.indicesSizeCollision;
    verticesSizeCollision = vertexData.verticesSizeCollision;
    verticesCollision = new float[verticesSizeCollision * 8];
    verticesCollisionUpdated = new float[verticesSizeCollision * 8];
    indicesCollision = new int[indicesSizeCollision];
    for (int i = 0; i < verticesSizeCollision * 8; i++) {
        verticesCollision[i]= vertexData.verticesCollision[i];
        verticesCollisionUpdated[i] = vertexData.verticesCollisionUpdated[i];
    }

    for (int i = 0; i < indicesSizeCollision; i++)
        indicesCollision[i] = vertexData.indicesCollision[i];
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
    
    indicesSizeTexture = jf["textureIndices"].size();
    verticesSizeTexture = jf["textureVertices"].size();
    indicesSizeCollision = jf["collisionIndices"].size();
    verticesSizeCollision = jf["collisionVertices"].size(); 
    verticesTexture = new float[verticesSizeTexture * 8];
    verticesCollision = new float[verticesSizeCollision * 8];
    verticesCollisionUpdated = new float[verticesSizeCollision * 8];
    verticesCollisionUpdated = new float[verticesSizeCollision * 8];
    indicesTexture = new int[indicesSizeTexture];
    indicesCollision = new int[indicesSizeCollision];

    for (int i = 0; i < verticesSizeTexture; i++) {  //responsible for just the texture vertices
        verticesTexture[i] = jf["textureVertices"][i];
    }
    for (int i = 0; i < indicesSizeTexture; i++) { // responsible for just the texture indices
        indicesTexture[i] = jf["textureIndices"][i];
    }
    for (int i = 0; i < verticesSizeCollision; i++) {  //responsible for just the collision vertices
        verticesCollision[i] = jf["collisionVertices"][i];
        verticesCollisionUpdated[i] = verticesCollision[i];
    }
    for (int i = 0; i < indicesSizeCollision; i++) { // responsible for just the collision indices
        indicesCollision[i] = jf["collisionIndices"][i];
    }

    computeAverage(verticesCollision, verticesSizeCollision / 8); //you only need to compute the average for the collision model
    conversion->format(verticesTexture, verticesSizeTexture);
    conversion->format(verticesCollision, verticesSizeCollision);

    this->gravity = jf["gravity"];

    //binds id
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesSizeTexture * 8 * sizeof(float), verticesTexture, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSizeTexture * 4, indicesTexture, GL_STATIC_DRAW);
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
    glDrawElements(GL_TRIANGLES, indicesSizeTexture, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);       
}

void VertexData::move(float x, float y) {
    this->x = x;
    this->y = y;
    float coor[2] = { x,y };
    xAvgGlobal = x+xAvgModel;//might need to change back to xAvgModel and yAvgModel
    yAvgGlobal = y+yAvgModel;
    std::unique_ptr<ConvertToFloat> conversion{ new ConvertToFloat(width,height) };
    conversion->convertToGlobal(coor);
    moveVertices(coor[0], coor[1]);
    glm::mat4 temp = glm::mat4(1.0f);
    //temp = glm::rotate(temp, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
    temp = glm::translate(temp, glm::vec3(coor[0], coor[1], 0.0f));
    trans = temp;
}

float VertexData::getAvgX() {
    return xAvgGlobal;
}

float VertexData::getAvgY() {
    return yAvgGlobal;
}

float VertexData::getAvgXModel() {
    return yAvgModel;
}

float VertexData::getAvgYModel() {
    return xAvgModel;
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

void VertexData::computeAverage(float model[], int size) {
    for (int i = 0; i < size; i++) {
        xAvgModel += model[i * 8];
        yAvgModel += model[i * 8 + 1];
    }
    xAvgModel /= size;
    yAvgModel /= size;
}

void VertexData::moveVertices(float x, float y) {
    for (int i = 0; i < verticesSizeCollision / 8; i++) {
        verticesCollisionUpdated[i*8] = verticesCollision[i*8] + x;
        verticesCollisionUpdated[i*8 + 1] = verticesCollision[i*8 + 1] + y;
    }
}

VertexData::~VertexData() {
    //glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);
}