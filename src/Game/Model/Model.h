#ifndef BHMODEL
#define BHMODEL
#include<memory>
#include<map>
#include "Scene/Sprites/MXSpriteScene.h"
#include "Scene/Managers/MXSceneStackManager.h"
#include "Utils/MXUtils.h"
#include "Script/MXScriptObject.h"

namespace bs2 = boost::signals2;

namespace Game
{

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

            void Hit()
            {
                active = false;
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

        KeyData::pointer getNearestKey(float timePoint, float threshold, int trackIndex)
        {
            auto& track = _tracks[trackIndex];
            auto it = track.lower_bound(timePoint);

            return nullptr;
        }
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
        }

        void Start()
        {

        }

        void Step()
        {

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

        SignalizingVariable<float> points = 0.0f;
        SignalizingVariable<float> time   = 0.0f;

        auto& trackInfo() { return _trackInfo; }
    protected:
        void Miss()
        {
            //TODO
        }

        void Hit(const TrackInfo::KeyData::pointer& data)
        {
            //TODO
        }

        TrackInfo::pointer _trackInfo;
    };

}


#endif
