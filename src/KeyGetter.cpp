#include "KeyGetter.h"

KeyGetter::KeyGetter(GLFWwindow* window) {
    this->window = window;

    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, KeyCallbackWrapper);
    glfwSetMouseButtonCallback(window, MouseButtonCallbackWrapper);
}

void KeyGetter::KeyCallbackWrapper(GLFWwindow* window, int key, int scancode, int action, int mods) {
    KeyGetter* keyGetter = static_cast<KeyGetter*>(glfwGetWindowUserPointer(window));
    keyGetter->keyCallback(window, key, scancode, action, mods);
}

void KeyGetter::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(action == GLFW_PRESS)
        pressed[ key ] = true;
    else if(action == GLFW_RELEASE)
        pressed[ key ] = false;

    //forceNotifyObservers();
}

void KeyGetter::MouseButtonCallbackWrapper(GLFWwindow* window, int button, int action, int mods) {
    KeyGetter* keyGetter = static_cast<KeyGetter*>(glfwGetWindowUserPointer(window));
    keyGetter->mouseCallback(window, button, action, mods);
}

void KeyGetter::mouseCallback(GLFWwindow* window, int button, int action, int mods) {
    if(action == GLFW_PRESS)
        pressed[ button ] = true;
    else if(action == GLFW_RELEASE)
        pressed[ button ] = false;

    //forceNotifyObservers();

}

void KeyGetter::registerObserver(KeyGetterObserver* observer) {
    observers.push_back(observer);
}

void KeyGetter::unregisterObserver(KeyGetterObserver* observer) {
    auto it = std::find(observers.begin(), observers.end(), observer);
    if(it != observers.end()) 
        observers.erase(it);

}

void KeyGetter::forceNotifyObservers() {
    for(auto observer : observers) {
        observer->onKeyStatesChanged(pressed, NULL, window);
    }
}
