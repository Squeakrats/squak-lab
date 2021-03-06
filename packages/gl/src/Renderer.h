#pragma once
#include <squak/graphics/IRenderer.h>
#include "Program.h"
#include "RenderingContext.h"
#include <squak/graphics/SceneNode.h>
#include <squak/gl/glutils.h>
#include "shaders.h"
#include <map>
#include <stack>

namespace gl {

class AGeometryRenderer : public Program {
public:
  virtual ~AGeometryRenderer() {}

  AGeometryRenderer(std::string vertexSource,
                    std::string fragmentSource,
                    std::vector<std::string> attributes,
                    std::vector<std::string> uniforms)
    : Program(vertexSource, fragmentSource, attributes, uniforms) {}

  virtual void Render(Matrix4& perspective,
                      Matrix4& view,
                      Matrix4& model,
                      Mesh::Geometry& geometry) = 0;
};

enum class RenderPass { Opaque, Light };

class Renderer : public IRenderer {
private:
  RenderingContext context{};
  std::shared_ptr<AGeometryRenderer> texuredRenderer{};
  std::shared_ptr<AGeometryRenderer> solidRenderer{};

  gl::Framebuffer framebuffer{};
  Program lightProgram = Program(shaders::fullscreen::vertex,
                                 shaders::fullscreen::fragment,
                                 { "aPosition", "aTextureCoordinate" },
                                 { "uLightPosition",
                                   "uLightColor",
                                   "uDiffuseTexture",
                                   "uPositionTexture",
                                   "uNormalTexture" });
  Program ambientProgram = Program(shaders::ambient::vertex,
                                   shaders::ambient::fragment,
                                   { "aPosition", "aTextureCoordinate" },
                                   { "uAmbientLight", "uDiffuseTexture" });
  GLuint quadBuffer{};

  std::stack<Matrix4> transforms{ { Matrix4::Identity() } };

  void RenderNode(RenderPass pass, CameraNode& camera, SceneNode& node);
  void RenderLight(CameraNode& camera, Light& light);
  void RenderAmbientLight();

public:
  Renderer(uint32_t width, uint32_t height);

  virtual void Render(CameraNode& camera, SceneNode& scene) override;
};

}