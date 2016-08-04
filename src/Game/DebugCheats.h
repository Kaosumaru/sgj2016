#ifndef BHDEBUGCHEATS
#define BHDEBUGCHEATS
#include<memory>
#include "Scene/Sprites/MXSpriteScene.h"
#include "Scene/Managers/MXSceneManager.h"
#include "Utils/MXUtils.h"

namespace BH
{
    struct CheatObject
    {
        virtual ~CheatObject() {}
    };

    std::shared_ptr<CheatObject> CreateCheats();
    
}


#endif
