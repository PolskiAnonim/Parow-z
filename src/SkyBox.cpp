#include "SkyBox.h"

void SkyBox::loadSkyBox(std::string mPath) {
	glGenTextures(1, &skyboxTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);


    std::vector<std::string> skyboxPaths = {
    "px.jpg",   
    "nx.jpg",    
    "py.jpg",     
    "ny.jpg",  
    "pz.jpg",    
    "nz.jpg"    
    };
    this->skyBoxMartix = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	int width, height, nrChannels;
	unsigned char* data;
    for(int i =0;i!=6;i++){
	    data = stbi_load(("./models/sky/" + skyboxPaths[i]).c_str(), &width, &height, &nrChannels, 0);
        if(data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else {
            std::cout << "AAAAAAA" << std::endl;
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void SkyBox::setupSkyboxVertices() {

    float skyboxVertices[] = {
    //px
  -1500.0f, -1500.0f, -1500.0f,
  -1500.0f, -1500.0f,  1500.0f,
   1500.0f, -1500.0f, -1500.0f,
   1500.0f, -1500.0f, -1500.0f,
  -1500.0f, -1500.0f,  1500.0f,
   1500.0f, -1500.0f,  1500.0f,

   //nx
  -1500.0f,  1500.0f, -1500.0f,
   1500.0f,  1500.0f, -1500.0f,
   1500.0f,  1500.0f,  1500.0f,
   1500.0f,  1500.0f,  1500.0f,
  -1500.0f,  1500.0f,  1500.0f,
  -1500.0f,  1500.0f, -1500.0f,

   //py
   1500.0f, -1500.0f, -1500.0f,
   1500.0f, -1500.0f,  1500.0f,
   1500.0f,  1500.0f,  1500.0f,
   1500.0f,  1500.0f,  1500.0f,
   1500.0f,  1500.0f, -1500.0f,
   1500.0f, -1500.0f, -1500.0f,
   //ny
  -1500.0f, -1500.0f,  1500.0f,
  -1500.0f,  1500.0f,  1500.0f,
   1500.0f,  1500.0f,  1500.0f,
   1500.0f,  1500.0f,  1500.0f,
   1500.0f, -1500.0f,  1500.0f,
  -1500.0f, -1500.0f,  1500.0f,
    // pz
  -1500.0f, -1500.0f,  1500.0f,
  -1500.0f, -1500.0f, -1500.0f,
  -1500.0f,  1500.0f, -1500.0f,
  -1500.0f,  1500.0f, -1500.0f,
  -1500.0f,  1500.0f,  1500.0f,
  -1500.0f, -1500.0f,  1500.0f,
  // nz
   -1500.0f,  1500.0f, -1500.0f,
  -1500.0f, -1500.0f, -1500.0f,
   1500.0f, -1500.0f, -1500.0f,
   1500.0f, -1500.0f, -1500.0f,
   1500.0f,  1500.0f, -1500.0f,
  -1500.0f,  1500.0f, -1500.0f,
    
};

    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);

    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_DYNAMIC_DRAW); // Tak to szalone bruh

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void SkyBox::draw(Shader& shader) {
    glDepthFunc(GL_LEQUAL);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
    glUniformMatrix4fv(shader.u("model"), 1, GL_FALSE, glm::value_ptr(skyBoxMartix));
    glUniform1i(shader.u("textureSampler"), 0);

    glBindVertexArray(this->skyboxVAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}