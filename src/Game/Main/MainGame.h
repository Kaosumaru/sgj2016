#ifndef BHMAINGAME
#define BHMAINGAME
#include<memory>
#include "Scene/Sprites/MXSpriteScene.h"
#include "Scene/Managers/MXSceneStackManager.h"
#include "Utils/MXUtils.h"
#include "Collision/Quadtree/MXQuadtree.h"
#include "Utils/MXSignalizingVariable.h"

namespace bs2 = boost::signals2;

namespace BH
{
    
    class MainGame : public MX::FullscreenDisplayScene, public bs2::trackable, public ScopeSingleton<MainGame>
    {
    public:
        MainGame();

        void Run() override;

        auto& obstaclesArea() { return _obstaclesArea; }

        SignalizingVariable<int> points = 0;
        SignalizingVariable<int> time = 0;
        SignalizingVariable<int> combo = 0;
    protected:
        void onExit();

        std::shared_ptr<MX::Collision::LayeredArea> _obstaclesArea;
        std::shared_ptr<struct CheatObject> _cheats;
    };
    

    struct GameInit
    {
        static void Initialize();
    };
}


#endif
