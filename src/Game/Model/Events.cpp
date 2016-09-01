#include "Events.h"
#include "Application/MXApp.h"
#include "Game/Resources/MXPaths.h"
#include "Game/Resources/MXResources.h"
#include "Script/MXScript.h"
#include "Script/MXScriptObject.h"
#include "Application/MXWindow.h"
#include "Utils/MXLine.h"
#include "Utils/MXQuad.h"
#include <iostream>

#include "Widgets/MXWidget.h"
#include "Widgets/Layouters/MXStackWidget.h"
#include "Widgets/MXLabel.h"
#include "Widgets/Controllers/MXListController.h"
#include "Widgets/Layouters/MXSimpleLayouters.h"
#include "Widgets/Layouters/MXScriptLayouters.h"

#include <boost/format.hpp>

#include "Utils/MXDebugGUIManager.h"
#include "Devices/MXKeyboard.h"


#include "Game/GameInitializer.h"
#include "Widgets/Animations/MXAnimations.h"
#include "Game/Model/Game.h"
#include "Script/MXScriptClassParser.h"

using namespace MX;


class YesResponse : public Game::Response
{
public:
    YesResponse(MX::LScriptObject& script) : Response(script)
    {
        if (_text.empty())
            _text = MX::loc("Text.Yes");
    }
};

class NoResponse : public Game::Response
{
public:
    NoResponse(MX::LScriptObject& script) : Response(script)
    {
        if (_text.empty())
            _text = MX::loc("Text.No");
    }
};


class GameGenericEvent : public Event
{
public:
    using Event::Event;

    void Do() override
    {
        if (!Context<Game::Game>::isCurrent())
            return;
        auto &game = Context<Game::Game>::current();
        onDo(game);
    }

protected:
    virtual void onDo(Game::Game &game)
    {

    }
};

class GameGainEvent : public GameGenericEvent
{
public:
    GameGainEvent(const std::string& objectName) : GameGenericEvent(objectName)
    {
        load_property(_name, "Stat");
        load_property(_amount, "Amount");
    }
protected:
    void onDo(Game::Game &game)
    {
        auto &stat = game.stats().stat(_name);
        if (!stat)
        {
            assert(false);
            return;
        }
        stat->Gain(_amount);
    }

    std::string _name;
    float _amount = 0.0f;
};

class GameTextEvent : public GameGenericEvent
{
public:
    GameTextEvent(const std::string& objectName) : GameGenericEvent(objectName)
    {
        load_property_child(_text, "Text");
    }
protected:
    void onDo(Game::Game &game)
    {
        game.onText(_text);
    }
    std::wstring _text;
};

class GameDeckAddEventEvent : public GameGenericEvent
{
public:
    GameDeckAddEventEvent(const std::string& objectName) : GameGenericEvent(objectName)
    {
        load_property_child(_event, "Event");
    }
protected:
    void onDo(Game::Game &game)
    {
        game.eventsDeck().AddEvent(_event);
    }
    std::shared_ptr<Game::GameEvent> _event;
};

class GameDeckMixEventsEvent : public GameGenericEvent
{
public:
    GameDeckMixEventsEvent(const std::string& objectName) : GameGenericEvent(objectName)
    {
        load_property_children(_events, "Events");
        assert(!_events.empty());
    }
protected:
    void onDo(Game::Game &game)
    {
        game.eventsDeck().MixEvents(_events);
    }
    Game::EventsDeck::Container _events;
};

class GameDeckMixRandomEventEvent : public GameDeckMixEventsEvent
{
public:
    using GameDeckMixEventsEvent::GameDeckMixEventsEvent;
protected:
    void onDo(Game::Game &game)
    {
        auto event = Random::randomFrom(_events);
        game.eventsDeck().MixEvent(event);
    }
    Game::EventsDeck::Container _events;
};

class GameDeckAddRandomEventEvent : public GameDeckMixEventsEvent
{
public:
    using GameDeckMixEventsEvent::GameDeckMixEventsEvent;
protected:
    void onDo(Game::Game &game)
    {
        auto event = Random::randomFrom(_events);
        game.eventsDeck().AddEvent(event);
    }
    Game::EventsDeck::Container _events;
};

class GameDeckRemoveTagEvent : public GameGenericEvent
{
public:
    GameDeckRemoveTagEvent(const std::string& objectName) : GameGenericEvent(objectName)
    {
        load_property_child(_tag, "Tag");
    }
protected:
    void onDo(Game::Game &game)
    {
        game.eventsDeck().RemoveEventsWithTag(_tag);
    }
    std::string _tag;
};

class GameOverEvent : public GameGenericEvent
{
public:
    using GameGenericEvent::GameGenericEvent;
protected:
    void onDo(Game::Game &game)
    {
        game.GameOver();
    }
};


class GameAddRuleEvent : public GameGenericEvent
{
public:
    GameAddRuleEvent(const std::string& objectName) : GameGenericEvent(objectName)
    {
        load_property_child(_event, "Rule");
    }
protected:
    void onDo(Game::Game &game)
    {
        game.AddRule(_event);
    }
    std::shared_ptr<MX::Event> _event;
};

void Game::EventsInit::Init()
{
    ScriptClassParser::AddCreator(L"Game.Response.Yes", new OutsideScriptClassCreatorContructor<YesResponse>());
    ScriptClassParser::AddCreator(L"Game.Response.No", new OutsideScriptClassCreatorContructor<NoResponse>());


    ScriptClassParser::AddCreator(L"Game.Gain", new DefaultClassCreatorContructor<GameGainEvent>());
    ScriptClassParser::AddCreator(L"Game.Text", new DefaultClassCreatorContructor<GameTextEvent>());
    ScriptClassParser::AddCreator(L"Game.AddRule", new DefaultClassCreatorContructor<GameAddRuleEvent>());
    ScriptClassParser::AddCreator(L"Game.Deck.Add", new DefaultClassCreatorContructor<GameDeckAddEventEvent>());
    ScriptClassParser::AddCreator(L"Game.Deck.Mix", new DefaultClassCreatorContructor<GameDeckMixEventsEvent>());
    ScriptClassParser::AddCreator(L"Game.Deck.Mix.RandomFrom", new DefaultClassCreatorContructor<GameDeckMixRandomEventEvent>());
    ScriptClassParser::AddCreator(L"Game.Deck.Add.RandomFrom", new DefaultClassCreatorContructor<GameDeckAddRandomEventEvent>());
    ScriptClassParser::AddCreator(L"Game.Deck.RemoveAllWithTag", new DefaultClassCreatorContructor<GameDeckRemoveTagEvent>());

    ScriptClassParser::AddCreator(L"Game.Over", new DefaultClassCreatorContructor<GameOverEvent>());
}