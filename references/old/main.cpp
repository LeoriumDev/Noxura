#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#include "utils/Dependencies.hpp"
#include "controls/AppDelegate.hpp"

int main() {
    NS::AutoreleasePool* autoreleasePool = NS::AutoreleasePool::alloc()->init();

    AppDelegate appDelegate;
    NS::Application* app = NS::Application::sharedApplication();
    app->setDelegate(&appDelegate);
    app->run();

    autoreleasePool->release();

    return 0;
}
