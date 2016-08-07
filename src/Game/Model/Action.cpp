#include "Action.h"
#include "Game/Resources/MXPaths.h"
#include "Game/Resources/MXResources.h"
#include "Script/MXScript.h"
#include "Script/MXScriptObject.h"
#include "Application/MXWindow.h"
#include "Utils/MXLine.h"
#include "Utils/MXQuad.h"
#include "Script/Class/MXScriptSoundClass.h"


#include "HTML/MXHTMLRendererCairo.h"
#include "Game.h"
#include "Player.h"
#include "Widgets/MXWidget.h"
#include "Widgets/Drawers/MXDrawer.h"

using namespace BH;
namespace bs2 = boost::signals2;


Action::Action(const std::string& objectName) : MX::ScriptObjectString(objectName)
{
    load_property(_cooldown, "Cooldown");
    load_property(_manaCost, "ManaCost");
    load_property(_doEvents, "Events");
    load_property(_drawer, "Drawer");

    int manaSource = -1;
    load_property(manaSource, "ManaSource");
    if (manaSource != -1)
        SetManaSource(manaSource);
}

bool Action::Do()
{
    if (_manaSource && !_manaSource->Pay(_manaCost))
        return false;

    if (!_cooldownTimer.Tick())
        return false;
    if (onDo())
    {
        _doEvents.Do();
        if (_cooldown != 0.0f)
            _cooldownTimer.Start(_cooldown);
        return true;
    }
    return false;
}

Player& Action::enemyPlayer()
{
    auto &game = Context<Game>::current();

    auto &current = Context<Player>::current();;

    for (auto &player : game.players())
    {
        if (&current != player.get())
            return *player;
    }
    return current;
}
Level &Action::enemyLevel()
{
    return *(enemyPlayer().level());
}

void Action::SetManaSource(int i)
{
    auto &player = Context<Player>::current();
    SetManaSource(player.stats().manas()[i]);
}

glm::ivec2 Action::selectorPosition()
{
    auto &player = Context<Player>::current();
    return player.selectorPosition();
}

void Action::destroyedGems(int color, int size)
{
    auto &player = Context<Player>::current();
    player.stats().manas()[color]->onDestroyedGems(size);
}

bool Action::levelContainsPosition(const glm::ivec2& pos)
{
    auto &level = Context<Level>::current();
    return level.containsPosition(pos);
}

void Action::reportPlayerLost()
{
    auto& player = Context<Player>::current();
    player._lost = true;
}

Selector::Direction Action::wantsDirection()
{
    auto &player = Context<Player>::current();
    return player.controller()->wantsDirection();
}
glm::ivec2 Action::wantsDelta()
{
    auto &player = Context<Player>::current();
    return player.controller()->wantsDelta();
}

bool ActionList::UseAction(int index)
{
    auto &player = Context<Player>::current();

    float cooldown = 0.1f;
    //if (!_actionCooldown.Tick())
    //    return false;

    if (index < (int)list().size())
    {
        auto &action = list()[index];
        if (!action)
            return false;
        action->Do();
        _actionCooldown.Start(cooldown);
        return true;
    }
    return false;
}

class FireballAction : public Action
{
public:
    FireballAction(const std::string& objectName) : Action(objectName)
    {
    }

    bool onDo() override
    {
        auto pos = selectorPosition();

        enumerateBlocks3x3(pos, [&](auto &level, auto& pos, auto &gem)
        {
            if (gem)
                gem->QueueDestruction();
        });

        return true;
    }
};

class FrostboltAction : public Action
{
public:
    FrostboltAction(const std::string& objectName) : Action(objectName)
    {

    }

    bool onDo() override
    {
        //auto pos = selectorPosition();
        auto pos = bestPoint();
        freezeAt(pos);
        return true;
    }

    glm::ivec2 bestPoint()
    {
        int maxPoints = -1;
        glm::ivec2 res = selectorPosition();
        int count = 20;

        for (int i = 0; i < count; i++)
        {
            glm::ivec2 p = randomPosition(1);
            int points = calculatePoints(p);
            if (points <= maxPoints)
                continue;
            maxPoints = points;
            res = p;
        }
        return res;
    }

    void freezeAt(const glm::ivec2& pos)
    {
        enumerateEnemyBlocks3x3(pos, [](auto &level, auto& pos, auto &gem)
        {
            if (gem)
                gem->_frozen = true;
        });
    }

    int calculatePoints(const glm::ivec2& pos)
    {
        int points = 0;
        enumerateEnemyBlocks3x3(pos, [&](auto &level, auto& pos, auto &gem)
        {
            points += gem && !gem->_frozen ? 1 : 0;
        });
        return points;
    }
};

std::shared_ptr<Action> ActionCreator::createSwap()
{
    return std::make_shared<SwapGemsAction>("Actions.Swap");
}

std::shared_ptr<Action> ActionCreator::createFireball()
{
    return std::make_shared<FireballAction>("Actions.Fireball");
}

std::shared_ptr<Action> ActionCreator::createFrostbolt()
{
    return std::make_shared<FrostboltAction>("Actions.Frostbolt");
}