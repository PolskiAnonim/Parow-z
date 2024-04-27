#include "Camera.h"


Camera::Camera(int width, int height, glm::vec3 eyePosition, KeyGetter* keyGetter) {
	Camera::width = width;
	Camera::height = height;
	EyePosition = eyePosition;
	this->keyGetter = keyGetter;

}

void Camera::matrix(float FOV, float nearPlane, float farPlane, Shader* shader, std::string uniform) {
	glm::mat4 V = glm::mat4(1.0f);
	glm::mat4 P = glm::mat4(1.0f);

	V = glm::lookAt(EyePosition, EyePosition + LookAtDirection, Up);
	P = glm::perspective(glm::radians(FOV), (float)width / height, nearPlane, farPlane);

	glUniformMatrix4fv(shader->u(uniform), 1, false, glm::value_ptr(P * V));
}

void Camera::onKeyStatesChanged(const bool* pressed, int key, GLFWwindow* window) {
	if(pressed[ GLFW_KEY_W ]) EyePosition += speed * LookAtDirection;
	if(pressed[ GLFW_KEY_S ]) EyePosition += speed * -LookAtDirection;
	if(pressed[ GLFW_KEY_A ]) EyePosition += speed * -glm::normalize(glm::cross(LookAtDirection, Up));
	if(pressed[ GLFW_KEY_D ]) EyePosition += speed * glm::normalize(glm::cross(LookAtDirection, Up));

	if(pressed[ GLFW_KEY_SPACE ]) EyePosition += speed * Up;
	if(pressed[ GLFW_KEY_LEFT_CONTROL ]) EyePosition += speed * -Up;
	if(pressed[ GLFW_KEY_LEFT_SHIFT ]) speed = 1.0f;
	if(!pressed[ GLFW_KEY_LEFT_SHIFT ]) speed = 0.1f;

	if(pressed[ GLFW_MOUSE_BUTTON_LEFT ]) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		if(firstClick) {
			glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (height / 2)) / height;

		glm::vec3 NewLookAtDirection = glm::rotate(LookAtDirection, glm::radians(-rotX), glm::normalize(glm::cross(LookAtDirection, Up)));

		if(!(glm::angle(NewLookAtDirection, Up) <= glm::radians(5.0f) || (glm::angle(NewLookAtDirection, -Up) <= glm::radians(5.0f))))
			LookAtDirection = NewLookAtDirection;

		LookAtDirection = glm::rotate(LookAtDirection, glm::radians(-rotY), Up);

		glfwSetCursorPos(window, (width / 2), (height/2));
	}

	if(!pressed[ GLFW_MOUSE_BUTTON_LEFT ]) {
		//glfwSetCursorPos(window, lastMouseY, lastMouseX); // NAPRAW
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstClick = true;
	}

}
