#ifndef BHMODEL
#define BHMODEL
#include<memory>
#include<map>
#include "Script/MXPropertyLoaders.h"
#include "Scene/Sprites/MXSpriteScene.h"
#include "Scene/Managers/MXSceneStackManager.h"
#include "Utils/MXUtils.h"
#include "Script/MXScriptObject.h"
#include "Game/ControlScheme/MXControlScheme.h"

namespace bs2 = boost::signals2;

namespace Stepmania
{

    class PlayerControlSchema : public MX::Game::ControlScheme
    {
    public:
        using MX::Game::ControlScheme::ControlScheme;

        void SetupForPlayer(int number);

        MX::Game::Action save{ this };
        MX::Game::ActionList<MX::Game::Action, 4> createKey{ this };
        MX::Game::ActionList<MX::Game::Action, 4> tapKey{ this };
    };

    class TrackInfo : public MX::ScriptObject
    {
    public:
        using pointer = std::shared_ptr<TrackInfo>;
        static const int MaxKeys = 4;

        struct KeyData
        {
            using pointer = std::shared_ptr<KeyData>;

            KeyData(float time, int i) : time(time), type(i)
            {

            }

            int type = 0;
            float time = 0.0f;
            SignalizingVariable<bool> active = true;
            SignalizingVariable<bool> missed = false;

            bool Hit()
            {
                active = false;
                return true;
            }
        };

        using KeyTrack = std::map<float, KeyData::pointer>;
        using KeyTracks = std::array<KeyTrack, MaxKeys>;

        auto& track(int index)
        {
            return _tracks[index];
        }

        auto& tracks()
        {
            return _tracks;
        }

        TrackInfo(MX::LScriptObject& script)
        {
            script.load_property(_name, "Name");
            //script.load_property(_tracks, "Keys");
            script.load_property(_trackFile, "TrackFile");

            std::vector<std::vector<float>> keys;
            script.load_property(keys, "Keys");
            LoadEntries(keys);
        }

        void LoadEntries(const std::vector<std::vector<float>>& keys)
        {
            for (auto &entry : keys)
            {
                if (entry.size() < 2)
                {
                    assert(false);
                    continue;
                }

                float timePoint = entry[0];

                for (int i = 1; i < entry.size(); i++)
                {
                    int index = entry[i];
                    auto &t = track(index);
                    t[timePoint] = std::make_shared<KeyData>(timePoint, index);
                }
            }
        }

        KeyData::pointer getKeyBefore(float timePoint, int trackIndex)
        {
            auto& track = _tracks[trackIndex];
            auto it = track.lower_bound(timePoint);

            if (it == track.begin())
                return nullptr;
            it--;
            return it->second;
        }

        KeyData::pointer getNearestKey(float timePoint, float threshold, int trackIndex)
        {
            auto& track = _tracks[trackIndex];
            auto it = track.lower_bound(timePoint - threshold);

            if (it == track.end())
                return nullptr;
            

            for (; it != track.end(); it++)
            {
                if (it->first > timePoint + threshold)
                    break;
                return it->second;
            }

            return nullptr;
        }

        void CreateKey(float timePoint, int index)
        {
            timePoint = floorf(timePoint * 10) / 10;
            auto &t = track(index);
            auto data = std::make_shared<KeyData>(timePoint, index);
            t[timePoint] = data;
            onCreateKey(data);
        }

        void Save(const std::string& path);
        void Start();

        MX::Signal<void(const KeyData::pointer& key)> onCreateKey;
    protected:
        std::string _trackFile;
        std::string _name;
        KeyTracks   _tracks;
    };

    class Game
    {
    public:
        Game(const TrackInfo::pointer& trackInfo)
        {
            _trackInfo = trackInfo;
            _controls.SetupForPlayer(0);

            _controls.save.onTrigger.connect([&]()
            {
                _trackInfo->Save("c:/sgj/test.msl");
            });
            _controls.tapKey.onTrigger.connect([&](int index) 
            {
                PlayerPress(index);
            });

            _controls.createKey.onTrigger.connect([&](int index)
            {
                PlayerCreate(index);
            });
        }

        void Start()
        {
            _trackInfo->Start();
        }

        void Run()
        {
            _controls.Run();
            time = MX::Time::Timer::current().total_seconds();

            calculateLateKeys();
        }

        void PlayerPress(int keyIndex)
        {
            float threshold = 1.0f;
            auto key = _trackInfo->getNearestKey(time, threshold, keyIndex);

            if (!key)
            {
                Miss();
                return;
            }

            Hit(key);
        }

        void PlayerCreate(int keyIndex)
        {
            _trackInfo->CreateKey(time, keyIndex);
        }


        float tolerance = 1.0f;
        SignalizingVariable<int> points = 0;
        SignalizingVariable<float> time = 0.0f;
        SignalizingVariable<int> combo = 0;
        SignalizingVariable<int> lateKeys = 0;
        SignalizingVariable<int> totalMiss = 0;

        auto& trackInfo() { return _trackInfo; }
        auto& controls() { return _controls; }
    protected:
        void calculateLateKeys()
        {
            float deathLine = time - tolerance;
            for (int i = 0; i < TrackInfo::MaxKeys; i++)
            {
                auto key = _trackInfo->getKeyBefore(deathLine, i);
                if (key && key->active)
                    Miss(key);
            }
        }

        void Miss(const TrackInfo::KeyData::pointer& data = nullptr)
        {
            if (data && data->missed)
                return;

            //TODO
            if (data)
            {
                lateKeys++;
                data->missed = true;
            }
            else
                totalMiss++;

            combo = 0;
        }

        void Hit(const TrackInfo::KeyData::pointer& data)
        {
            bool wasActive = data->active;
            if (!data->Hit())
            {
                Miss();
                return;
            }
            
            if (!wasActive)
                return;

            combo++;
            points = points + combo;
            //TODO
        }

        TrackInfo::pointer _trackInfo;
        Stepmania::PlayerControlSchema _controls;
    };

}


#endif
