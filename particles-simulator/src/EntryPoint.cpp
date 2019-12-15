#include "pch.h"
#include "Application.h"

int main()
{
    auto app = Application::create();
    app->run();
    delete app;
    return 0;
}