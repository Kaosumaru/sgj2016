#include "Level.h"
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

namespace bs2 = boost::signals2;
using namespace BH;

bool Selector::Move(Direction direction)
{
    if (direction == Selector::Direction::None)
        return false;

    static const float speed = 0.10f;

    if (!_moveCooldown.Tick())
        return false;

    auto delta = deltaFromDirection(direction);
    auto new_position = pos() + delta;

    auto &level = Context<Level>::current();
    if (!level.containsPosition(new_position))
        return false;

    position = new_position;
    _moveCooldown.Start(speed);

    return true;
}

Level::Level()
{
    _gems.resize(_width * _height);
    RandomizeAll();
}

bool Level::InsertGem(const glm::ivec2& pos, const Gem::pointer& gem)
{
    gem->SetPosition(pos);
    at(pos) = gem;
    return true;
}

bool Level::SwapGems(const glm::ivec2& pos1, const glm::ivec2& pos2)
{
    assert(pos1 != pos2);
    auto gem1 = at(pos1);
    auto gem2 = at(pos2);

    if (gem2)
        gem2->SetPosition(pos1);
    if (gem1)
        gem1->SetPosition(pos2);

    at(pos1) = gem2;
    at(pos2) = gem1;

    return true;
}

void Level::RandomizeAll()
{
    for (int x = 0; x < _width; x++)
    for (int y = 0; y < _height; y++)
    {
        auto gem = Gem::random();
        InsertGem({ x, y }, gem);
    }

        
}