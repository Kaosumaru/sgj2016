#ifndef BHMAINGAME
#define BHMAINGAME
#include<memory>
#include "Scene/Sprites/MXSpriteScene.h"
#include "Scene/Managers/MXSceneStackManager.h"
#include "Utils/MXUtils.h"
#include "Collision/Quadtree/MXQuadtree.h"
#include "Utils/MXSignalizingVariable.h"
#include "Scene/Generators/MXActorFactory.h"

namespace bs2 = boost::signals2;

namespace BH
{
    
    class MainGame : public MX::FullscreenDisplayScene, public bs2::trackable, public ScopeSingleton<MainGame>
    {
    public:
        MainGame();

        void Run() override;

        auto& obstaclesArea() { return _obstaclesArea; }
        float levelTime = 30;

        SignalizingVariable<int> points = 0;
        SignalizingVariable<int> maxPoints = 100;

        SignalizingVariable<int> time = 0;
        SignalizingVariable<int> remainingTime = levelTime;
        SignalizingVariable<int> combo = 0;

        MX::Signal<void(bool)> onGameOver;
        
        auto gameActive() { return _gameActive; }

        void GainPoints(int points);
        float progress()
        {
            if (points < 0)
                return 0.0f;
            return (float)points / (float)maxPoints;
        }
    protected:
        void onExit();

        std::shared_ptr<MX::Collision::SimplestWeakArea> _obstaclesArea;
        std::shared_ptr<struct CheatObject> _cheats;
        std::shared_ptr<MX::ActorFactory> _factory;

        void GameOver(bool win);
        bool _gameActive = true;
    };
    

    struct GameInit
    {
        static void Initialize();
    };
}


#endif
