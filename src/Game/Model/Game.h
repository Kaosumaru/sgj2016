#ifndef BHGAME
#define BHGAME
#include<memory>
#include "Scene/Sprites/MXSpriteScene.h"
#include "Scene/Managers/MXSceneStackManager.h"
#include "Utils/MXUtils.h"
#include "Scene/Script/MXEvent.h"
#include "Stat.h"
#include "Events.h"

namespace bs2 = boost::signals2;

namespace Game
{
    class Player
    {
    public:

    protected:

    };
 

    class EventsDeck
    {
    public:
        using Container = std::vector<GameEvent::pointer>;

        EventsDeck(MX::LScriptObject& script)
        {
            std::vector<Container> events;
            script.load_property(events, "EventsLists");

            for (auto &evs : events)
                AddEvents(evs);
            Shuffle();
        }

        GameEvent::pointer nextEvent()
        {
            if (_events.empty())
                return nullptr;
            auto event = _events.back();
            _events.pop_back();
            return event;
        }

        void AddEvents(const Container& c)
        {
            _events.insert(_events.end(), c.begin(), c.end());
        }

        void AddEvent(const GameEvent::pointer& e)
        {
            assert(e);
            _events.push_back(e);
        }

        void MixEvent(const GameEvent::pointer& e)
        {
            AddEvent(e);
            Shuffle();
        }

        void MixEvents(const Container& c)
        {
            AddEvents(c);
            Shuffle();
        }

        void Shuffle()
        {
            std::shuffle(_events.begin(), _events.end(), Random::_rgn);
        }

        void RemoveEventsWithTag(const std::string& tag)
        {
            auto it = std::remove_if(_events.begin(), _events.end(), [&](auto& item) { return item->haveTag(tag); });
            _events.erase(it, _events.end());
        }
    protected:
        Container _events;
    };

    class Game
    {
    public:
        Game(MX::LScriptObject& script) : _stats(script), _eventsDeck(script)
        {
            Context<Game>::SetCurrent(*this);
            script.load_property(_rules, "Rules");
        }

        void nextEvent()
        {
            if (_gameOver)
            {
                return;
            }
            if (_currentEvent)
            {
                assert(false);
                return;
            }
                
            _currentEvent = _eventsDeck.nextEvent();
            if (!_currentEvent)
            {
                GameOver();
                return;
            }

            _currentEventConnection = _currentEvent->onDone.connect([&]() { eventDone(); });
            onNextEvent(_currentEvent);
        }

        MX::Signal<void(const GameEvent::pointer&)> onNextEvent;
        MX::Signal<void(const std::wstring& text)> onText;
        MX::Signal<void(void)> onEventDone;
        MX::Signal<void(void)> onGameOver;

        auto& stats() { return _stats; }
        auto& eventsDeck() { return _eventsDeck; }
        auto& currentEvent() { return _currentEvent; }

        void GameOver()
        {
            _gameOver = true;
            onGameOver();
        }

        void AddRule(const std::shared_ptr<MX::Event>& event)
        {
            _rules.AddEvent(event);
        }
    protected:
        void eventDone()
        {
            _currentEventConnection->disconnect();
            _currentEvent = nullptr;
            _rules.Do();
            onEventDone();
        }

        Stats _stats;
        EventsDeck _eventsDeck;

        std::shared_ptr<MX::Connection> _currentEventConnection;
        GameEvent::pointer _currentEvent;
        MX::EventHolder   _rules;
        bool              _gameOver = false;
    };
    
}


#endif
