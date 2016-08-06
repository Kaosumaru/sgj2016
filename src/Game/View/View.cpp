#include "View.h"
#include "Game/Model/Rule.h"

namespace bs2 = boost::signals2;

using namespace MX;
using namespace BH;
using namespace std;

class GemView : public MX::Widgets::ScriptLayouterWidget
{
public:
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

        _gem->_frozen.onValueChanged.connect([&](const bool &v, const bool & old)
        {
            properties().SetValue("Frozen", v ? 1.0f : 0.0f);
        });

    }

    static auto from(const Gem::pointer& gem)
    {
        return std::make_shared<GemView>(gem);
    }

    MX::Vector2 positionFromGem(const Gem::pointer& gem)
    {
        return{ gem->position().x * 64.0f, gem->position().y * 64.0f };
    }

    void Run() override
    {
        auto g1 = Context<Gem>::Lock(_gem);
        MX::Widgets::ScriptLayouterWidget::Run();
    }

    void Draw(float x, float y) override
    {
        auto g1 = Context<Gem>::Lock(_gem);
        MX::Widgets::ScriptLayouterWidget::Draw(x, y);
    }

protected:

    void OnMoving(glm::ivec2 newPos, glm::ivec2 oldPos)
    {
        SetPosition(positionFromGem(_gem));
    }

    Gem::pointer _gem;
};

class ActionView : public MX::Widgets::ScriptLayouterWidget
{
public:
    ActionView(const Action::pointer& action)
    {
        _action = action;
        SetLayouter("Game.Action.Layouter");
    }

    void Run() override
    {
        properties().SetValue("Progress", _action->costProgress());
        MX::Widgets::ScriptLayouterWidget::Run();
    }

protected:
    Action::pointer _action;
};

ActionsView::ActionsView(const Player::pointer& player)
{
    _player = player;
    SetLayouter("Game.Actions.Layouter");

    bool first = true;
    for (auto& action : player->actions().list())
    {
        if (first)
        {
            first = false;
            continue;
        }

        auto view = std::make_shared<ActionView>(action);
        AddNamedWidget("Action", view);
    }
}


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