#include "TexturedModel.h"
#include <SOIL.h>
#include <assimp/postprocess.h>
#include "utils.h"

TexturedModel::TexturedModel(const float* vertices, int size, int count, const char* texturePath) {
	vertexCount = count;
    //vertex buffer object (VBO)
    VBO = 0;
    glGenBuffers(1, &VBO); // generate the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    //vertex attribute object(VAO)
    VAO = 0;
    glGenVertexArrays(1, &VAO); //generate the VAO
    glBindVertexArray(VAO); //bind the VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glEnableVertexAttribArray(0); //enable vertex attributes
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(6 * sizeof(float)));

    glActiveTexture(GL_TEXTURE0);
    textureID = SOIL_load_OGL_texture(texturePath, SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    if (textureID == NULL) {
        std::cout << "An error occurred while loading image." << std::endl;
        exit(EXIT_FAILURE);
    }
    glBindTexture(GL_TEXTURE_2D, textureID);

    // set texture parameters
    setTextureParams(GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);
	assimpModel = false;
}

void TexturedModel::draw() {
    glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_2D, textureID);
	if (assimpModel) {
		glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, NULL);
	}
	else {
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	}
    glBindVertexArray(0);
}

TexturedModel::TexturedModel()
{
	assimpModel = false;
	textureID = 0;
}

void TexturedModel::setTextureParams(GLint wrapS, GLint wrapT, GLint minFilter, GLint magFilter)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
}

/*********************************Assimp*******************************/
TexturedModel::TexturedModel(const std::string& modelPath) {
    loadModel(modelPath);
	assimpModel = true;
}

void TexturedModel::loadModel(const std::string& path) {
    Assimp::Importer importer;
    unsigned int importOptions = aiProcess_Triangulate //Converts polygons to triangles
        | aiProcess_OptimizeMeshes              // Reduces the number of submeshes
        | aiProcess_JoinIdenticalVertices       // Removes duplicate vertices
        | aiProcess_CalcTangentSpace;           // Computes tangents and bitangents

    //aiProcess_GenNormals/ai_Process_GenSmoothNormals - Generates flat/Smooth normals

    const aiScene* scene = importer.ReadFile(path, importOptions);

    if (scene) { //pokud bylo nacteni uspesne
        printf("scene->mNumMeshes = %d\n", scene->mNumMeshes);
        printf("scene->mNumMaterials = %d\n", scene->mNumMaterials);
        //Materials
        for (unsigned int i = 0; i < scene->mNumMaterials; i++)
        {
            const aiMaterial* mat = scene->mMaterials[i];
            aiString name;
            mat->Get(AI_MATKEY_NAME, name);
            printf("Material [%d] name %s\n", i, name.C_Str());
            aiColor4D d;
            glm::vec4 diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
            if (AI_SUCCESS == aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &d))
                diffuse = glm::vec4(d.r, d.g, d.b, d.a);
        }
        //Objects
        for (unsigned int i = 0; i < scene->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[i];
            Vertex* pVertices = new Vertex[mesh->mNumVertices];
            std::memset(pVertices, 0, sizeof(Vertex) * mesh->mNumVertices);
            for (unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                if (mesh->HasPositions()) {
                    pVertices[i].Position[0] = mesh->mVertices[i].x;
                    pVertices[i].Position[1] = mesh->mVertices[i].y;
                    pVertices[i].Position[2] = mesh->mVertices[i].z;
                }

                if (mesh->HasNormals()) {
                    pVertices[i].Normal[0] = mesh->mNormals[i].x;
                    pVertices[i].Normal[1] = mesh->mNormals[i].y;
                    pVertices[i].Normal[2] = mesh->mNormals[i].z;
                }

                if (mesh->HasTextureCoords(0)) {
                    pVertices[i].Texture[0] = mesh->mTextureCoords[0][i].x;
                    pVertices[i].Texture[1] = mesh->mTextureCoords[0][i].y;
                }

                if (mesh->HasTangentsAndBitangents()) {
                    pVertices[i].Tangent[0] = mesh->mTangents[i].x;
                    pVertices[i].Tangent[1] = mesh->mTangents[i].y;
                    pVertices[i].Tangent[2] = mesh->mTangents[i].z;
                }
            }

            unsigned int* pIndices = nullptr;
            if (mesh->HasFaces()) {
                pIndices = new unsigned int[mesh->mNumFaces * 3];
                for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
                    pIndices[i * 3] = mesh->mFaces[i].mIndices[0];
                    pIndices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
                    pIndices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
                }
            }

            glGenVertexArrays(1, &VAO);

            VBO = 0;
            glGenBuffers(1, &VBO);

            GLuint IBO = 0;
            glGenBuffers(1, &IBO);

            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mesh->mNumVertices, pVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(0));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));

            //Tangent for Normal Map
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(8 * sizeof(GLfloat)));

            //Index Buffer
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh->mNumFaces * 3, pIndices, GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(VAO);

            GLuint err = glGetError();
            if (err != GL_NO_ERROR) {
                std::cout << "GL ERROR: " << err << std::endl;
            }
            vertexCount = mesh->mNumFaces * 3;
            delete[] pVertices;
            delete[] pIndices;
        }
    }
    else {
        printf("Error during parsing mesh from %s : %s \n", path.c_str(), importer.GetErrorString());
    }
    glBindVertexArray(0);

	// png path (has to have the same name as the model)
	auto pngPath = path.substr(0, path.find_last_of('.')) + ".png";

    glActiveTexture(GL_TEXTURE0);
    textureID = SOIL_load_OGL_texture(pngPath.c_str(), SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    if (textureID == NULL) {
        std::cout << "An error occurred while loading texture." << std::endl;
        exit(EXIT_FAILURE);
    }
    glBindTexture(GL_TEXTURE_2D, textureID);
}