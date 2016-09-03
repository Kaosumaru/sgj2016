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



}


#endif
