#ifndef BHGAME
#define BHGAME
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
        }


    protected:
        std::wstring _text;
        std::vector<Response::pointer> _responses;
        Response::pointer              _tieResponse;
    };
    
}


#endif
