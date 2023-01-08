#pragma once

#include "../UILayer.h"

namespace GameEngine
{
    class PerformanceView : public UILayer
    {
        float lastRenderTime = 0;

        void Start() override;

        void Update() override;
    };
}