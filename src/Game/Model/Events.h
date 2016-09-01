#ifndef BHEVENTS
#define BHEVENTS
#include<memory>
#include "Scene/Sprites/MXSpriteScene.h"
#include "Scene/Managers/MXSceneStackManager.h"
#include "Utils/MXUtils.h"
#include "Scene/Script/MXEvent.h"

namespace bs2 = boost::signals2;

namespace Game
{

    class Response : public MX::ScriptObject
    {
    public:
        using pointer = std::shared_ptr<Response>;
        Response(MX::LScriptObject& script) : MX::ScriptObject(script)
        {
            script.load_property(_text, "Text");
            script.load_property(_selected, "Selected");
        }

        auto& text() { return _text; }

        void Selected()
        {
            if (_selected)
                _selected->Do();
        }
    protected:
        std::wstring _text;
        std::shared_ptr<MX::Event> _selected;
    };

    class GameEvent : public MX::ScriptObject
    {
    public:
        using pointer = std::shared_ptr<GameEvent>;

        GameEvent(MX::LScriptObject& script) : MX::ScriptObject(script)
        {
            script.load_property(_text, "Text");
            script.load_property(_responses, "Responses");
            script.load_property(_tieResponse, "Response.Tie");
            script.load_property(_tags, "Tags");
        }

        auto& text() { return _text; }
        const auto& responses() { return _responses; }

        void RespondWith(const Response::pointer& response)
        {
            //TODO
            if (response)
                response->Selected();
            onDone();
        }

        auto& tags() { return _tags; }

        bool haveTag(const std::string& tag)
        {
            return _tags.find(tag) != _tags.end();
        }

        MX::Signal<void(void)> onDone;
    protected:
        std::set<std::string> _tags;
        std::wstring _text;
        std::vector<Response::pointer> _responses;
        Response::pointer              _tieResponse;
    };
    
    class EventsInit
    {
    public:
        static void Init();
    };

}


#endif
