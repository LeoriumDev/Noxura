#include "AppDelegate.hpp"

AppDelegate::~AppDelegate() {
    mtkView->release();
    window->release();
    device->release();
    delete viewDelegate;
}

NS::Menu* AppDelegate::createMenuBar() {
    using NS::StringEncoding::UTF8StringEncoding;

    NS::Menu* mainMenu = NS::Menu::alloc()->init();
    NS::MenuItem* appMenuItem = NS::MenuItem::alloc()->init();
    NS::Menu* appMenu = NS::Menu::alloc()->init(NS::String::string("Venturers", UTF8StringEncoding));

    NS::String* appName = NS::RunningApplication::currentApplication()->localizedName();
    NS::String* quitItemName = NS::String::string("Quit ", UTF8StringEncoding)->stringByAppendingString(appName);
    SEL quitCb = NS::MenuItem::registerActionCallback( "appQuit", [](void*,SEL,const NS::Object* sender){
        auto app = NS::Application::sharedApplication();
        app->terminate(sender);
    } );

    NS::MenuItem* appQuitItem = appMenu->addItem(quitItemName, quitCb, NS::String::string("q", UTF8StringEncoding ) );
    appQuitItem->setKeyEquivalentModifierMask(NS::EventModifierFlagCommand );
    appMenuItem->setSubmenu(appMenu );

    NS::MenuItem* windowMenuItem = NS::MenuItem::alloc()->init();
    NS::Menu* windowMenu = NS::Menu::alloc()->init(NS::String::string("File", UTF8StringEncoding ) );

    SEL closeWindowCb = NS::MenuItem::registerActionCallback( "windowClose", [](void*, SEL, const NS::Object*){
        auto pApp = NS::Application::sharedApplication();
        pApp->windows()->object< NS::Window >(0)->close();
    } );
    NS::MenuItem* closeWindowItem = windowMenu->addItem(NS::String::string("Close Window", UTF8StringEncoding ), closeWindowCb, NS::String::string("w", UTF8StringEncoding ) );
    closeWindowItem->setKeyEquivalentModifierMask(NS::EventModifierFlagCommand );

    windowMenuItem->setSubmenu(windowMenu );

    mainMenu->addItem(appMenuItem);
    mainMenu->addItem(windowMenuItem);

    appMenuItem->release();
    windowMenuItem->release();
    appMenu->release();
    windowMenu->release();

    return mainMenu->autorelease();
}

void AppDelegate::applicationWillFinishLaunching(NS::Notification* notification) {
    NS::Menu* menu = createMenuBar();
    auto* app = reinterpret_cast<NS::Application*>(notification->object());
    app->setMainMenu(menu);
    app->setActivationPolicy(NS::ActivationPolicy::ActivationPolicyRegular);
}

void AppDelegate::applicationDidFinishLaunching(NS::Notification* notification) {
    CGRect frame = (CGRect){{100.0, 100.0},
                              {640.0, 480.0}};

    window = NS::Window::alloc()->init(
        frame,
        NS::WindowStyleMaskClosable | NS::WindowStyleMaskTitled,
        NS::BackingStoreBuffered,
        false);

    device = MTL::CreateSystemDefaultDevice();

    mtkView = MTK::View::alloc()->init(frame, device);
    mtkView->setColorPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);
    mtkView->setClearColor(MTL::ClearColor::Make(0.0, 0.0, 0.0, 1.0)); // Set black background

    viewDelegate = new ViewDelegate(device);
    mtkView->setDelegate(viewDelegate);

    window->setContentView(mtkView);
    window->setTitle(NS::String::string("Venturers", NS::StringEncoding::UTF8StringEncoding));
    window->makeKeyAndOrderFront(nullptr);

    auto* app = reinterpret_cast<NS::Application*>(notification->object());
    app->activateIgnoringOtherApps(true);
}

bool AppDelegate::applicationShouldTerminateAfterLastWindowClosed
    (NS::Application* sender) {
    return true;
}