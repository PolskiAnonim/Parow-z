#include "Model.h"

float vt = 1.0f* 0.016f; //predkosc liniowa*1/144 (klatki) 1 m/s = 3.6 km/h
float s = 0.0f;
float RENDER_DISTANCE = 500.0f;


void updateS() {
	s += vt;
}

void changeSpeed(float speed) {
	vt = glm::min(speed, 30.0f) * 0.016;
	vt = vt < 0.0f ? 0.0f : vt; // ¯eby nie by³o jak w tej starej grze z wyœcigami ciê¿arówek gdzie w mo¿na by³o przyspieszaæ w nieskoñczonoœæ na wstecznym
}

void Mesh::setUpMesh(){
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex),&this->vertices[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() *
		sizeof(GLuint), &this->indices[0], GL_DYNAMIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),(void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),(void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),(void*)offsetof(Vertex, TexCoords));
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures, std::string name) {
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	this->name = name;

	this->setUpMesh();
}


void Mesh::Draw(Shader& shader, glm::mat4& modelMatrix){
	int nr[] = { 1,1,1,1,1 };
	for (int i = 0; i != this->textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i); // activate texture unit first
		std::string number;
		std::string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(nr[0]++);
		else if (name == "texture_specular")
			number = std::to_string(nr[1]++);
		else if (name == "texture_normal")
			number = std::to_string(nr[2]++);
		else if(name == "texture_emission")
			number = std::to_string(nr[3]++);
		else if(name == "texture_shininess")
			number = std::to_string(nr[4]++);
		shader.setFloat(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
	}
	GLint przekazano[ 5 ];
	std::transform(std::begin(nr), std::end(nr), std::begin(przekazano), [](int n) { return (n > 1) ? 1 : 0; });

		glm::vec3 lampPositions[] = {
		glm::vec3(3.0f, fmod(s, 80), 1.0f),
		};
		glUniform1i(shader.u("lampNumber"), 1);
		glUniform3fv(shader.u("lampPositions"), 1, glm::value_ptr(lampPositions[ 0 ]));
	
	
	

	glUniform1iv(shader.u("przekazano"), 5, przekazano);
	glUniform3fv(shader.u("sunColor"), 1, glm::value_ptr(glm::vec3(1.0f, 0.96f, 0.61f)));
	glUniform3fv(shader.u("sunPos"), 1, glm::value_ptr(glm::vec3(-100.0f, -100.0f, 70.0f)));

	glUniformMatrix4fv(shader.u("model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), this->vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), this->indices.data(), GL_STATIC_DRAW);

	if(this->instances.size() > 0) {
		glBindBuffer(GL_ARRAY_BUFFER, this->instanceVBO);
		glBufferData(GL_ARRAY_BUFFER, this->instances.size() * sizeof(glm::vec3), this->instances.data(), GL_STATIC_DRAW);

		glDrawElementsInstanced(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0, this->instances.size());
	}
	else {
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);

	for (int i = 0; i != this->textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D,i);
	}

}

void Mesh::applyTransformation(const glm::mat4& transformation) {
	for(Vertex& vertex: vertices ) {
		vertex.Position = glm::vec3(transformation * glm::vec4(vertex.Position,1.0f));
		vertex.Normal = glm::normalize(glm::mat3(transformation) * vertex.Normal);
	}
}

void Model::loadModel(std::string mPath,glm::mat4 transition, bool flip) {
	Assimp::Importer import;
	Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);
	Assimp::DefaultLogger::get()->attachStream(new Logger());
	const aiScene* scene;
	int flags = aiProcessPreset_TargetRealtime_MaxQuality;
	if(flip) flags |= aiProcess_FlipUVs;

	
	scene = import.ReadFile(mPath, flags);
	
	modelMatrix = transition;
	directory = mPath.substr(0, mPath.find_last_of('/'));
	processNode(scene->mRootNode, scene, aiMatrix4x4());
}

void Model::processNode(aiNode* node, const aiScene* scene, aiMatrix4x4 transformation) {
	for (GLuint i = 0; i != node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		transformation *= node->mTransformation;
		meshes.push_back(processMesh(mesh, scene, transformation));

	}

	for (GLuint i = 0; i != node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene, transformation);
	}
}


void printMaterialTextures(aiMaterial* material) {
	for(int ai = aiTextureType_NONE; ai != aiTextureType_UNKNOWN; ai++) {
		aiTextureType textureType = static_cast<aiTextureType>(ai);
		unsigned int textureCount = material->GetTextureCount(textureType);

		std::cout << "Number of " << textureType << " textures: " << textureCount << std::endl;
		for(unsigned int i = 0; i < textureCount; i++) {
			aiString texturePath;
			material->GetTexture(textureType, i, &texturePath);
			std::cout << textureType << " Texture[" << i << "]: " << texturePath.C_Str() << std::endl;
		}
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene, aiMatrix4x4 transformation) {
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	for (int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		glm::vec3 vec;

		vec.x = (transformation * mesh->mVertices[i]).x;
		vec.y = (transformation * mesh->mVertices[i]).y;
		vec.z = (transformation * mesh->mVertices[i]).z;
		vec = glm::vec3(modelMatrix * glm::vec4(vec, 1.0f));
		vertex.Position = vec;

		if (mesh->HasNormals()) {
			vec.x = mesh->mNormals[i].x;
			vec.y = mesh->mNormals[i].y;
			vec.z = mesh->mNormals[i].z;

			vec = glm::normalize(glm::mat3(modelMatrix) * vec);

			vertex.Normal = vec;

		}


		if (mesh->mTextureCoords[0]) {
			glm::vec2 vec2;

			vec2.x = mesh->mTextureCoords[0][i].x;
			vec2.y = mesh->mTextureCoords[0][i].y;

			vertex.TexCoords = vec2;

		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		

		vertices.push_back(vertex);
	}
	for (int i = 0; i != mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j != face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= 0) {
		std::cout << (std::string)mesh->mName.C_Str() << std::endl;
		printMaterialTextures(scene->mMaterials[ mesh->mMaterialIndex ]);

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// 1. diffuse maps
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. specular maps
		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// 3. normal maps
		std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		// 4. emission maps
		std::vector<Texture> emissionMaps = loadMaterialTextures(material, aiTextureType_EMISSIVE, "texture_emission");
		textures.insert(textures.end(), emissionMaps.begin(), emissionMaps.end());
		// 7. roughness
		std::vector<Texture> roughnessMMaps = loadMaterialTextures(material, aiTextureType_SHININESS, "texture_shininess");
		textures.insert(textures.end(), roughnessMMaps.begin(), roughnessMMaps.end());

	}

	

	return Mesh(vertices, indices, textures, (mesh->mName).C_Str() /*, extents, origin, mesh->mName*/);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName) {
	std::vector<Texture> textures;
	for (int i = 0; i != material->GetTextureCount(type); i++) {
		aiString str;
		material->GetTexture(type, i, &str);

		bool skip = false;

		for (int j = 0; j != textures_loaded.size(); j++) {
			if (textures_loaded[j].path == str) {
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip) {
			Texture texture;
			texture.id = getTextureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);

			this->textures_loaded.push_back(texture);
		}
	}

	return textures;
}

GLuint Model::getTextureFromFile(std::string path, std::string& directory){
	std::string fileName = directory + '/' + path;

	GLuint texID;
	glGenTextures(1, &texID);

	int width, height, nComp;
	unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nComp, 0);
	if (data) {
		GLenum format;
		switch (nComp) {
		case 1:
			format = GL_RED;
			break;
		case 3:
			format = GL_RGB;
			break;
		case 4:
			format = GL_RGBA;
			break;
		}
		glBindTexture(GL_TEXTURE_2D, texID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return texID;
}


void Model::draw(Shader& shader) {
	for(int i = 0; i != meshes.size(); i++)
		meshes[ i ].Draw(shader, this->modelMatrix);
}

Model::~Model() {}

void Logger::write(const char* message) {
	std::cout << "Assimp Log: " << message << std::endl;
}

Train::~Train() {
}

glm::vec3 calculateCenter(const std::vector<Vertex>& vertices,float & r) {
	glm::vec3 minValues = glm::vec3(std::numeric_limits<float>::max());
	glm::vec3 maxValues = glm::vec3(std::numeric_limits<float>::lowest());

	for(const Vertex& vertex : vertices) {
		minValues = glm::min(minValues, vertex.Position);
		maxValues = glm::max(maxValues, vertex.Position);
	}

	glm::vec3 center = (minValues + maxValues) / 2.0f;
	r = glm::distance(center, maxValues);
	return center;
}

glm::vec3 getMax(const std::vector<Vertex>& vertices) {
	glm::vec3 maxValues = glm::vec3(std::numeric_limits<float>::lowest());

	for(const Vertex& vertex : vertices) 
		maxValues = glm::max(maxValues, vertex.Position);
	
	return maxValues;
}

glm::vec3 getMin(const std::vector<Vertex>& vertices) {
	glm::vec3 minValues = glm::vec3(std::numeric_limits<float>::infinity());

	for(const Vertex& vertex : vertices)
		minValues = glm::min(minValues, vertex.Position);

	return minValues;
}

void Train::onKeyStatesChanged(const bool* pressed, int key, GLFWwindow* window) {
	if(pressed[ GLFW_KEY_UP ]) changeSpeed((vt* 1 / 0.016) + 0.02f);
	if(pressed[ GLFW_KEY_DOWN ]) changeSpeed((vt* 1 / 0.016) - 0.02f);
}

void Train::animate() {
	//Wheels
	glm::vec3 center;
	float pgd;
	float plp;
	float r = 0.305;
	float dx;
	std::for_each(meshes.begin(), meshes.end(), [&](Mesh& mesh) {
		if(mesh.name.find("wheel") != std::string::npos) {
			float rk;	//promieñ ko³a albo czegoœ podobnego
			center = calculateCenter(mesh.vertices, rk);
			mesh.meshMatrix = glm::translate(mesh.meshMatrix, center);
			this->obr = 360 * vt / (2 * 3.1415 * rk);
			mesh.meshMatrix = glm::rotate(mesh.meshMatrix, glm::radians(obr), glm::vec3(1.0f, 0.0f, 0.0f));
			mesh.meshMatrix = glm::translate(mesh.meshMatrix, -center);
			mesh.applyTransformation(mesh.meshMatrix);
			mesh.meshMatrix = glm::mat4(1.0f);
		}
		// bela
		else if(mesh.name.find("belka_tlok_kolo") != std::string::npos) {
			this->sumobr += obr; //ostatnie ko³o (po to mi to by³o lol)
			this->sumobr = sumobr >= 360 ? sumobr - 360 : sumobr;
			pgd = sin(glm::radians(180 - sumobr - obr)) - sin(glm::radians(180 - sumobr));
			plp = cos(glm::radians(180 - sumobr - obr)) - cos(glm::radians(180 - sumobr));

			float length = 2.0f; //belki akurat dziala

			dx = cos(glm::radians(180 - sumobr)) * r + sqrt(length * length - sin(glm::radians(180 - sumobr)) * r * sin(glm::radians(180 - sumobr)) * r);
			dx -= cos(glm::radians(180 - sumobr-obr)) * r + sqrt(length * length - sin(glm::radians(180 - sumobr-obr)) * r * sin(glm::radians(180 - sumobr-obr)) * r);
			
			float cosinus = sin(glm::radians(180 - sumobr)) * r / length; //cosinus trojkata prostokatnego (tlok-y z promienia-rzutowanie y na os x)
			float cosinus2 = sin(glm::radians(180 - sumobr - obr)) * r / length; //jw ale dla nowego
			float deg = -acos(cosinus2) + acos(cosinus);
			mesh.meshMatrix = glm::translate(mesh.meshMatrix, center);
			mesh.meshMatrix = glm::translate(mesh.meshMatrix, glm::vec3(0.0f, -cos(glm::radians(180 - sumobr - obr)) * r, sin(glm::radians(180 - sumobr - obr)) * r));
			mesh.meshMatrix = glm::rotate(mesh.meshMatrix, deg, glm::vec3(1.0f, 0.0f, 0.0f));
			mesh.meshMatrix = glm::translate(mesh.meshMatrix, glm::vec3(0.0f, cos(glm::radians(180 - sumobr - obr)) * r, -sin(glm::radians(180 - sumobr - obr)) * r));
			mesh.meshMatrix = glm::translate(mesh.meshMatrix, -center);
			mesh.meshMatrix = glm::translate(mesh.meshMatrix, glm::vec3(0.0f, -plp * r, pgd * r));


			mesh.applyTransformation(mesh.meshMatrix);
			mesh.meshMatrix = glm::mat4(1.0f);
		}
		else if(mesh.name.find("tlok_przod_tyl") != std::string::npos) {
			mesh.meshMatrix = glm::translate(mesh.meshMatrix, glm::vec3(0.0f, dx, 0.0f));

			mesh.applyTransformation(mesh.meshMatrix);
			mesh.meshMatrix = glm::mat4(1.0f);

		}

		else if(mesh.name.find("belka_kolo_kolo") != std::string::npos) {
			mesh.meshMatrix = glm::translate(mesh.meshMatrix, glm::vec3(0.0f, -plp * r, pgd * r));

			mesh.applyTransformation(mesh.meshMatrix);
			mesh.meshMatrix = glm::mat4(1.0f);
		}
		});

}

void Cart::animate() {
	std::for_each(meshes.begin(), meshes.end(), [&](Mesh& mesh) {
		if (mesh.name.find("wheel") != std::string::npos) {
			float rk;	//promieñ ko³a albo czegoœ podobnego
			glm::vec3 center = calculateCenter(mesh.vertices, rk);
			mesh.meshMatrix = glm::translate(mesh.meshMatrix, center);
			this->obr = 360 * vt / (2 * 3.1415 * rk);
			mesh.meshMatrix = glm::rotate(mesh.meshMatrix, glm::radians(obr), glm::vec3(1.0f, 0.0f, 0.0f));
			mesh.meshMatrix = glm::translate(mesh.meshMatrix, -center);
			mesh.applyTransformation(mesh.meshMatrix);
			mesh.meshMatrix = glm::mat4(1.0f);
		}
		});
}

void PolandMountain::animate() {
}

void Mesh::propagate(GLuint x, GLuint y, GLuint interval) {
	for(GLuint i = 0; i != x; i++) {
		if(i * interval > 4*RENDER_DISTANCE) break;
		for(GLuint j = 0; j != y; j++) {
			if(j * interval > 2 * RENDER_DISTANCE) break;
			glm::vec3 instance = glm::vec3(j * interval, i * interval, 0.0f);
			instances.push_back(instance);
		}
	}
	glGenBuffers(1, &this->instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->instanceVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * instances.size(), instances.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	glVertexAttribDivisor(3, 1);
}

void Mesh::updatePropagate(GLuint interval) {
	for(int i = 0; i != instances.size();i++) {
		if(std::abs(instances[i].y) < s) {
			instances[i].y += 4*RENDER_DISTANCE + interval;
		}
		else {
			if(i == 0) return;
			std::rotate(instances.begin(), instances.begin() + i, instances.end());
			return;
		}
	}
	
	
}

bool Model::isInstanced() {
	if(this->meshes[ 0 ].instances.size() > 0)
		return true;
	return false;
}

void Rail::animate() {
	for(Mesh& mesh : meshes) {
		mesh.meshMatrix = glm::translate(mesh.meshMatrix, glm::vec3(0.0f, vt , 0.0f));
		mesh.applyTransformation(mesh.meshMatrix);
		mesh.meshMatrix = glm::mat4(1.0f);
		mesh.updatePropagate(this->instanceInterval);
	}
}

void Ground::animate() {
	for(Mesh& mesh : meshes) {
		mesh.meshMatrix = glm::translate(mesh.meshMatrix, glm::vec3(0.0f, vt , 0.0f));
		mesh.applyTransformation(mesh.meshMatrix);
		mesh.meshMatrix = glm::mat4(1.0f);
		mesh.updatePropagate(this->instanceInterval);
	}
}

void Tree::animate() {
	for(Mesh& mesh : meshes) {
		mesh.meshMatrix = glm::translate(mesh.meshMatrix, glm::vec3(0.0f, vt , 0.0f));
		mesh.applyTransformation(mesh.meshMatrix);
		mesh.meshMatrix = glm::mat4(1.0f);
		mesh.updatePropagate(this->instanceInterval);
	}
}

void Cloud::animate() {
	for(Mesh& mesh : meshes) {
		mesh.meshMatrix = glm::mat4(1.0f);
		for(int i = 0; i != mesh.instances.size(); i++) {
			mesh.instances[ i ].y -= (rand()%10 * 0.003 + vt/4); //do tylu
			mesh.instances[i].z -= rand() % 15 * 0.001f; //do gory
			if (i%3==0)
				mesh.instances[ i ].x += fmod(rand(), 0.001);
			else if(i%3==1)
				mesh.instances[ i ].x -= fmod(rand(), 0.001);
			if(std::abs(mesh.instances[ i ].y) > 15.0f - vt/16) {
				mesh.instances[ i ].y = 0;
				mesh.instances[ i ].z = 0;
				mesh.instances[ i ].x = 0;
			}
		}
	}
}

void Lantern::animate() {
	for(Mesh& mesh : meshes) {
		mesh.meshMatrix = glm::translate(mesh.meshMatrix, glm::vec3(0.0f, vt, 0.0f));
		mesh.applyTransformation(mesh.meshMatrix);
		mesh.meshMatrix = glm::mat4(1.0f);
		mesh.updatePropagate(this->instanceInterval);
	}
}