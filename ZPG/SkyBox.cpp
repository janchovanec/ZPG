#include "SkyBox.h"

#include <SOIL.h>

#include "Models/skycube.h"

SkyBox::SkyBox(const std::string& posx, const std::string& negx, const std::string& posy, const std::string& negy, const std::string& posz, const std::string& negz) {
    vertexCount = 108;
    VBO = 0;
    glGenBuffers(1, &VBO); // generate the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skycube), &skycube[0], GL_STATIC_DRAW);

    VAO = 0;
    glGenVertexArrays(1, &VAO); //generate the VAO
    glBindVertexArray(VAO); //bind the VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0);

    glActiveTexture(GL_TEXTURE0);
	textureID = SOIL_load_OGL_cubemap(posx.c_str(), negx.c_str(), posy.c_str(), negy.c_str(), posz.c_str(), negz.c_str(), SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
    if (textureID == NULL) {
        std::cout << "An error occurred while loading CubeMap." << std::endl;
        exit(EXIT_FAILURE);
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS); //Smooth connection
}