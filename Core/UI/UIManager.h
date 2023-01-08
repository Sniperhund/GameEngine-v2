#pragma once
#include <vector>

#include "UILayer.h"

namespace GameEngine
{
    class UIManager
    {
        static std::vector<UILayer*> m_Layers;
    public:
        static void Init();
        static void Update();

        static void AddUILayer(UILayer* layer);
    };
}
