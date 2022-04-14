#include <iostream>
#include <filesystem>
#include "gl/Renderer.h"
#include "glfw/glfw3.h"
#include "gltf.h"
#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

struct App {
    GLFWwindow* window;
    gl::Renderer renderer;
    Matrix4 camera;
    std::shared_ptr<SceneNode> scene;
    uint32_t frameId{};
};

std::unique_ptr<App> app{};

GLFWwindow* createWindow() {
    assert(glfwInit());
    GLFWwindow* window = glfwCreateWindow(600, 600, "Lawless", nullptr, nullptr);
    glfwMakeContextCurrent(window);

#ifndef EMSCRIPTEN
    glewExperimental = true;
    assert(glewInit() == GLEW_OK);
#endif

    return window;
}

void tick() {
    app->renderer.Render(app->camera, *app->scene);
    app->frameId++;
    
    app->scene->transform.rotation.y += .001f;

    glfwSwapBuffers(app->window);
    glfwPollEvents();
}

void onCursorMoved(GLFWwindow* window, double x, double y) {
    // rotate
}

int main(int argc, char* argv[]) {
    std::cout << "CWD : " << std::filesystem::current_path() << std::endl;



#ifndef EMSCRIPTEN
    std::string path = "..\\..\\..\\..\\assets\\suzanne.glb";
#else
    std::string path = "./suzanne.glb";
#endif

    app = std::make_unique<App>(App{
       createWindow(),
       gl::Renderer{},
       Matrix4::Perspective(110.0f, 1, 100),
       std::make_shared<SceneNode>()
    });

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;

    std::shared_ptr<SceneNode> suzanne = gltf::Load(path);

    std::shared_ptr<SceneNode> node1 = std::make_shared<SceneNode>();
    node1->transform.position = Vector3(4, 0.0, 0.0);
    node1->children.push_back(suzanne);

    std::shared_ptr<SceneNode> node2 = std::make_shared<SceneNode>();
    node2->transform.position = Vector3(-4, 0.0, 0.0);
    node2->children.push_back(suzanne);

    app->scene->children.push_back(node1);
    app->scene->children.push_back(node2);

    app->scene->transform.position = Vector3(0.0f, -0.0f, -30.0);

    glfwSetCursorPosCallback(app->window, onCursorMoved);

#ifndef EMSCRIPTEN
    while (!glfwWindowShouldClose(app->window)) {
        tick();
    }
#else
    emscripten_set_main_loop(tick, 0, true);
#endif

    return 0;
}