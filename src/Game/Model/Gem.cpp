#include "Gem.h"
#include "Game/Resources/MXPaths.h"
#include "Game/Resources/MXResources.h"
#include "Script/MXScript.h"
#include "Script/MXScriptObject.h"
#include "Application/MXWindow.h"
#include "Utils/MXLine.h"
#include "Utils/MXQuad.h"
#include "Script/Class/MXScriptSoundClass.h"


#include "HTML/MXHTMLRendererCairo.h"
#include "Game.h"

namespace bs2 = boost::signals2;
using namespace BH;

Gem::Gem(int type)
{
    _type = type;
}


Gem::pointer Gem::random()
{
    int randomType = Random::randomInt(0, maxType);
    return std::make_shared<Gem>(randomType);
}