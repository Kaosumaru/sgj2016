#ifndef BHVIEW
#define BHVIEW
#include<memory>
#include "Scene/Sprites/MXSpriteScene.h"
#include "Scene/Managers/MXSceneStackManager.h"
#include "Utils/MXUtils.h"
#include "Game/Model/Level.h"
#include "Game/Model/PLayer.h"
#include "Widgets/Layouters/MXScriptLayouters.h"

namespace bs2 = boost::signals2;

namespace BH
{
    class ActionsView : public MX::Widgets::ScriptLayouterWidget
    {
    public:
        ActionsView(const Player::pointer& player);
        Player::pointer _player;
    };

    
    class LevelView : public MX::Widgets::ScriptLayouterWidget
    {
    public:
        LevelView(const Player::pointer& player, const Level::pointer& level);

        void Run() override;
        void Draw(float x, float y) override;
    protected:
        void onNewLevel();

        Level::pointer _level;
        Player::pointer _player;
    };



    class SelectorView : public MX::Widgets::ScriptLayouterWidget
    {
    public:
        SelectorView(const Selector::pointer& selector);
        MX::Vector2 positionFromSelector();
    protected:
        Selector::pointer _selector;
    };
    
}


#endif
