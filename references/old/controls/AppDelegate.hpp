#pragma once

#include "../Dependencies.hpp"
#include "ViewDelegate.hpp"

class AppDelegate final : public NS::ApplicationDelegate {
public:
    ~AppDelegate() override;

    NS::Menu* createMenuBar();

    void applicationWillFinishLaunching(NS::Notification* notification) override;
    void applicationDidFinishLaunching(NS::Notification* notification) override;
    bool applicationShouldTerminateAfterLastWindowClosed(NS::Application* sender) override;

private:
    NS::Window* window = nullptr;
    MTK::View* mtkView = nullptr;
    MTL::Device* device = nullptr;
    ViewDelegate* viewDelegate = nullptr;

};