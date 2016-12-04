#ifndef BHMAINGAME
#define BHMAINGAME
#include<memory>
#include "Scene/Sprites/SpriteScene.h"
#include "Scene/Managers/SceneStackManager.h"
#include "Utils/Utils.h"



namespace BH
{
    
    class MainGame : public MX::DisplaySceneTimer
    {
    public:
        MainGame();

    protected:
        void onExit();
#if 0
        std::shared_ptr<struct CheatObject> _cheats;
#endif
    };
    
}


#endif
