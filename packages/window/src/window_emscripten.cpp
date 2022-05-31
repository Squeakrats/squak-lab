#include "window.h"
#include "utility.h"
#include <emscripten.h>
#include <glr.h>
#include <set>

namespace window {

std::set<uint64_t> keys{};

EM_BOOL onKeyDown(int eventType, const EmscriptenKeyboardEvent* keyEvent, void* userData) {
    keys.insert(keyEvent->which);
}

EM_BOOL onKeyUp(int eventType, const EmscriptenKeyboardEvent* keyEvent, void* userData) {
    keys.erase(keyEvent->which);
}

class Window : public IWindow {
public:
    virtual void Poll() override {
       // nothing to do
    }

    virtual void Flush() override {
        // nothing to do
    }

    virtual bool ShouldClose() override {
        return false;
    }

    virtual bool GetKey(uint8_t key) override {
        return keys.find(key) != keys.end();
    }
};

std::shared_ptr<IWindow> Create(std::string name, uint32_t width, uint32_t height) {
    EmscriptenWebGLContextAttributes attributes;
    emscripten_webgl_init_context_attributes(&attributes);

    emscripten_set_canvas_element_size("#canvas", width, height);
    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE ctx = emscripten_webgl_create_context("#canvas", &attributes);

    emscripten_webgl_make_context_current(ctx);
    glViewport(0, 0, width, height);

    emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, false, onKeyDown);
    emscripten_set_keyup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, false, onKeyUp);

    return std::make_shared<Window>();
}

};
