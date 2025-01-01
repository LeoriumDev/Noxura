#pragma once

#include "../views/Renderer.hpp"

class ViewDelegate : public MTK::ViewDelegate {

public:
    ViewDelegate(MTL::Device* device);
    ~ViewDelegate() override;
    void drawInMTKView(MTK::View* view) override;

private:
    Renderer* renderer;

};