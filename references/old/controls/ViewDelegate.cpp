#include "ViewDelegate.hpp"

ViewDelegate::ViewDelegate(MTL::Device* device)
        : MTK::ViewDelegate(), renderer(new Renderer(device)) {
}

ViewDelegate::~ViewDelegate() {
    delete renderer;
}

void ViewDelegate::drawInMTKView(MTK::View* view) {
    renderer->draw(view);
}