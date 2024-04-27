#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include "Shader.h"
#include "KeyGetter.h"

class Camera : public KeyGetterObserver { 
public:
	int width, height;
	glm::vec3 EyePosition; // Position
	glm::vec3 LookAtDirection = glm::vec3(1.0f, 0.0f, 0.0f); // Orientation
	glm::vec3 Up = glm::vec3(0.0f, 0.0f, 1.0f);

	KeyGetter* keyGetter;

	bool firstClick = true;
	double lastMouseX, lastMouseY;

	float speed = 0.1f;
	float sensitivity = 150.0f;


	Camera(int width, int height, glm::vec3 EyePosition, KeyGetter* keyGetter);
	
	void matrix(float FOV, float nearPlane, float farPlane, Shader* shader, std::string uniform);

	void onKeyStatesChanged(const bool* pressed, int key, GLFWwindow* window) override;
};