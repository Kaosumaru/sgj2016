#include "MainGame.h"
#include "Game/Resources/MXPaths.h"
#include "Game/Resources/MXResources.h"
#include "Script/MXScript.h"
#include "Script/MXScriptObject.h"
#include "Application/MXWindow.h"
#include "Utils/MXLine.h"
#include "Utils/MXQuad.h"
#include "Script/Class/MXScriptSoundClass.h"


#include "HTML/MXHTMLRendererCairo.h"
#include "HTML/MXFTFont.h"


#include "Devices/MXMouse.h"
#include "Devices/MXKeyboard.h"
#include "Application/MXWindow.h"
#include "Graphic/MXDisplay.h"

#include "Script/MXScriptClassParser.h"

#include "Game/GameInitializer.h"
#include "Game/DebugCheats.h"

#include "Widgets/MXWidget.h"
#include "Widgets/Layouters/MXStackWidget.h"
#include "Widgets/MXLabel.h"
#include "Widgets/Controllers/MXListController.h"
#include "Widgets/Layouters/MXSimpleLayouters.h"
#include "Widgets/Layouters/MXScriptLayouters.h"

#include "Game/View/View.h"

namespace bs2 = boost::signals2;

using namespace MX;
using namespace BH;
using namespace std;

MainGame::MainGame(int players) : DisplaySceneTimer(MX::Window::current().display()->size())
{
    _game = std::make_shared<Game>(players);
    _cheats = CreateCheats();
    Context<BaseGraphicScene>::SetCurrent(*this);
    
    auto bg = MX::make_shared<MX::Widgets::ScriptLayouterWidget>();
    _bg = bg;
    {
        bg->AddStrategy(MX::make_shared<MX::Strategies::FillInParent>());
        bg->SetLayouter("Game.Layouter");
        AddActor(bg);
    }

    if (players > 0)
    {
        auto player = _game->players()[0];
        auto levelView = std::make_shared<LevelView>(player->level());
        bg->AddNamedWidget("Player1.Level", levelView);

        auto actionsView = std::make_shared<ActionsView>(player);
        bg->AddNamedWidget("Player1.Actions", actionsView);
    }

    if (players > 1)
    {
        auto player = _game->players()[1];
        auto levelView = std::make_shared<LevelView>(player->level());
        bg->AddNamedWidget("Player2.Level", levelView);

        auto actionsView = std::make_shared<ActionsView>(player);
        bg->AddNamedWidget("Player2.Actions", actionsView);
    }

    _game->onGameWon.connect([&](int r) { onGameWon(r); });
    MX::Window::current().keyboard()->on_specific_key_down[ci::app::KeyEvent::KEY_ESCAPE].connect(boost::bind(&MainGame::onExit, this));
}

void MainGame::Run()
{
    if (_game)
        _game->Update();
    MX::DisplaySceneTimer::Run();
}

void MainGame::onGameWon(int r)
{
    std::string path;
    if (r == 0)
        path = "Game.Victory.Screen.Player1.Drawer";
    else if (r == 1)
        path = "Game.Victory.Screen.Player2.Drawer";
    else
        path = "Game.Victory.Screen.Tie.Drawer";

    auto widget = std::make_shared<MX::Widgets::Widget>();
    auto slotSetDrawer = MX::Widgets::DrawerManager::drawer(path);
    widget->SetDrawer(slotSetDrawer);
    _bg->AddNamedWidget("Victory", widget);
}


void MainGame::onExit()
{
    SpriteSceneStackManager::manager_of(this)->PopScene(std::make_shared<MoveBitmapTransition>(false));
}

