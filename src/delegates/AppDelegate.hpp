#pragma once

#include "utils/Dependencies.hpp"

class AppDelegate final : public NS::ApplicationDelegate {
  public:
    ~AppDelegate() override;

    static NS::Menu* createMenuBar();

    void applicationWillFinishLaunching(NS::Notification* notification) override;
    void applicationDidFinishLaunching(NS::Notification* notification) override;
    bool applicationShouldTerminateAfterLastWindowClosed(NS::Application* sender) override;

  private:
    NS::Window* window;
    MTK::View* view;
    MTL::Device* device;
    MTK::ViewDelegate* viewDelegate = nullptr;
};
