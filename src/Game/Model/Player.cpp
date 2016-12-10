#include "Player.h"

#include "Game.h"
using namespace BH;



void PlayerControlSchema::SetupForPlayer(int number)
{
    using namespace MX::Game;
    direction.SetFirstTickMultiplier(1.3f);
#if WIP
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
#endif
}

Player::Player(int number)
{
    _number = number;
    auto g1 = Context<Player>::Lock(this);
    auto g12 = Context<Level>::Lock(_level);

    _controlSchema.SetupForPlayer(number);
    _controlSchema.useSkill.whileOn([&](unsigned index) 
    {
        return [&, index]() { actions().UseAction(index); };
    });

    _controlSchema.direction.onTick.static_connect([&](auto &v) 
    {
        if (!_controlSchema.useSkill.anyIsOn())
            _level->selector()->Move(v);
    });

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

    _controlSchema.Run();
    /*
    if (!_controller->wantsToUseAction())
        _level->selector()->Move(_controller->wantsDirection());
    _controller->Update();
    */
    _level->Update();
}