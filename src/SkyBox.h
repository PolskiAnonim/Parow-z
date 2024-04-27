#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include "Shader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include "Model.h"

struct SkyBoxVertex{};

class SkyBox {
	GLuint skyboxVAO, skyboxVBO;
	GLuint skyboxTexture;

	glm::mat4 skyBoxMartix;

	void loadSkyBox(std::string mPath);
	void setupSkyboxVertices();
public:
	SkyBox(std::string mPath) {
		loadSkyBox(mPath);
		setupSkyboxVertices();
	}
	void draw(Shader& shader);
};
