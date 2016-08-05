#ifndef BHMAINGAME
#define BHMAINGAME
#include<memory>
#include "Scene/Sprites/MXSpriteScene.h"
#include "Scene/Managers/MXSceneStackManager.h"
#include "Utils/MXUtils.h"
#include "Game/Model/Game.h"

namespace bs2 = boost::signals2;

namespace BH
{
    
    class MainGame : public MX::DisplaySceneTimer, public bs2::trackable
    {
    public:
        MainGame();

        void Run() override;
    protected:
        void onExit();

        std::shared_ptr<struct CheatObject> _cheats;
        Game::pointer _game = std::make_shared<Game>();
    };
    
}


#endif
