#include "pch.h"
#include "Application.h"

int main(int argc, char* argv[])
{
    auto app = new Application(argc, argv);
    app->run();
    delete app;
    return 0;
}