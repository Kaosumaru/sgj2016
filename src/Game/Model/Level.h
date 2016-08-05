#ifndef BHLEVEL
#define BHLEVEL
#include<memory>
#include "Scene/Sprites/MXSpriteScene.h"
#include "Scene/Managers/MXSceneManager.h"
#include "Utils/MXUtils.h"
#include "Gem.h"
#include "Utils/MXSignalizingVariable.h"

namespace BH
{
    class ActionList;
    class Selector
    {
    public:
        using pointer = std::shared_ptr<Selector>;

        enum class Direction
        {
            None,
            Up,
            Down,
            Left,
            Right
        };

        static auto deltaFromDirection(Direction direction)
        {
            static std::vector<glm::ivec2> deltas =
            {
                { 0 ,  0 },
                { 0 , -1 },
                { 0 ,  1 },
                { -1,  0 },
                { 1 ,  0 }
            };
            return deltas[(int)direction];
        }

        auto pos() { return (glm::ivec2) position; }
        SignalizingVariable<glm::ivec2> position;

        bool Move(Direction direction);

    protected:
        Direction _lastDirection = Direction::None;
        MX::Time::ManualStopWatchAbsolute   _moveCooldown;
    };

    class Level
    {
    public:
        using pointer = std::shared_ptr<Level>;

        Level();

        bool InsertGem(const glm::ivec2& pos, const Gem::pointer& gem);
        bool SwapGems(const glm::ivec2& pos1, const glm::ivec2& pos2);
        void DestroyGem(const glm::ivec2& pos);

        void RandomizeAll();

        void Update();

        auto& at(const glm::ivec2& pos)
        {
            static Gem::pointer gem;
            if (!containsPosition(pos))
                return gem;
            return _gems[pos.x + pos.y * _width];
        }

        auto& gems() { return _gems; }
        auto& selector() { return _selector; }

        auto& width() { return _width; }
        auto& height() { return _height; }

        bool containsPosition(const glm::ivec2& pos)
        {
            if (pos.x < 0 || pos.y < 0)
                return false;
            if (pos.x >= width() || pos.y >= height())
                return false;
            return true;
        }
    protected:
        void InsertRandomNonExplodingGem(glm::ivec2 pos);

        int _width = 6;
        int _height = 12;
        std::vector<Gem::pointer> _gems;
        Selector::pointer _selector = std::make_shared<Selector>();
        std::shared_ptr<ActionList> _rules;
    };
}


#endif
