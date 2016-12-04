#include "MainGame.h"
#include "Game/Resources/Paths.h"
#include "Game/Resources/Resources.h"
#include "Script/Script.h"
#include "Script/ScriptObject.h"
#include "Application/Window.h"

#include "Devices/Mouse.h"
#include "Devices/Keyboard.h"

#include "Script/ScriptClassParser.h"


using namespace MX;
using namespace BH;
using namespace std;

MainGame::MainGame() : DisplaySceneTimer(MX::Window::current().size())
{
#if WIP
    _cheats = CreateCheats();
#endif
    Context<BaseGraphicScene>::SetCurrent(*this);


   // MX::Window::current().keyboard()->on_specific_key_down[ci::app::KeyEvent::KEY_ESCAPE].connect(boost::bind(&MainGame::onExit, this));
}

void MainGame::onExit()
{
    SpriteSceneStackManager::manager_of(this)->PopScene(std::make_shared<MoveBitmapTransition>(false));
}
