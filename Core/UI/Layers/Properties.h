#pragma once

#include <memory>
#include <vector>

#include "../UILayer.h"
#include "../../Util/Entity.h"

namespace GameEngine
{
    class Properties : public UILayer
    {
    public:
        void Start() override;

        void Update() override;
    };

}
