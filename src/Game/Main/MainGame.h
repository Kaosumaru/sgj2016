#ifndef BHMAINGAME
#define BHMAINGAME
#include<memory>
#include "Scene/Sprites/MXSpriteScene.h"
#include "Scene/Managers/MXSceneStackManager.h"
#include "Utils/MXUtils.h"
#include "Game/Model/Game.h"
#include "Widgets/Layouters/MXScriptLayouters.h"

namespace bs2 = boost::signals2;

namespace BH
{
    
    class MainGame : public MX::DisplaySceneTimer, public bs2::trackable
    {
    public:
        MainGame(int players);

        void Run() override;
    protected:
        void PlayGameMusic();
        void onGameWon(int r);
        void onExit();

        std::shared_ptr<MX::Widgets::ScriptLayouterWidget> _bg;
        std::shared_ptr<struct CheatObject> _cheats;
        Game::pointer _game;
    };
    
}


#endif
