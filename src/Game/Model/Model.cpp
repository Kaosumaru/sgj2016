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
        save.bindKey(ci::app::KeyEvent::KEY_q);

        tapKey.bindKey(0, ci::app::KeyEvent::KEY_z);
        tapKey.bindKey(1, ci::app::KeyEvent::KEY_x);
        tapKey.bindKey(2, ci::app::KeyEvent::KEY_c);
        tapKey.bindKey(3, ci::app::KeyEvent::KEY_v);

        createKey.bindKey(0, ci::app::KeyEvent::KEY_a);
        createKey.bindKey(1, ci::app::KeyEvent::KEY_s);
        createKey.bindKey(2, ci::app::KeyEvent::KEY_d);
        createKey.bindKey(3, ci::app::KeyEvent::KEY_f);
    }
}

