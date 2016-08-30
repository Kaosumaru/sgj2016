#ifndef BHPLAYER
#define BHPLAYER
#include<memory>
#include "Scene/Sprites/MXSpriteScene.h"
#include "Scene/Managers/MXSceneManager.h"
#include "Utils/MXUtils.h"
#include "Action.h"
#include "Level.h"
#include "Mana.h"
#include "Game/ControlScheme/MXControlScheme.h"

namespace BH
{

    class PlayerControlSchema : public MX::Game::ControlScheme
    {
    public:
        using MX::Game::ControlScheme::ControlScheme;

        void SetupForPlayer(int number);

        MX::Game::TargetDirection<glm::ivec2> direction{ this };
        MX::Game::ActionList<MX::Game::Action, 4> useSkill{ this };
    };

    class Controller
    {
    public:
        using pointer = std::shared_ptr<Controller>;

        virtual Selector::Direction wantsDirection() { return Selector::Direction::None; }
        auto wantsDelta()
        {
            return Selector::deltaFromDirection(wantsDirection());
        }

        virtual void Update() {};

        virtual bool wantsToUseAction() { return false; };
        bool UseAction(int index);


    protected:

    };

    class Player
    {
    public:
        using pointer = std::shared_ptr<Player>;

        Player(int number);

        auto &actions() { return _actions; }
        auto &level() { return _level; }
        //auto &controller() { return _controller; }
        auto &schema() { return _controlSchema; }
        auto &stats() { return _stats; }

        auto selectorPosition()
        {
            return _level->selector()->pos();
        }

        void Update();

        SignalizingVariable<bool> _lost = false;

        int number() { return _number; }
    protected:
        int                 _number = 0;
        Stats               _stats;
        //Controller::pointer _controller;
        
        Level::pointer _level = std::make_shared<Level>();
        ActionList _actions;
        MX::FunctorsQueue _queue;
        PlayerControlSchema _controlSchema{ &_queue };
    };
}


#endif
