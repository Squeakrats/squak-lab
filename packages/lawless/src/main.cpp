#include <iostream>
#include <filesystem>
#include "gl/Renderer.h"
#include "Window.h"
#include "gltf.h"
#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

struct App {
    Window window;
    gl::Renderer renderer;
    Matrix4 camera;
    std::shared_ptr<SceneNode> scene;
    uint32_t frameId;
};

std::unique_ptr<App> app{};

void tick() {
    app->renderer.Render(app->camera, *app->scene);
    app->frameId++;
    
    app->scene->children[0]->transform.rotation.y += .001f;
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
       Window::Create(600, 600, "Lawless"),
       gl::Renderer{},
       Matrix4::Perspective(110.0f, 1, 100),
       gltf::Load(path),
       0
    });

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;

    glfwSetCursorPosCallback(app->window.window, onCursorMoved);

    app->scene->children[0]->transform.position = Vector3(0.0f, -0.0f, -30.0);

#ifndef EMSCRIPTEN
    app->window.Tick([]() { tick(); });
#else
    emscripten_set_main_loop(tick, 0, true);
#endif

    return 0;
}