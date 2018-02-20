#include "../include/globj.hpp"


globj::globj(std::string configPath, std::string verticesPath, std::string indicesPath)
{
    std::ifstream confFile;
    std::ifstream verticesFile;
    std::ifstream indexFile;

    confFile.open(configPath);
    verticesFile.open(verticesPath);
    indexFile.open(indicesPath);

    confFile >> std::hex >> vboType >> drawType >> eboType >> dataType >> normalize >> drawShape >> indexType >> std::dec >> step >> dataSize >> indexSize >> attrNum;
    // std::dec >> dataIndex >> stepSize >> dataSize >> indexSize;
    data = new float[dataSize];
    index = new int[indexSize];
    aLocation = new int[attrNum];
    aSize = new int[attrNum];
    aOffset = new int[attrNum];

    for (int i = 0; i < attrNum; i++)
        confFile >> aLocation[i] >> aSize[i] >> aOffset[i];

    for (int i = 0; i < dataSize; i++)
            verticesFile >> data[i];

    for (int i = 0; i < indexSize; i++)
        indexFile >> index[i];

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(vboType, VBO);
    glBufferData(vboType, dataSize * sizeof(float), data, drawType);

    glBindBuffer(eboType, EBO);
    glBufferData(eboType, indexSize * sizeof(int), index, drawType);

    for (int i = 0; i < attrNum; i++)
    {
        glVertexAttribPointer(aLocation[i], aSize[i], dataType, normalize, step * sizeof(float), (void *)(aOffset[i]* sizeof(float)));
        glEnableVertexAttribArray(i);
    }

    glBindBuffer(vboType, 0);
    glBindVertexArray(0); 
}

void globj::draw()
{
    glBindVertexArray(VAO);
    glDrawElements(drawShape, indexSize, indexType, 0);
}

globj::~globj()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}