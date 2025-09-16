// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// THIS FILE SHOULD NOT NEED TO BE TOUCHED.  ~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once
using namespace IonixEngine;

#include "Architecture/Application.h"

namespace ClientApp
{
    IonixEngine::Application* CreateApplication();
}

int main(int argc, char** argv)
{
    Application* app = ClientApp::CreateApplication();
    app->Run();

    delete app;
    return 0;
}
