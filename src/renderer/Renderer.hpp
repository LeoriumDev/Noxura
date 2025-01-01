#pragma once

#include "utils/Dependencies.hpp"

class Renderer {
  public:
    explicit Renderer(MTL::Device* device);
    ~Renderer();
    void buildShaders();
    void buildBuffers();
    void draw(const MTK::View* view) const;

  private:
    MTL::Device* device;
    MTL::CommandQueue* commandQueue;
    MTL::RenderPipelineState* renderPipelineState{};
    MTL::Buffer* vertexPositionsBuffer{};
    MTL::Buffer* vertexColorsBuffer{};
};