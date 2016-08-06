#ifndef BHRULE
#define BHRULE
#include<memory>
#include "Scene/Sprites/MXSpriteScene.h"
#include "Scene/Managers/MXSceneManager.h"
#include "Utils/MXUtils.h"
#include "Action.h"
#include "Utils/MXSignalizingVariable.h"

namespace BH
{
    class Rule : public Action
    {
    public:
        using Action::Action;
    };

    class GravityRule : public Rule
    {
    public:
        GravityRule() : Rule(0.1f)
        {

        }
        bool onDo() override
        {
            auto &l = level();

            for (int y = level().height() - 1; y > 0; y--)
            {
                for (int x = 0; x < level().width(); x++)
                {
                    auto currentGem = l.at({ x,y });
                    auto upperGem = l.at({ x,y-1 });

                    if (currentGem || !upperGem)
                    {
                        if (upperGem)
                            upperGem->_falling = false;
                        continue;
                    }

                    //TODO check it
                    upperGem->_falling = true;
                    l.SwapGems({ x,y }, { x,y - 1 });

                }
            }

            int y = level().height() - 1;
            for (int x = 0; x < level().width(); x++)
            {
                auto currentGem = l.at({ x,y });
                if (currentGem)
                    currentGem->_falling = false;
            }

            return true;
        }
    };

    class Match3Rule : public Rule
    {
    public:

        bool onDo() override
        {
            auto &l = level();

            bool foundExplosion = false;
            auto foundRange = [&](glm::ivec2 start, glm::ivec2 end, int size, const glm::ivec2& d, int color)
            {
                if (size < 3 || color < 0)
                    return;
                foundExplosion = true;

                for (int i = 0; i < size; i++)
                {
                    l.at(start)->_exploding = true;
                    start += d;
                }
            };

            {
                glm::ivec2 horizDelta = { 1,0 };
                for (int y = 0; y < level().height(); y++)
                {
                    int prevColor = -1;
                    int rangeStart = -1;
                    for (int x = 0; x < level().width() + 1; x++)
                    {
                        auto gem = l.at({ x,y });
                        int color = gem ? gem->type() : -1;

                        if (gem && gem->_falling)
                            color = -1;

                        if (color == prevColor)
                            continue;

                        auto rangeEnd = x - 1;
                        int size = rangeEnd - rangeStart + 1;
                        foundRange({ rangeStart, y }, { rangeEnd, y }, size, horizDelta, prevColor);
                        rangeStart = x;
                        prevColor = color;
                    }
                }
            }

            {
                glm::ivec2 vertDelta = { 0,1 };
                for (int x = 0; x < level().width(); x++)
                {
                    int prevColor = -1;
                    int rangeStart = -1;
                    for (int y = 0; y < level().height() + 1; y++)
                    {
                        auto gem = l.at({ x,y });
                        int color = gem ? gem->type() : -1;

                        if (gem && gem->_falling)
                            color = -1;

                        if (color == prevColor)
                            continue;

                        auto rangeEnd = y - 1;
                        int size = rangeEnd - rangeStart + 1;
                        foundRange({ x, rangeStart }, { x, rangeEnd }, size, vertDelta, prevColor);
                        rangeStart = y;
                        prevColor = color;
                    }
                }
            }


            for (auto &gem : l.gems())
            {
                if (gem && gem->_exploding)
                    l.DestroyGem(gem->pos());
            }
            return true;
        }
    };


    class LevelMovement : public Rule
    {
    public:
        LevelMovement() : Rule(3.0f)
        {

        }


        bool onDo() override
        {
            if (first)
            {
                first = false;
                return true;
            }

            MoveUp();
            return true;
        }

        void MoveUp()
        {
            auto &l = level();

            {
                int y = 1;
                for (int x = 0; x < level().width(); x++)
                {
                    glm::ivec2 pos = { x, y };
                    auto gem = l.at(pos);
                    if (!gem)
                        continue;

                    reportPlayerLost();
                }
            }


            for (int y = 1; y < level().height(); y++)
            {
                for (int x = 0; x < level().width(); x++)
                {
                    glm::ivec2 pos = { x, y };
                    glm::ivec2 upper_pos = { x, y-1 };
                    l.SwapGems(pos, upper_pos);
                }
            }
            FillRow(level().height()-1);
            l.selector()->ForceMove(Selector::Direction::Up);
        }

        void FillRow(int row)
        {
            auto &l = level();
            for (int x = 0; x < level().width(); x++)
            {
                glm::ivec2 pos = { x, row };
                l.InsertRandomNonExplodingGem(pos);
            }

        }


        bool first = true;
    };
}


#endif
