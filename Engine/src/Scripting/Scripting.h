#pragma once

namespace IonixEngine
{
    class Scripting
    {
    public:
        // Singleton
        static Scripting& Get();

    private:
        // Singleton Instance
        static Scripting* s_Instance;
    };
}

