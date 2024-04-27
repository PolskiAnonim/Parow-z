#include "Grafika.h"

#define PI 3.14f

std::map<std::string, Shader*> shaders;

int width = 1200, height = 1200;

GLFWwindow* init() {
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(width, height, "Parowoz", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glewInit();


    glEnable(GL_DEPTH_TEST);

    shaders.insert(std::make_pair("draw", new Shader("v.glsl", "", "f.glsl")));
    shaders.insert(std::make_pair("instance", new Shader("vInstance.glsl", "", "fInstance.glsl")));
    shaders.insert(std::make_pair("skyBox", new Shader("vSkyBox.glsl", "", "fSkyBox.glsl")));
    shaders.insert(std::make_pair("chmurki", new Shader("vChmurki.glsl", "", "fChmurki.glsl")));

    
    return window;
}

void free(GLFWwindow* window) {
    for(auto& shader : shaders) {
        delete shader.second;
    }
}

void draw(GLFWwindow* window, Camera camera, std::vector<Model*> models, SkyBox* skybox) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    Shader** activeShader;
  
    

    //1. Draw normalny Modele 1-3 - nie instancjonowane
    activeShader = &(shaders[ "draw" ]);
    (*activeShader)->enable();
    camera.matrix(50.0f, 0.01f, 500.0f, *activeShader, "cam");
    for(int i = 0; i != 3;i++ ) {
        models[i]->draw(**activeShader);
    }

    //2. Draw instancyjny 4-last
    activeShader = &(shaders[ "instance" ]);
    (*activeShader)->enable();
    camera.matrix(50.0f, 0.01f, 500.0f, *activeShader, "cam");
    for(int i = 5; i !=8; i++) {
        models[ i ]->draw(**activeShader);

    }



    //2.5 Chmurki
    activeShader = &(shaders[ "chmurki" ]);
    (*activeShader)->enable();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    camera.matrix(50.0f, 0.01f, 500.0f, *activeShader, "cam");
    models[ models.size() - 1]->draw(**activeShader);
    glDisable(GL_BLEND);

    //3. SkyBox
    activeShader = &(shaders[ "skyBox" ]);
    (*activeShader)->enable();
    camera.matrix(50.0f, 0.01f, 500.0f, *activeShader, "cam");
    skybox->draw(**activeShader);

    //4. Drzewka (przezroczyste)
    activeShader = &(shaders["instance"]);
    (*activeShader)->enable();
    for (int i = 3; i <=4; i++) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        models[i]->draw(**activeShader);
        glDisable(GL_BLEND);
    }

    //4. Obliczenie animacji
    std::vector<std::future<void>> futures;
    for(Model* model : models) {
        std::future<void> future = std::async(std::launch::async, [model]() {
            model->animate();
            });

        futures.push_back(std::move(future));
    }
    
   
    for(std::future<void>& future : futures) {
        future.wait();
    }
    updateS();
    glfwSwapBuffers(window);
}

int main() {
    srand(time(NULL));
    GLFWwindow* window = init();
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    KeyGetter* keyGetter = new KeyGetter(window);
    Camera camera(width, height, glm::vec3(-20.0f, 30.0f, 2.0f),keyGetter);

    std::vector<Model*> loadedModels;
    loadedModels.push_back(new Train("./models/pociagi/t3/animowany30.obj", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 10.0f, 0.068f))));
    loadedModels.push_back(new Cart("./models/cart/b.obj", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 17.0f, 0.04f))));
    loadedModels.push_back(new PolandMountain("./models/terrain/m.obj", glm::translate(glm::mat4(1.0f), glm::vec3(210.0f, 0.0f,-1.0f))));
    loadedModels.push_back(new Tree("./models/tree/t.obj", glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f, 250.0f, -1.0f))));
    loadedModels.push_back(new Tree("./models/tree/t.obj", glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 252.0f, -1.0f))));
    loadedModels.push_back(new Ground("./models/ground/g.obj", glm::translate(glm::mat4(1.0f), glm::vec3(250.0f,250.0f,0.0f))));
    loadedModels.push_back(new Rail("./models/rail/r.obj", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 300.0f, 0.0f))));
    loadedModels.push_back(new Lantern("./models/lantern/l.obj", glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 320.0f, 0.0f))));
    loadedModels.push_back(new Cloud("./models/cloud/c.obj", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 8.9f, 2.0f))));
    SkyBox* skybox = new SkyBox("./models/sky/s.png");

    keyGetter->registerObserver(&camera);
    keyGetter->registerObserver(loadedModels[0]);


    while (!glfwWindowShouldClose(window)) {
        glfwSetTime(0);
        // ProcessInput działa na przerwaniach
        draw(window,camera, loadedModels, skybox);
        glfwPollEvents();
        keyGetter->forceNotifyObservers();

        
    }

    for(Model* model : loadedModels) {
        delete model;
    }
   
  
    delete keyGetter;
    glfwTerminate();
    return 0;
}


