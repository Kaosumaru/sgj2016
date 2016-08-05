#ifndef BHMAINGAME
#define BHMAINGAME
#include<memory>
#include "Scene/Sprites/MXSpriteScene.h"
#include "Scene/Managers/MXSceneStackManager.h"
#include "Utils/MXUtils.h"


namespace bs2 = boost::signals2;

namespace BH
{
    
    class MainGame : public MX::DisplaySceneTimer, public bs2::trackable
    {
    public:
        MainGame();

    protected:
        void onExit();

        std::shared_ptr<struct CheatObject> _cheats;
    };
    
}


#endif
