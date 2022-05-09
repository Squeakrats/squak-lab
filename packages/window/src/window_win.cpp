#include "window.h"
#include <windows.h>
#include <wingdi.h>
#include <winuser.h>
#define GLAD_GL_IMPLEMENTATION
#include "utility.h"
#include <glad/gl.h>

namespace window {

HMODULE glModule{};

GLADapiproc FindProcAddress(const char* name) {
  if (glModule == nullptr) {
    glModule = GetModuleHandle("opengl32.dll");
  }

  Assert(glModule != nullptr, "unable to locate opengl32");
  GLADapiproc address = (GLADapiproc)GetProcAddress(glModule, name);
  if (address != nullptr) {
    return address;
  }

  return (GLADapiproc)wglGetProcAddress(name);
}

class Window : public IWindow {
private:
  HWND window;
  bool shouldClose = false;

public:
  Window(HWND window)
    : window(window){};

  virtual void Poll() override {
    MSG message{};

    while (PeekMessage(&message, this->window, 0, 0, PM_REMOVE)) {
      if (message.message == WM_CLOSE) {
        shouldClose = true;
      } else {
        DispatchMessage(&message);
      }
    }
  }

  virtual void Flush() override { glFlush(); }

  virtual bool ShouldClose() override { return this->shouldClose; }

  virtual bool GetKey(uint8_t key) override {
    return 0x80000000 & GetAsyncKeyState(key);
  }
};

const char defaultWindowClassName[] = "Lawless Window";

std::shared_ptr<IWindow> Create(std::string name,
                                uint32_t width,
                                uint32_t height) {
  WNDCLASS windowClass{};
  windowClass.lpfnWndProc = DefWindowProc;
  windowClass.hInstance = GetModuleHandle(nullptr);
  windowClass.lpszClassName = defaultWindowClassName;
  windowClass.style = CS_OWNDC;
  Assert(RegisterClass(&windowClass), "Failed to register window class");

  HWND window = CreateWindow(defaultWindowClassName,
                             name.c_str(),
                             WS_VISIBLE | WS_SYSMENU,
                             0,
                             0,
                             width,
                             height,
                             nullptr,
                             nullptr,
                             windowClass.hInstance,
                             nullptr);

  PIXELFORMATDESCRIPTOR pixelFormatDescriptor = {
    sizeof(PIXELFORMATDESCRIPTOR),
    1,
    PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL,
    PFD_TYPE_RGBA,
    32, // color depth
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0, // RGBA bits/shift
    0, // accumulation depth
    0,
    0,
    0,
    0,  // RGBA accumulation bits
    24, // depth depth
    8,  // stencil depth
    0,  // # auxiliary buffers
    0,  // ignored
    0,  // # overlay / underlay planes?
    0,  // ignored
    0,  // transparent color
    0   // ignored
  };

  HDC deviceContext = GetDC(window);
  int pixelFormat = ChoosePixelFormat(deviceContext, &pixelFormatDescriptor);
  Assert(SetPixelFormat(deviceContext, pixelFormat, &pixelFormatDescriptor),
         "Failed to set pixel format");

  HGLRC glContext = wglCreateContext(deviceContext);
  Assert(wglMakeCurrent(deviceContext, glContext),
         "Failed to activate OpenGL context");

  gladLoadGL(FindProcAddress);

  glViewport(0, 0, width, height);

  return std::make_shared<Window>(window);
}

};
