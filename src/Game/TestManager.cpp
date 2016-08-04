#include "TestManager.h"
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

TestManager::TestManager() : DisplaySceneTimer(MX::Window::current().display()->size())
{
    _cheats = CreateCheats();
	_visibility = -1;

	MX::Window::current().keyboard()->on_specific_key_down[ci::app::KeyEvent::KEY_SPACE].connect(boost::bind(&TestManager::reloadScripts, this));
	MX::Window::current().keyboard()->on_specific_key_down[ci::app::KeyEvent::KEY_r].connect(boost::bind(&TestManager::clearReloadScripts, this));

	reloadScripts();

    Context<BaseGraphicScene>::SetCurrent(*this);
}

void TestManager::AddSomething()
{
#if 0
	std::shared_ptr<SpriteActor> actor = Script::valueOf("Game.Resources.Explosion.ExplosionTest").to_object<SpriteActor>();
	if (!actor)
		return;
	auto clone = actor->clone();
	clone->geometry.position = point;
#endif
}


void TestManager::clearReloadScripts()
{
	MX::Resources::get().Clear();
	reloadScripts();
}

void TestManager::reloadScripts()
{
    GameInitializer::ReloadScripts();

    Clear();

    ScriptObjectString script("Test.Scene");
    std::list<SpriteActorPtr> arr;
    script.load_property(arr, "Actors");

    for (auto &actor : arr)
        AddActor(actor);

}




void TestManager::Draw(float x, float y)
{
	DisplaySceneTimer::Draw(x, y);
}



