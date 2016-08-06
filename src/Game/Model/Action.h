#ifndef BHACTION
#define BHACTION
#include<memory>
#include<vector>
#include "Scene/Sprites/MXSpriteScene.h"
#include "Scene/Managers/MXSceneManager.h"
#include "Utils/MXUtils.h"
#include "Level.h"
#include "Mana.h"

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

        void SetManaSource(const Mana::pointer& manaSource)
        {
            _manaSource = manaSource;
        }

        void SetManaSource(int i);

        using pointer = std::shared_ptr<Action>;

        glm::ivec2 selectorPosition();
        bool levelContainsPosition(const glm::ivec2& pos);
        void destroyedGems(int color, int size);

        void reportPlayerLost();
        auto &level()
        {
            return Context<Level>::current();
        }

        float costProgress()
        {
            if (!_manaSource || _manaCost == 0.0f)
                return 1.0f;
            float p = _manaSource->_current / _manaCost;
            if (p > 1.0f)
                p = 1.0f;
            return p;
        }

        Selector::Direction wantsDirection();
        glm::ivec2 wantsDelta();

        virtual bool Do() 
        {
            if (_manaSource && !_manaSource->Pay(_manaCost))
                return false;

            if (!_cooldownTimer.Tick())
                return false;
            if (onDo())
            {
                if (_cooldown != 0.0f)
                    _cooldownTimer.Start(_cooldown);
                return true;
            }
            return false;
        }

        auto& cooldownTImer() { return _cooldownTimer; }


    protected:
        virtual bool onDo()
        {
            return true;
        }

        Mana::pointer _manaSource;
        float _manaCost = 0.0f;
        float _cooldown;
        MX::Time::ManualStopWatchAbsolute   _cooldownTimer;
    };

    class SwapGemsAction : public Action
    {
    public:
        SwapGemsAction() : Action(0.1f) {}

        bool Do()
        {
            {
                auto pos1 = selectorPosition();
                auto direction = wantsDirection();

                if (pos1 != _lastPosition || direction != _lastDirection)
                {
                    if (onDo())
                    {
                        _cooldownTimer.Start(_cooldown);
                        return true;
                    }
                    return false;
                }
            }

            return Action::Do();
        }

        bool onDo() override
        {
            auto pos1 = selectorPosition();
            glm::ivec2 dir = wantsDelta();
            auto pos2 = pos1 + dir;

            if (!levelContainsPosition(pos1) || !levelContainsPosition(pos2))
                return false;

            auto direction = wantsDirection();

            if (direction == Selector::Direction::None)
                return false;


            auto gem1 = level().at(pos1);
            auto gem2 = level().at(pos2);

            if (gem1 && !gem1->canBeMovedByPlayer())
                return false;
            if (gem2 && !gem2->canBeMovedByPlayer())
                return false;

            //dont allow to swap up/down with empty spaces
            {
                if (direction == Selector::Direction::Up || direction == Selector::Direction::Down)
                {
                    if (!gem1 || !gem2)
                        return false;
                }
            }


            level().SwapGems(pos1, pos2);

            _lastDirection = direction;
            _lastPosition = pos1;

            return true;
        }

        glm::ivec2 _lastPosition = { -1,-1 };
        Selector::Direction _lastDirection = Selector::Direction::None;
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

    class FireballAction : public Action
    {
    public:
        FireballAction()
        {
            _manaCost = 7;
            SetManaSource(2);
        }

        bool onDo() override
        {
            auto pos = selectorPosition();

            for(int x = -1; x < 2; x ++)
            for (int y = -1; y < 2; y++)
            {
                glm::ivec2 delta = { x, y };
                auto p = pos + delta;

                if (!levelContainsPosition(p))
                    continue;
                level().DestroyGem(p);
            }
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
