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


namespace bs2 = boost::signals2;

using namespace MX;
using namespace BH;
using namespace std;




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

        addButton("Button.Game")->onClicked.connect([&]() { OnGame(1); });
        addButton("Button.Game2")->onClicked.connect([&]() { OnGame(2); });
        addButton("Button.Exit")->onClicked.connect([&]() { OnExit(); });

    }

    void OnGame(int players, bool animate = true)
    {
        auto game = std::make_shared<MainGame>(players);

        if (animate)
            SpriteSceneStackManager::manager_of(this)->PushScene(game, std::make_shared<MoveBitmapTransition>(true));
        else
            SpriteSceneStackManager::manager_of(this)->PushScene(game, nullptr);
    }
protected:

    void OnExit()
    {
        MX::App::current().Quit();
    }

};




GuiManager::GuiManager()
{
	reloadScripts();

#ifndef MX_GAME_RELEASE
    MX::Window::current().keyboard()->on_specific_key_down[ci::app::KeyEvent::KEY_u].connect(boost::bind(&GuiManager::reloadScripts, this));
#endif

    auto menu = MX::make_shared<MMenuScene>();
	PushScene(menu);

#ifndef MX_GAME_RELEASE
    menu->OnGame(1, false);
#endif
}



void GuiManager::reloadScripts()
{
    GameInitializer::ReloadScripts();
}





