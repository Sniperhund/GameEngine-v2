#pragma once

#include "../UILayer.h"

namespace GameEngine
{
    class SceneView : public UILayer
    {
        void Start() override;

        void Update() override;
    };
}