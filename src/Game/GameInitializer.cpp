#include "GameInitializer.h"

#include "Game/Resources/MXResources.h"
#include "Widgets/Animations/MXAnimations.h"
#include "Widgets/Drawers/MXDrawers.h"
#include "Widgets/Drawers/MXShaderDrawers.h"
#include "Widgets/Drawers/MXProxyDrawers.h"
#include "Widgets/Drawers/MXCompositeDrawers.h"
#include "Widgets/Drawers/MXDestinationDrawers.h"
#include "Widgets/Layouters/MXScriptLayouters.h"
#include "Graphic/Effects/MXEffectActors.h"
#include "Graphic/Particles/MXParticles.h"
#include "Scene/Script/MXEvent.h"

#include "Script/MXScriptClassParser.h"
#include "RPN/Function.h"

#include "Game/Main/MainGame.h"


namespace bs2 = boost::signals2;

using namespace MX;

using namespace std;

void BH::GameInitializer::Init()
{
	

	MX::ScriptableSpriteActorInit::Init();
	MX::EventInit::Init();


	MX::Graphic::ParticleInit::Init();
	MX::Graphic::EffectActorsInit::Init();
	MX::Widgets::DrawersInit::Init();
	MX::Widgets::ShaderDrawersInit::Init();
	MX::Widgets::ProxyDrawersInit::Init();
	MX::Widgets::CompositeDrawersInit::Init();
	MX::Widgets::DestinationDrawersInit::Init();
	MX::Widgets::ShapePolicyInit::Init();
	MX::Widgets::ScriptLayouterInit::Init();
	MX::Widgets::WidgetAnimationsInit::Init();



    BH::GameInit::Initialize();
    /*
    {
        using namespace Game;
        ScriptClassParser::AddCreator(L"Game.Event", new OutsideScriptClassCreatorContructor<GameEvent>());
        ScriptClassParser::AddCreator(L"Game.Response", new OutsideScriptClassCreatorContructor<Response>());
        //EventsInit::Init();
    }
    */
    AfterScriptParse();
}


void BH::GameInitializer::AfterScriptParse()
{

}

void BH::GameInitializer::ReloadScripts(bool reset)
{

#ifndef MX_GAME_RELEASE
    static  std::list<std::string> p = { "Script/Game", "Script/Resources", "Script/Test", "c:/" MX_APPLICATION_NAME "/" };
#else
    static  std::list<std::string> p = { "Script/Game", "Script/Resources", "Script/Test" };
#endif

    Script::ParseDirs(p, reset);
    AfterScriptParse();
}