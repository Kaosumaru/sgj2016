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

bool Controller::UseAction(int index)
{
    auto &player = Context<Player>::current();

    return player.actions().UseAction(index);

}

class KeyboardController : public Controller
{
public:
    KeyboardController()
    {
        _directionKeys[ci::app::KeyEvent::KEY_UP] = Selector::Direction::Up;
        _directionKeys[ci::app::KeyEvent::KEY_DOWN] = Selector::Direction::Down;
        _directionKeys[ci::app::KeyEvent::KEY_LEFT] = Selector::Direction::Left;
        _directionKeys[ci::app::KeyEvent::KEY_RIGHT] = Selector::Direction::Right;

        _actionKeys.push_back(ci::app::KeyEvent::KEY_z);
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
        int index = 0;
        for (auto actionKey : _actionKeys)
        {
            if (MX::Window::current().keyboard()->key(actionKey))
                UseAction(index);
            index++;
        }
    }

    std::map<int, Selector::Direction> _directionKeys;
    std::vector<int> _actionKeys;
};


Player::Player()
{
    _controller = std::make_shared<KeyboardController>();

    _actions.Add(std::make_shared<SwapGemsAction>());
}

void Player::Update()
{
    auto g1 = Context<Player>::Lock(this);
    auto g12 = Context<Level>::Lock(_level);

    if (!_controller->wantsToUseAction())
        _level->selector()->Move(_controller->wantsDirection());
    _controller->Update();
}