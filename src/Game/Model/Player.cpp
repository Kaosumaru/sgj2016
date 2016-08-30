#include "Player.h"
#include "Game/Resources/MXPaths.h"
#include "Game/Resources/MXResources.h"
#include "Script/MXScript.h"
#include "Script/MXScriptObject.h"
#include "Application/MXWindow.h"
#include "Utils/MXLine.h"
#include "Utils/MXQuad.h"
#include "Script/Class/MXScriptSoundClass.h"
#include "Devices/MXKeyboard.h"



#include "HTML/MXHTMLRendererCairo.h"
#include "Game.h"
using namespace BH;
namespace bs2 = boost::signals2;


void PlayerControlSchema::SetupForPlayer(int number)
{
    using namespace MX::Game;
    if (number == 0)
    {
        direction.bindKeys(ci::app::KeyEvent::KEY_w, ci::app::KeyEvent::KEY_s, ci::app::KeyEvent::KEY_a, ci::app::KeyEvent::KEY_d);

        useSkill.bindKey(0, ci::app::KeyEvent::KEY_v);
        useSkill.bindKey(1, ci::app::KeyEvent::KEY_b);
        useSkill.bindKey(2, ci::app::KeyEvent::KEY_n);
    }
    else if (number == 1)
    {
        direction.bindKeys(ci::app::KeyEvent::KEY_UP, ci::app::KeyEvent::KEY_DOWN, ci::app::KeyEvent::KEY_LEFT, ci::app::KeyEvent::KEY_RIGHT);

        useSkill.bindKey(0, ci::app::KeyEvent::KEY_COMMA);
        useSkill.bindKey(1, ci::app::KeyEvent::KEY_PERIOD);
        useSkill.bindKey(2, ci::app::KeyEvent::KEY_SLASH);
    }
    else
    {
        direction.bindKeys(ci::app::KeyEvent::KEY_UP, ci::app::KeyEvent::KEY_DOWN, ci::app::KeyEvent::KEY_LEFT, ci::app::KeyEvent::KEY_RIGHT);

        useSkill.bindKey(0, ci::app::KeyEvent::KEY_z);
        useSkill.bindKey(1, ci::app::KeyEvent::KEY_x);
        useSkill.bindKey(2, ci::app::KeyEvent::KEY_c);
        useSkill.bindKey(2, ci::app::KeyEvent::KEY_n);
    }
}



bool Controller::UseAction(int index)
{
    auto &player = Context<Player>::current();

    return player.actions().UseAction(index);

}

class KeyboardController : public Controller
{
public:
    KeyboardController(int number)
    {
        if (number == 0)
        {
            _directionKeys[ci::app::KeyEvent::KEY_w] = Selector::Direction::Up;
            _directionKeys[ci::app::KeyEvent::KEY_s] = Selector::Direction::Down;
            _directionKeys[ci::app::KeyEvent::KEY_a] = Selector::Direction::Left;
            _directionKeys[ci::app::KeyEvent::KEY_d] = Selector::Direction::Right;

            _actionKeys.push_back(ci::app::KeyEvent::KEY_v);
            _actionKeys.push_back(ci::app::KeyEvent::KEY_b);
            _actionKeys.push_back(ci::app::KeyEvent::KEY_n);
        }

        if (number == 1)
        {
            _directionKeys[ci::app::KeyEvent::KEY_UP] = Selector::Direction::Up;
            _directionKeys[ci::app::KeyEvent::KEY_DOWN] = Selector::Direction::Down;
            _directionKeys[ci::app::KeyEvent::KEY_LEFT] = Selector::Direction::Left;
            _directionKeys[ci::app::KeyEvent::KEY_RIGHT] = Selector::Direction::Right;

            _actionKeys.push_back(ci::app::KeyEvent::KEY_COMMA);
            _actionKeys.push_back(ci::app::KeyEvent::KEY_PERIOD);
            _actionKeys.push_back(ci::app::KeyEvent::KEY_SLASH);
        }


        if (number == 3)
        {
            _directionKeys[ci::app::KeyEvent::KEY_UP] = Selector::Direction::Up;
            _directionKeys[ci::app::KeyEvent::KEY_DOWN] = Selector::Direction::Down;
            _directionKeys[ci::app::KeyEvent::KEY_LEFT] = Selector::Direction::Left;
            _directionKeys[ci::app::KeyEvent::KEY_RIGHT] = Selector::Direction::Right;

            _actionKeys.push_back(ci::app::KeyEvent::KEY_z);
            _actionKeys.push_back(ci::app::KeyEvent::KEY_x);
            _actionKeys.push_back(ci::app::KeyEvent::KEY_c);
            _actionKeys.push_back(ci::app::KeyEvent::KEY_v);
        }

    }

    Selector::Direction wantsDirection() override
    { 
        for (auto &direction : _directionKeys)
            if (MX::Window::current().keyboard()->key(direction.first))
                return direction.second;

        return Selector::Direction::None; 
    }

    bool wantsToUseAction() override
    {
        for (auto actionKey : _actionKeys)
        {
            if (MX::Window::current().keyboard()->key(actionKey))
                return true;
        }
        return false;
    }

    void Update() override
    {
        auto dir = wantsDirection();

        int index = 0;
        for (auto actionKey : _actionKeys)
        {
            if (MX::Window::current().keyboard()->key(actionKey))
                if (!UseAction(index))
                {
                    
                }
            index++;
        }

    }

    std::map<int, Selector::Direction> _directionKeys;
    std::vector<int> _actionKeys;
};


Player::Player(int number)
{
    _number = number;
    auto g1 = Context<Player>::Lock(this);
    auto g12 = Context<Level>::Lock(_level);

    _controlSchema.SetupForPlayer(number);

    _controlSchema.useSkill.onTrigger.connect([&](int index)
    {
        actions().UseAction(index);
    });

    _controlSchema.direction.target.onValueChanged.connect([&](auto &v, auto &o) 
    {
        if (!_controlSchema.useSkill.anyIsOn())
            _level->selector()->Move(v);
    });

    //_controller = std::make_shared<KeyboardController>(number);

    _actions.Add(ActionCreator::createSwap());
    _actions.Add(ActionCreator::createFireball());

    if (number < 3)
    {
        _actions.Add(ActionCreator::createFrostbolt());
        _actions.Add(ActionCreator::createGemRain());
    }
    else
    {
#ifdef _DEBUG
        //_actions.Add(ActionCreator::createGemRain());
#endif
    }
    


    //SwapGems
    //Fireball
    //Freezeball
    //Confuse
    //SlowTime - passive
}

void Player::Update()
{
    if (_lost)
        return;

    auto g1 = Context<Player>::Lock(this);
    auto g12 = Context<Level>::Lock(_level);
    _actions.Update();

    _queue.Run();
    /*
    if (!_controller->wantsToUseAction())
        _level->selector()->Move(_controller->wantsDirection());
    _controller->Update();
    */
    _level->Update();
}