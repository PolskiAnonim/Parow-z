#pragma once
#define STB_IMAGE_IMPLEMENTATION
#define GLM_FORCE_CTOR_INIT


#include <GL/glew.h> // Tak... glew też jest potrzebny... inaczej nici z aliasów
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream> // na komentarze
#include <vector>
#include <future>
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "KeyGetter.h"
#include "SkyBox.h"
 