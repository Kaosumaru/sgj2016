#include "GUIManager.h"
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
#include "Game/Main/MainGame.h"
#include "Widgets/Animations/MXAnimations.h"
#include "Game/Model/Model.h"

namespace bs2 = boost::signals2;

using namespace MX;
using namespace BH;
using namespace std;



class LogField : public MX::Widgets::ScriptLayouterWidget
{
public:
    class ScrollToLast : public MX::Widgets::Strategy::Strategy
    {
    public:
        ScrollToLast(float speed)
        {
            _speed = speed;
        }

        bool runnable() override { return true; }
        bool Run()
        {
            if (!_widget->linked())
                return true;

            bool shouldScroll = false;
            _widget->for_each_child([&](const std::shared_ptr<Widget>& widget)
            {
                float size = widget->dimensions().y;
                float childY = widget->geometry.position.y;
                float parentY = _widget->geometry.position.y;
                if (!size)
                    return;
                if (childY + size >= parentY)
                {
                    if (childY + size >= parentY + _widget->dimensions().y - _widget->margins().bottom)
                        shouldScroll = true;
                    return;
                }
                   
                _widget->SetVerticalScroll(_widget->scroll().y - size);
                widget->Unlink();
            });

            if (!shouldScroll)
                return true;

            float scrollY = _widget->scroll().y;
            scrollY += _speed;
            _widget->SetVerticalScroll(scrollY);

            return true;
        }
    protected:
        Time::FloatPerSecond _speed;
    };

    LogField() : _scrollToLast(60.0f)
    {
        AddStrategy(_scrollToLast);

#ifdef _DEBUG
        MX::Window::current().keyboard()->on_specific_key_down[ci::app::KeyEvent::KEY_q].connect([&]() 
        {
            AddText(L"Text lorem ipsum lala lala lala lala");
        });
#endif
    }


    void AddText(const std::wstring& text)
    {
        auto item = MX::make_shared<MX::Widgets::Label>();
        item->SetHTML(true);
        item->SetText(text);
        AddNamedWidget("Item", item);
    }
protected:
    ScrollToLast _scrollToLast;
};


class MMenuScene : public MX::FullscreenDisplayScene, public bs2::trackable
{
	std::shared_ptr<MX::Widgets::ScriptLayouterWidget> _bgLayouter;
public:
	MMenuScene()
    {
        {
            auto bg = MX::make_shared<MX::Widgets::ScriptLayouterWidget>();
            bg->AddStrategy(MX::make_shared<MX::Strategies::FillInParent>());
            bg->SetLayouter("GUI.MainMenu.Layouter");
            AddActor(bg);
            _bgLayouter = bg;
        }

        auto addButton = [&](const std::string& name)
        {
            auto wstr_name = loc(name);
            auto button = MX::make_shared<MX::Widgets::LabelButton>(wstr_name);
            _bgLayouter->AddNamedWidget(name, button);
            return button;
        };

        addButton("Button.Game")->onClicked.connect([&]() { OnGame(); });
        addButton("Button.Exit")->onClicked.connect([&]() { OnExit(); });
    }


protected:

    void OnGame()
    {
        auto game = std::make_shared<MainGame>();
        SpriteSceneStackManager::manager_of(this)->PushScene(game, std::make_shared<MoveBitmapTransition>(true));
    }

    void OnExit()
    {

    }

};


class KeyWidget : public MX::Widgets::ScriptLayouterWidget
{
public:
    KeyWidget(const Game::TrackInfo::KeyData::pointer& key)
    {
        _key = key;
        properties().SetValue("Type", (float)key->type);
    }

    void Draw(float x, float y) override
    {
        MX::Widgets::ScriptLayouterWidget::Draw(x, y);
    }
protected:
    Game::TrackInfo::KeyData::pointer _key;
};

class KeysContainer : public MX::Widgets::ScriptLayouterWidget
{
public:
    float sizeOfSecondinPixels = 64.0f;

    KeysContainer(const std::shared_ptr<Game::TrackInfo>& trackInfo)
    {
        _trackInfo = trackInfo;
        auto &tracks = trackInfo->tracks();

        for (auto& track : tracks)
        {
            for (auto& item : track)
                CreateItemFromKey(item.second);
        }
    }

    void Run() override
    {
        MX::Widgets::ScriptLayouterWidget::Run();

        float scrollY = scroll().y;
        scrollY -= _speed;
        SetVerticalScroll(scrollY);
    }

protected:
    void CreateItemFromKey(const std::shared_ptr<Game::TrackInfo::KeyData>& key)
    {
        auto itemWidget = std::make_shared<KeyWidget>(key);
        AddNamedWidget("Item", itemWidget);

        itemWidget->SetPosition(0.0f, 0.0f, 64.0f, 64.0f);

        float y = -key->time * 64.0f;
        float x = key->type * 64.0f;
        itemWidget->SetPosition(x, y);
    }

    Time::FloatPerSecond             _speed = sizeOfSecondinPixels;
    std::shared_ptr<Game::TrackInfo> _trackInfo;
};


class MGameScene : public MX::FullscreenDisplayScene, public bs2::trackable
{
    std::shared_ptr<MX::Widgets::ScriptLayouterWidget> _bgLayouter;
public:
    MGameScene()
    {
        {
            auto bg = MX::make_shared<MX::Widgets::ScriptLayouterWidget>();
            bg->AddStrategy(MX::make_shared<MX::Strategies::FillInParent>());
            bg->SetLayouter("GUI.Game.Layouter");
            AddActor(bg);
            _bgLayouter = bg;
        }
        loadGame();
    }


protected:
    void loadGame()
    {
        ScriptObjectString script("TestTrack");
        auto trackInfo = std::make_shared<Game::TrackInfo>(script);
        auto game = std::make_shared<Game::Game>(trackInfo);
        prepareForGame(game);
    }

    void prepareForGame(const std::shared_ptr<Game::Game>& game)
    {
        auto container = MX::make_shared<KeysContainer>(game->trackInfo());
        _bgLayouter->AddNamedWidget("Container", container);
    }
};


GuiManager::GuiManager()
{
	reloadScripts();

#ifndef MX_GAME_RELEASE
    MX::Window::current().keyboard()->on_specific_key_down[ci::app::KeyEvent::KEY_u].connect(boost::bind(&GuiManager::reloadScripts, this));
#endif

	//PushScene(MX::make_shared<MMenuScene>());
    PushScene(MX::make_shared<MGameScene>());
}



void GuiManager::reloadScripts()
{
    GameInitializer::ReloadScripts();
}





