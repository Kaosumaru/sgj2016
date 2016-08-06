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


using namespace BH;
namespace bs2 = boost::signals2;

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

    float cooldown = 0.05f;
    if (!_actionCooldown.Tick())
        return false;

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

