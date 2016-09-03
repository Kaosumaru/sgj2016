#ifndef BHMODEL
#define BHMODEL
#include<memory>
#include<map>
#include "Script/MXPropertyLoaders.h"
#include "Scene/Sprites/MXSpriteScene.h"
#include "Scene/Managers/MXSceneStackManager.h"
#include "Utils/MXUtils.h"
#include "Script/MXScriptObject.h"
#include "Game/ControlScheme/MXControlScheme.h"

namespace bs2 = boost::signals2;

namespace Stepmania
{

    class PlayerControlSchema : public MX::Game::ControlScheme
    {
    public:
        using MX::Game::ControlScheme::ControlScheme;

        void SetupForPlayer(int number);

        MX::Game::Action save{ this };
        MX::Game::ActionList<MX::Game::Action, 4> createKey{ this };
        MX::Game::ActionList<MX::Game::Action, 4> tapKey{ this };
    };


    class Game
    {
    public:
        Game()
        {
            _controls.SetupForPlayer(0);
        }

        void Start()
        {

        }

        void Run()
        {
            _controls.Run();
            time = MX::Time::Timer::current().total_seconds();
        }

      


        float tolerance = 1.0f;
        SignalizingVariable<int> points = 0;
        SignalizingVariable<float> time = 0.0f;
        SignalizingVariable<int> combo = 0;


    protected:
        Stepmania::PlayerControlSchema _controls;
    };

}


#endif
