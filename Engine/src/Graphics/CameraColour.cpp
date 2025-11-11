#include "CameraColour.h"
#include "Architecture/Application.h"
#include <iostream>

namespace IonixEngine
{
    CameraColour::CameraColour(Camera* targetCamera)
        : camera(targetCamera)
    {
        // default color
        currentColor = { 50, 80, 200, 255 };
        Update();
    }

    void CameraColour::SetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
    {
        currentColor = { r, g, b, a };
    }

    void CameraColour::Update()
    {
        if (!camera) return;
        camera->SetBackgroundColor(
            currentColor.r,
            currentColor.g,
            currentColor.b,
            currentColor.a
        );
    }
}