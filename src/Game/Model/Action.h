#ifndef BHACTION
#define BHACTION
#include<memory>
#include<vector>
#include "Scene/Sprites/MXSpriteScene.h"
#include "Scene/Managers/MXSceneManager.h"
#include "Utils/MXUtils.h"
#include "Level.h"

namespace BH
{
    class Player;
    class Action
    {
    public:
        using pointer = std::shared_ptr<Action>;

        glm::ivec2 selectorPosition();
        bool levelContainsPosition(const glm::ivec2& pos);
        auto &level()
        {
            return Context<Level>::current();
        }

        Selector::Direction wantsDirection();
        glm::ivec2 wantsDelta();

        virtual bool Do() 
        {
            return true;
        }
    };

    class SwapGemsAction : public Action
    {
    public:
        bool Do() override
        {
            auto pos1 = selectorPosition();
            glm::ivec2 dir = wantsDelta();
            auto pos2 = pos1 + dir;

            if (!levelContainsPosition(pos1) || !levelContainsPosition(pos2))
                return false;

            if (wantsDirection() == Selector::Direction::None)
                return false;

            level().SwapGems(pos1, pos2);

            return true;
        }
    };

    class ActionList
    {
    public:
        void Add(const Action::pointer& action)
        {
            _list.push_back(action);
        }

        auto& list() { return _list; }

        bool UseAction(int index);
    protected:
        std::vector<Action::pointer> _list;
        MX::Time::ManualStopWatchAbsolute   _actionCooldown;
    };
}


#endif
