#ifndef BHGEM
#define BHGEM
#include<memory>
#include "Scene/Sprites/MXSpriteScene.h"
#include "Scene/Managers/MXSceneManager.h"
#include "Utils/MXUtils.h"
#include "Utils/MXSignalizingVariable.h"

namespace BH
{
    class Gem
    {
    public:
        friend class Level;

        Gem(int type);
        using pointer = std::shared_ptr<Gem>;

        static pointer random();

        static const int maxType = 4;

        auto position() { return (glm::ivec2)_pos; }
        auto& pos() { return _pos; }

        auto& type() { return _type; }

        
    protected:
        void SetPosition(glm::ivec2 pos)
        {
            _pos = pos;
        }

        SignalizingVariable<glm::ivec2> _pos;
        int _type = 0;
    };
}


#endif
