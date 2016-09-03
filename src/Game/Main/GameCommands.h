#ifndef BHGAMECOMMANDS
#define BHGAMECOMMANDS
#include<memory>
#include "Scene/Sprites/MXSpriteScene.h"
#include "Scene/Managers/MXSceneStackManager.h"
#include "Utils/MXUtils.h"
#include "Collision/Quadtree/MXQuadtree.h"
#include "Utils/MXSignalizingVariable.h"
#include "Scene/Generators/MXActorFactory.h"

namespace bs2 = boost::signals2;

namespace BH
{
    
    

    struct GameCommandsInit
    {
        static void Initialize();
    };
}


#endif
