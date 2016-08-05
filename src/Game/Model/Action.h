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
        Action(float cooldown = 0.0f)
        {
            _cooldown = cooldown;
        }

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
            if (_cooldown == 0.0f)
            {
                return onDo();
            }

            if (!_cooldownTimer.Tick())
                return false;
            if (onDo())
            {
                _cooldownTimer.Start(_cooldown);
                return true;
            }
            return false;
        }

    protected:
        virtual bool onDo()
        {
            return true;
        }

        float _cooldown;
        MX::Time::ManualStopWatchAbsolute   _cooldownTimer;
    };

    class SwapGemsAction : public Action
    {
    public:
        bool onDo() override
        {
            auto pos1 = selectorPosition();
            glm::ivec2 dir = wantsDelta();
            auto pos2 = pos1 + dir;

            if (!levelContainsPosition(pos1) || !levelContainsPosition(pos2))
                return false;

            if (wantsDirection() == Selector::Direction::None)
                return false;

            auto gem1 = level().at(pos1);
            auto gem2 = level().at(pos2);

            if (gem1 && !gem1->canBeMovedByPlayer())
                return false;
            if (gem2 && !gem2->canBeMovedByPlayer())
                return false;

            //dont allow to swap up/down with empty spaces
            {
                if (wantsDirection() == Selector::Direction::Up || wantsDirection() == Selector::Direction::Down)
                {
                    if (!gem1 || !gem2)
                        return false;
                }
            }


            level().SwapGems(pos1, pos2);

            return true;
        }
    };

    class DestroyGemAction : public Action
    {
    public:
        bool onDo() override
        {
            auto pos = selectorPosition();
            if (!levelContainsPosition(pos))
                return false;

            level().DestroyGem(pos);
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

        void UseAll()
        {
            for (auto& action : _list)
                action->Do();
        }
    protected:
        std::vector<Action::pointer> _list;
        MX::Time::ManualStopWatchAbsolute   _actionCooldown;
    };
}


#endif
