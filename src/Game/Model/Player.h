#ifndef BHPLAYER
#define BHPLAYER
#include<memory>
#include "Scene/Sprites/MXSpriteScene.h"
#include "Scene/Managers/MXSceneManager.h"
#include "Utils/MXUtils.h"
#include "Action.h"
#include "Level.h"

namespace BH
{
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


    };

    class Player
    {
    public:
        using pointer = std::shared_ptr<Player>;

        Player();

        auto &actions() { return _actions; }
        auto &level() { return _level; }
        auto &controller() { return _controller; }

        auto selectorPosition()
        {
            return _level->selector()->pos();
        }

        void Update();
    protected:
        

        Controller::pointer _controller;
        Level::pointer _level = std::make_shared<Level>();
        ActionList _actions;
    };
}


#endif
