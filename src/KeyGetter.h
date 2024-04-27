#pragma once
#include <GLFW/glfw3.h>
#include <vector>
#include <algorithm>

class KeyGetterObserver {
public:
    virtual void onKeyStatesChanged(const bool* pressed, int key, GLFWwindow* window) = 0;
};

class KeyGetter {
    std::vector<KeyGetterObserver*> observers;
    GLFWwindow* window;
    bool pressed[ GLFW_KEY_LAST ] = { false };
public:

    KeyGetter(GLFWwindow* window);

    static void KeyCallbackWrapper(GLFWwindow* window, int key, int scancode, int action, int mods);
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    static void MouseButtonCallbackWrapper(GLFWwindow* window, int button, int action, int mods);
    void mouseCallback(GLFWwindow* window, int button, int action, int mods);

    void registerObserver(KeyGetterObserver* observer);

    void unregisterObserver(KeyGetterObserver* observer);

    void forceNotifyObservers();
};