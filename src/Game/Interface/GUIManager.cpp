#include "GUIManager.h"
#include "Script/Script.h"
#include "Script/ScriptObject.h"
#include "Application/Window.h"
#include <iostream>

#include "Widgets/Widget.h"
#include "Widgets/Layouters/StackWidget.h"
#include "Widgets/Label.h"
#include "Widgets/Controllers/ListController.h"
#include "Widgets/Layouters/SimpleLayouters.h"
#include "Widgets/Layouters/ScriptLayouters.h"





using namespace MX;
using namespace BH;
using namespace std;




class MMenuScene : public MX::FullscreenDisplayScene, public MX::SignalTrackable
{
	std::shared_ptr<MX::Widgets::ScriptLayouterWidget> _bgLayouter;
public:
	MMenuScene()
    {
        {
            auto bg = std::make_shared<MX::Widgets::ScriptLayouterWidget>();
            bg->AddStrategy(std::make_shared<MX::Strategies::FillInParent>());
            bg->SetLayouter("GUI.MainMenu.Layouter");
            AddActor(bg);
            _bgLayouter = bg;
        }

        auto addButton = [&](const std::string& name)
        {
            auto wstr_name = loc(name);
            //auto button = std::make_shared<MX::Widgets::LabelButton>(wstr_name);
			auto button = std::make_shared<MX::Widgets::ButtonWidget>();
            _bgLayouter->AddNamedWidget(name, button);
            return button;
        };

        addButton("Button.Game")->onClicked.connect([&]() { OnGame(); }, this);
        addButton("Button.Exit")->onClicked.connect([&]() { OnExit(); }, this);
    }


protected:

    void OnGame()
    {
        //auto game = std::make_shared<MainGame>();
        //SpriteSceneStackManager::manager_of(this)->PushScene(game, std::make_shared<MoveBitmapTransition>(true));
    }

    void OnExit()
    {

    }

};




GuiManager::GuiManager()
{

#ifndef MX_GAME_RELEASE
    //MX::Window::current().keyboard()->on_specific_key_down[ci::app::KeyEvent::KEY_u].connect(boost::bind(&GuiManager::reloadScripts, this));
#endif

	PushScene(std::make_shared<MMenuScene>());
}



