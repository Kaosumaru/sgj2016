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

namespace bs2 = boost::signals2;

using namespace MX;
using namespace BH;
using namespace std;

MainGame::MainGame() : DisplaySceneTimer(MX::Window::current().display()->size())
{
    _cheats = CreateCheats();
    Context<BaseGraphicScene>::SetCurrent(*this);


    MX::Window::current().keyboard()->on_specific_key_down[ci::app::KeyEvent::KEY_ESCAPE].connect(boost::bind(&MainGame::onExit, this));
}

void MainGame::onExit()
{
    SpriteSceneStackManager::manager_of(this)->PopScene(std::make_shared<MoveBitmapTransition>(false));
}

