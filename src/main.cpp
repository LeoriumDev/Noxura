#include "utils/Definitions.hpp"
#include "utils/Dependencies.hpp"
#include "delegates/AppDelegate.hpp"

int main() {

    NS::AutoreleasePool* autoreleasePool = NS::AutoreleasePool::alloc()->init();

    AppDelegate appDelegate;
    NS::Application* app = NS::Application::sharedApplication();
    app->setDelegate(&appDelegate);
    app->run();

    autoreleasePool->release();

    return 0;
}
