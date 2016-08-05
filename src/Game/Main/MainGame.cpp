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

MainGame::MainGame() : DisplaySceneTimer(MX::Window::current().display()->size())
{
    _cheats = CreateCheats();
    Context<BaseGraphicScene>::SetCurrent(*this);
    
    auto bg = MX::make_shared<MX::Widgets::ScriptLayouterWidget>();
    {
        bg->AddStrategy(MX::make_shared<MX::Strategies::FillInParent>());
        bg->SetLayouter("Game.Layouter");
        AddActor(bg);
    }

    {
        auto player = _game->players()[0];
        auto levelView = std::make_shared<LevelView>(player->level());
        bg->AddNamedWidget("Player1.Level", levelView);
    }

    MX::Window::current().keyboard()->on_specific_key_down[ci::app::KeyEvent::KEY_ESCAPE].connect(boost::bind(&MainGame::onExit, this));
}

void MainGame::Run()
{
    if (_game)
        _game->Update();
    MX::DisplaySceneTimer::Run();
}

void MainGame::onExit()
{
    SpriteSceneStackManager::manager_of(this)->PopScene(std::make_shared<MoveBitmapTransition>(false));
}

