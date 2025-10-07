#pragma once
#include "PetersTest.h"
#include "imgui.h"

namespace IonixEngine
{
     PetersTest::PetersTest(){
         Scripting::Get().Init();
         Scripting::Get().ExecuteScript("Scripts/test.lua");
         
     }
}

