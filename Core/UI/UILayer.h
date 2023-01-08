#pragma once

#include <imgui.h>
#include <string>

namespace GameEngine
{
    class UILayer
    {
    protected:
        bool removePadding = false;
    public:
        void _Update();

        void _Start();

        bool active = false;
        std::string title = "Default name";

        virtual ~UILayer() = default;

        virtual void Start()
        {
        }

        virtual void Update()
        {
        }
    };
}
