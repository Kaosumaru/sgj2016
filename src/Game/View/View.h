#ifndef BHVIEW
#define BHVIEW
#include<memory>
#include "Scene/Sprites/MXSpriteScene.h"
#include "Scene/Managers/MXSceneStackManager.h"
#include "Utils/MXUtils.h"
#include "Game/Model/Level.h"
#include "Widgets/Layouters/MXScriptLayouters.h"

namespace bs2 = boost::signals2;

namespace BH
{
    
    class LevelView : public MX::Widgets::ScriptLayouterWidget
    {
    public:
        LevelView(const Level::pointer& level);

        void Run() override;
    protected:
        void onNewLevel();

        Level::pointer _level;
    };

    class GemView : public MX::Widgets::ScriptLayouterWidget
    {
    public:
        GemView(const Gem::pointer& gem);

        static auto from(const Gem::pointer& gem)
        {
            return std::make_shared<GemView>(gem);
        }

        static MX::Vector2 positionFromGem(const Gem::pointer& gem);
    protected:
        void OnMoving(glm::ivec2 newPos, glm::ivec2 );

        Gem::pointer _gem;
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
