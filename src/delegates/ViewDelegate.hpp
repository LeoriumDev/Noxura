#pragma once

#include "utils/Dependencies.hpp"
#include "renderer/Renderer.hpp"

class ViewDelegate final : public MTK::ViewDelegate {
  public:
    explicit ViewDelegate(MTL::Device* device);
    ~ViewDelegate() override;
    void drawInMTKView(MTK::View* view) override;

  private:
    Renderer* renderer;
};