#ifndef BHSTAT
#define BHSTAT
#include<memory>
#include "Scene/Sprites/MXSpriteScene.h"
#include "Scene/Managers/MXSceneManager.h"
#include "Utils/MXUtils.h"
#include "Utils/MXSignalizingVariable.h"
#include "Script/MXScriptObject.h"

namespace Game
{
    class Stat
    {
    public:
        using pointer = std::shared_ptr<Stat>;

        Stat(float current = 0, float max = 20)
        {
            _current = current;
            _max = max;
        }

        SignalizingVariable<float> _current = 0.0f;
        SignalizingVariable<float> _max;

        void Gain(float amount)
        {
            auto new_current = _current + amount;
            if (new_current < 0)
                new_current = 0;
            if (new_current > _max)
                new_current = _max;
            _current = new_current;
        }

        bool Pay(float amount)
        {
            if (_current < amount)
                return false;
            _current -= amount;
            return true;
        }
    };

    class Stats
    {
    public:
        Stats(MX::LScriptObject& script)
        {
            std::map<std::string, std::pair<float, float>> stats;
            script.load_property(stats, "Stats");
            for (auto &item : stats)
                AddStat(item.first, item.second.first, item.second.second);
        }

        auto &stats() { return _stats; }
       
        auto stat(const std::string& name)
        {
            auto it = _stats.find(name);
            return it == _stats.end() ? nullptr : it->second;
        }

        float amountOf(const std::string& name)
        {
            auto s = stat(name);
            return s ? s->_current.directValueAccess() : 0.0f;
        }

        MX::Signal<void(void)> statChanged;
    protected:
        void AddStat(const std::string& name, float current, float max)
        {
            auto stat = std::make_shared<Stat>(current, max);
            _stats[name] = stat;

            stat->_current.onValueChanged.connect([&](auto ...) { statChanged(); });
            stat->_max.onValueChanged.connect([&](auto ...) { statChanged(); });
        }

        std::map<std::string, Stat::pointer> _stats;
    };
}


#endif
