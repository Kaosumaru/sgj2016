#include "Model.h"
#include "Game/Resources/MXPaths.h"
#include "Game/Resources/MXResources.h"
#include "Script/MXScript.h"
#include "Script/MXScriptObject.h"
#include "Application/MXWindow.h"
#include "Utils/MXLine.h"
#include "Utils/MXQuad.h"
#include "Script/Class/MXScriptSoundClass.h"
#include "Devices/MXKeyboard.h"

#include "Game/Resources/MXPaths.h"
#include "Game/Resources/MXResources.h"

using namespace Stepmania;

void PlayerControlSchema::SetupForPlayer(int number)
{
    using namespace MX::Game;

    if (number == 0)
    {
        tapKey.bindKey(0, ci::app::KeyEvent::KEY_z);
        tapKey.bindKey(1, ci::app::KeyEvent::KEY_x);
        tapKey.bindKey(2, ci::app::KeyEvent::KEY_c);
        tapKey.bindKey(3, ci::app::KeyEvent::KEY_v);
    }
}

void TrackInfo::Start()
{
    MX::Sound::StreamManager::get().SetCurrent(MX::Resources::get().loadStream(_trackFile));
}