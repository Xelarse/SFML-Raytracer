#include "App.h"

int main()
{
    std::unique_ptr<App> application(std::make_unique<App>());
    application->Run();
    return 0;
}