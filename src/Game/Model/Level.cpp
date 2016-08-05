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
#include "Rule.h"
#include <numeric>

namespace bs2 = boost::signals2;
using namespace BH;

bool Selector::Move(Direction direction)
{
    if (direction == Selector::Direction::None)
    {
        _lastDirection = direction;
        return false;
    }
        

    static const float speed = 0.10f;

    if (!_moveCooldown.Tick())
        return false;

    auto delta = deltaFromDirection(direction);
    auto new_position = pos() + delta;

    auto &level = Context<Level>::current();
    if (!level.containsPosition(new_position))
        return false;

    position = new_position;

    if (_lastDirection == Selector::Direction::None)
        _moveCooldown.Start(speed*2);
    else
        _moveCooldown.Start(speed);

    _lastDirection = direction;
    return true;
}

Level::Level()
{
    _rules = std::make_shared<ActionList>();
    _rules->Add(std::make_shared<GravityRule>());
    _rules->Add(std::make_shared<Match3Rule>());

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

void Level::DestroyGem(const glm::ivec2& pos)
{
    auto gem = at(pos);
    if (!gem)
        return;
    gem->Destroy();
    at(pos) = nullptr;
}

void Level::RandomizeAll()
{
    for (int x = 0; x < _width; x++)
    for (int y = 0; y < _height; y++)
    {
        InsertRandomNonExplodingGem({ x, y });
    } 
}

void Level::Update()
{
    _rules->UseAll();
}

void Level::InsertRandomNonExplodingGem(glm::ivec2 pos)
{
    static std::vector<int> allColors;
    if (allColors.empty())
    {
        allColors.resize(Gem::maxType);
        std::iota(allColors.begin(), allColors.end(), 0);
    }

    std::vector<int> availableColors = allColors;



    static std::vector<std::pair<int,int>> deltas = 
    {
        {-2, -1},
        {-1, 1},
        {1, 2}
    };

    auto compareGems = [&](const Gem::pointer &g1, const Gem::pointer &g2)
    {
        if (!g1 || !g2)
            return;
        if (g1->type() != g2->type())
            return;
        
        auto &vec = availableColors;
        auto color = g1->type();
        vec.erase(std::remove(vec.begin(), vec.end(), color), vec.end());
    };

    {
        for (auto &x : deltas)
        {
            auto gem1 = at(pos + glm::ivec2{ x.first, 0});
            auto gem2 = at(pos + glm::ivec2{ x.second, 0 });
            compareGems(gem1, gem2);
        }
    }

    {
        for (auto &y : deltas)
        {
            auto gem1 = at(pos + glm::ivec2{ 0, y.first });
            auto gem2 = at(pos + glm::ivec2{ 0, y.second });
            compareGems(gem1, gem2);
        }
    }

    assert(!availableColors.empty());
    auto color = Random::randomFrom(availableColors);
    auto gem = std::make_shared<Gem>(color);
    InsertGem(pos, gem);
}