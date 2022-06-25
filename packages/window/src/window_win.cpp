#include "utility.h"
#include "window.h"
#include <windows.h>
#include <wingdi.h>
#include <winuser.h>
#define GLR_IMPLEMENTATION
#include <glr.h>

namespace window {

LRESULT WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
  switch (Msg) {
    case WM_NCDESTROY:
      PostQuitMessage(0);
    default:
      return DefWindowProc(hWnd, Msg, wParam, lParam);
  }
}

class Window : public IWindow {
private:
  bool shouldClose = false;
  bool keys[256]{};

public:
  virtual void Poll() override {
    MSG message{};

    while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)) {
      if (message.message == WM_QUIT) {
        shouldClose = true;
      } else if (message.message == WM_KEYDOWN && message.wParam < 256) {
        this->keys[message.wParam] = true;
      } else if (message.message == WM_KEYUP && message.wParam < 256) {
        this->keys[message.wParam] = false;
      } else {
        DispatchMessage(&message);
      }
    }
  }

  virtual bool ShouldClose() override { return this->shouldClose; }

  virtual bool GetKey(uint8_t key) override { return this->keys[key]; }
};

const char defaultWindowClassName[] = "Lawless Window";

std::shared_ptr<IWindow> Create(std::string name,
                                uint32_t width,
                                uint32_t height) {
  WNDCLASS windowClass{};
  windowClass.lpfnWndProc = WindowProc;
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

  glrInit();

  glViewport(0, 0, width, height);

  return std::make_shared<Window>();
}

};
