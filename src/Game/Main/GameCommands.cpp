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
    SinusoideCommand(const SinusoideCommand& other) : _frequency(other._frequency), _width(other._width), _coordinateX(other._coordinateX)
    {
        _timeOffset = other._timeOffset;
        offset = Random::randomRange(_timeOffset);
    }

    SinusoideCommand(LScriptObject& script)
    {
        script.load_property(_frequency, "Frequency");
        script.load_property(_width, "Width");
        script.load_property(_timeOffset, "TimeOffset");
		script.load_property(_coordinateX, "XCoordinate");

        offset = Random::randomRange(_timeOffset);
    }


    bool operator () ()
    {
        if (_firstTime)
        {
            _firstTime = false;
            _timeStamp = Time::Timer::current().total_seconds();
        }

        auto t = sin( (time() + offset) * _frequency ) * _width;

        auto delta = t - _oldX;
        _oldX = t;
		if(_coordinateX)
			ScriptableSpriteActor::current().geometry.position.x += delta;
		else
			ScriptableSpriteActor::current().geometry.position.y += delta;
        return true;
    }

    Command::pointer clone() { return MX::make_shared<SinusoideCommand>(*this); }

    float time()
    {
        return Time::Timer::current().total_seconds() - _timeStamp;
    }
protected:
    float offset = 0.0f;
    float _oldX = 0.0f;
    float _timeStamp = 0.0f;
    float _timeOffset = 0.0f;
    bool _firstTime = true;
    float _frequency = 1;
    float _width = 1;

	bool _coordinateX = true;
};



void GameCommandsInit::Initialize()
{
    ScriptClassParser::AddCreator(L"Command.Sinusoide", new OutsideScriptClassCreatorContructor<SinusoideCommand>());

}

