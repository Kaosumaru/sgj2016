#include "GameCommands.h"
#include "Game/Resources/MXPaths.h"
#include "Game/Resources/MXResources.h"
#include "Script/MXScript.h"
#include "Script/MXScriptObject.h"
#include "Application/MXWindow.h"
#include "Utils/MXLine.h"
#include "Utils/MXQuad.h"
#include "Script/Class/MXScriptSoundClass.h"
#include "Scene/Generators/MXActorDecorator.h"

#include "HTML/MXHTMLRendererCairo.h"
#include "HTML/MXFTFont.h"


#include "Devices/MXMouse.h"
#include "Devices/MXKeyboard.h"
#include "Application/MXWindow.h"
#include "Graphic/MXDisplay.h"

#include "Script/MXScriptClassParser.h"

#include "Game/GameInitializer.h"
#include "Game/DebugCheats.h"
#include "Scene/Sprites/MXScriptableSpriteActor.h"

#include "Collision/Shape/MXSignalizingShape.h"


namespace bs2 = boost::signals2;

using namespace MX;
using namespace BH;
using namespace std;


class SinusoideCommand : public Command
{
public:
    SinusoideCommand(const SinusoideCommand& other) : _frequency(other._frequency), _width(other._width)
    {
    }

    SinusoideCommand(LScriptObject& script)
    {
        script.load_property(_frequency, "Frequency");
        script.load_property(_width, "Width");
    }


    bool operator () ()
    {
        //ScriptableSpriteActor::current().geometry.angle += _speed;
        return true;
    }

    Command::pointer clone() { return MX::make_shared<SinusoideCommand>(*this); }
protected:
    float _frequency = 1;
    float _width = 1;
};



void GameCommandsInit::Initialize()
{
    ScriptClassParser::AddCreator(L"Command.Sinusoide", new OutsideScriptClassCreatorContructor<SinusoideCommand>());

}

