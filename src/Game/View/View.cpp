#include "View.h"
#include "Game/Model/Rule.h"

namespace bs2 = boost::signals2;

using namespace MX;
using namespace BH;
using namespace std;

LevelView::LevelView(const Level::pointer& level)
{
    _level = level;
    SetLayouter("Game.Level.Layouter");

    auto selectorView = std::make_shared<SelectorView>(_level->selector());
    AddNamedWidget("Selector", selectorView);

    onNewLevel();

    _level->onCreatedGem.connect([&]
    (const Gem::pointer& gem) 
    {
        if (!gem)
            return;
        auto gemView = GemView::from(gem);
        AddNamedWidget("Gem", gemView);
    });
}

void LevelView::Run()
{
    int scroll = _level->movementRule()->cooldownTImer().percent() * 64.0f;
    SetVerticalScroll(scroll);
    MX::Widgets::ScriptLayouterWidget::Run();
}

void LevelView::onNewLevel()
{
    for (auto& gem : _level->gems())
    {
        if (!gem)
            continue;

        auto gemView = GemView::from(gem);
        AddNamedWidget("Gem", gemView);
    }

}

GemView::GemView(const Gem::pointer& gem)
{
    _gem = gem;
    SetLayouter("Game.Gem.Layouter");
    SetPosition(positionFromGem(gem));

    properties().SetValue("GemType", _gem->type());

    _gem->pos().onValueChanged.connect(
        [&](const glm::ivec2 &newPos, const glm::ivec2 &oldPos)
        {
            OnMoving(newPos, oldPos);
        }
    );

    _gem->onDestroyed.connect(
        [&]()
        {
            Unlink();
        }
    );
}

void GemView::OnMoving(glm::ivec2 newPos, glm::ivec2 oldPos)
{
    SetPosition(positionFromGem(_gem));
}

MX::Vector2 GemView::positionFromGem(const Gem::pointer& gem)
{
    return { gem->position().x * 64.0f, gem->position().y * 64.0f };
}

SelectorView::SelectorView(const Selector::pointer& selector)
{
    _selector = selector;
    SetLayouter("Game.Selector.Layouter");
    SetPosition(positionFromSelector());

    _selector->position.onValueChanged.connect([&](auto&...)
    {
        SetPosition(positionFromSelector());
    });
}

MX::Vector2 SelectorView::positionFromSelector()
{
    int x = _selector->pos().x;
    int y = _selector->pos().y;
    return { x * 64.0f, y * 64.0f };
}