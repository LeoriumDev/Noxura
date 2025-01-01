#include "AppDelegate.hpp"
#include "ViewDelegate.hpp"

AppDelegate::~AppDelegate() {
    view->release();
    window->release();
    device->release();
    delete viewDelegate;
}

NS::Menu* AppDelegate::createMenuBar() {
    using NS::StringEncoding::UTF8StringEncoding;

    NS::Menu* MainMenu = NS::Menu::alloc()->init();
    NS::MenuItem* appMenuItem = NS::MenuItem::alloc()->init();
    NS::Menu* appMenu = NS::Menu::alloc()->init(NS::String::string("Appname", UTF8StringEncoding));

    NS::String* appName = NS::RunningApplication::currentApplication()->localizedName();
    NS::String* quitItemName = NS::String::string("Quit ", UTF8StringEncoding)->stringByAppendingString(appName);
    SEL quitCallback = NS::MenuItem::registerActionCallback("appQuit", [](void*, SEL, const NS::Object* sender) {
        auto app = NS::Application::sharedApplication();
        app->terminate(sender);
    });

    NS::MenuItem* appQuitItem = appMenu->addItem(quitItemName, quitCallback, NS::String::string("q", UTF8StringEncoding));
    appQuitItem->setKeyEquivalentModifierMask(NS::EventModifierFlagCommand);
    appMenuItem->setSubmenu(appMenu);

    NS::MenuItem* windowMenuItem = NS::MenuItem::alloc()->init();
    NS::Menu* windowMenu = NS::Menu::alloc()->init( NS::String::string("Window", UTF8StringEncoding));

    SEL closeWindowCallback = NS::MenuItem::registerActionCallback(
        "windowClose", [](void*, SEL, const NS::Object*) {
            auto app = NS::Application::sharedApplication();
            app->windows()->object<NS::Window>(0)->close();
    });
    NS::MenuItem* closeWindowItem = windowMenu->addItem(
        NS::String::string("Close Window", UTF8StringEncoding),
        closeWindowCallback, NS::String::string("w", UTF8StringEncoding));
    closeWindowItem->setKeyEquivalentModifierMask(NS::EventModifierFlagCommand);

    windowMenuItem->setSubmenu(windowMenu);

    MainMenu->addItem(appMenuItem);
    MainMenu->addItem(windowMenuItem);

    appMenuItem->release();
    windowMenuItem->release();
    appMenu->release();
    windowMenu->release();

    return MainMenu->autorelease();
}

void AppDelegate::applicationWillFinishLaunching(NS::Notification* notification) {
    const NS::Menu* menu = createMenuBar();
    const auto app = reinterpret_cast<NS::Application*>(notification->object());
    app->setMainMenu(menu);
    app->setActivationPolicy(NS::ActivationPolicy::ActivationPolicyRegular);
}

void AppDelegate::applicationDidFinishLaunching(NS::Notification* notification)
{
    constexpr CGRect frame = {{100.0, 100.0}, {512.0, 512.0}};

    window = NS::Window::alloc()->init(
        frame,
        NS::WindowStyleMaskClosable|NS::WindowStyleMaskTitled,
        NS::BackingStoreBuffered,
        false
    );

    device = MTL::CreateSystemDefaultDevice();

    view = MTK::View::alloc()->init(frame, device);
    view->setColorPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);
    view->setClearColor(MTL::ClearColor::Make(0.0, 0.0, 0.0, 1.0 ));

    viewDelegate = new ViewDelegate(device);
    view->setDelegate(viewDelegate);

    window->setContentView(view);
    window->setTitle(NS::String::string("Noxura", NS::StringEncoding::UTF8StringEncoding));

    window->makeKeyAndOrderFront(nullptr);

    const auto app = reinterpret_cast<NS::Application*>(notification->object());
    app->activateIgnoringOtherApps(true);
}

bool AppDelegate::applicationShouldTerminateAfterLastWindowClosed(NS::Application* sender) {
    return true;
}

