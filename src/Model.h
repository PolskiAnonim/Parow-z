#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <string>
#include "Shader.h"
#include <fstream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include <iostream>
#include <assimp/Logger.hpp>
#include <assimp/DefaultLogger.hpp>
#include <GLFW/glfw3.h>
#include<glm/gtc/quaternion.hpp>
#include "KeyGetter.h"
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

#include <stb_image.h>
#include <set>
#include <map>


class Logger : public Assimp::LogStream {
public:
    void write(const char* message);
};


struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    GLuint id;
    std::string type;
    aiString path;
};


class Mesh {
protected:
    void setUpMesh();
    GLuint VAO, VBO, EBO, instanceVBO;

public:
    std::string name;
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    std::vector<glm::vec3> instances;

    glm::mat4 meshMatrix = glm::mat4(1.0f);

    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices,
        std::vector<Texture> textures, std::string name);

    void Draw(Shader& shader, glm::mat4& modelMatrix);

    void applyTransformation(const glm::mat4& transformation);

    void propagate(GLuint x, GLuint y, GLuint interval);
    void updatePropagate(GLuint interval);
};

class Model : public KeyGetterObserver {
protected:
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> textures_loaded;
    glm::mat4 modelMatrix;
    GLuint instanceInterval = 0;

    void loadModel(std::string mPath,glm::mat4 transition, bool flip);
    void processNode(aiNode* node, const aiScene* scene, aiMatrix4x4 transformation);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene, aiMatrix4x4 transformation);
    std::vector<Texture> loadMaterialTextures(aiMaterial* material, aiTextureType type,
        std::string typeName);
    GLuint getTextureFromFile(std::string path, std::string& directory);
    

public:
    virtual void animate() = 0;
    Model() = default; // CHuj dzia³a
    Model(std::string mPath) {
        this->loadModel(mPath, glm::mat4(1.0), true);
    }
    explicit Model(std::string mPath,glm::mat4 transition) {
        this->loadModel(mPath, transition, true);
        std::sort(meshes.begin(), meshes.end(), [](const Mesh& a, const Mesh& b) {
            return a.name < b.name; // javaScript sort
            });
    };
    void draw(Shader& shader);
    bool isInstanced();

    ~Model();
};

class Ground : public Model {
public:
    void animate();
    Ground(std::string mPath, glm::mat4 trans) {
        Model::loadModel(mPath, trans, false);
        for(Mesh& mesh : meshes) {
            this->instanceInterval = 3.99999989f;
            mesh.propagate(20000, 1000, this->instanceInterval);
        }
    }
    void onKeyStatesChanged(const bool* pressed, int key, GLFWwindow* window) override {};

};

class Rail : public Model {
public:
    void animate();
    Rail(std::string mPath, glm::mat4 trans) {
        Model::loadModel(mPath, trans, true);
        for(Mesh& mesh : meshes) {
            this->instanceInterval = 4;
            mesh.propagate(1000, 1, this->instanceInterval);
        }
    }
    void onKeyStatesChanged(const bool* pressed, int key, GLFWwindow* window) override {};

};

class Tree : public Model {
public:
    void animate();
    Tree(std::string mPath, glm::mat4 trans) {
        Model::loadModel(mPath, trans, true);
        for(Mesh& mesh : meshes) {
            this->instanceInterval = 10;
            mesh.propagate(1000, 1, this->instanceInterval);
        }
    }
    void onKeyStatesChanged(const bool* pressed, int key, GLFWwindow* window) override {};

};

class PolandMountain : public Model {
public:
    void animate();
    PolandMountain(std::string mPath, glm::mat4 trans) : Model(mPath, trans) {}
    void onKeyStatesChanged(const bool* pressed, int key, GLFWwindow* window) override {};

    
};

class Cart : public Model {
    using Model::Model;
    float obr = 0.0f; //kat o ktory obraca sie kolo w jednym wykonaniu funkcji
public:
    void animate();
    //Cart(std::string mPath) : Model(mPath) {}
    Cart(std::string mPath, glm::mat4 trans) : Model(mPath,trans) {}
    void onKeyStatesChanged(const bool* pressed, int key, GLFWwindow* window) override {};
};

class Train : public Model {
    using Model::Model;
    KeyGetter* keyGetter;
    float obr = 0.0f; //kat o ktory obraca sie kolo w jednym wykonaniu funkcji
    float sumobr = 0.0f; //calkowity obrot kola (do 360 st)
public:
    // Yop profesional
    void animate() override;
    Train(std::string mPath) : Model(mPath,glm::mat4(1.0f)) {}
    Train(std::string mPath, glm::mat4 trans, KeyGetter* keyGetter) : Model(mPath,trans) {
        this->keyGetter = keyGetter;
    }

    void onKeyStatesChanged(const bool* pressed, int key, GLFWwindow* window) override;

    ~Train();
};

class Cloud : public Model {
public:
    void animate();
    Cloud(std::string mPath, glm::mat4 trans) {
        Model::loadModel(mPath, trans, true);
        for(Mesh& mesh : meshes) {
            this->instanceInterval = 0.1;
            mesh.propagate(1000, 1, this->instanceInterval);

        }
    }
    void onKeyStatesChanged(const bool* pressed, int key, GLFWwindow* window) override {};


};

class Sun : public Model {
public:
    void animate() {};
    Sun(std::string mPath) : Model(mPath, glm::mat4(1.0f)) {}
    Sun(std::string mPath, glm::mat4 trans) : Model(mPath, trans) {}

    void onKeyStatesChanged(const bool* pressed, int key, GLFWwindow* window) {};
};

class Lantern : public Model {
public:
    void animate();
    Lantern(std::string mPath, glm::mat4 trans) {
        Model::loadModel(mPath, trans, true);
        for(Mesh& mesh : meshes) {
            this->instanceInterval = 80;
            mesh.propagate(1000, 1, this->instanceInterval);

        }
    }
    void onKeyStatesChanged(const bool* pressed, int key, GLFWwindow* window) override {};


};


void updateS();



static inline glm::mat4 aiMatrix4ToGlm4(const aiMatrix4x4& from) {
    glm::mat4 to;
    to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
    to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
    to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
    to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
    return to;
}

