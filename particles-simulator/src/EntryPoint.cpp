#include "pch.h"
#include "Application.h"

int main()
{
    auto app = Application::getInstance();
    app->run();
    delete app;
    return 0;
}