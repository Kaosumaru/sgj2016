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
#include "Game/Model/Gem.h"
#include "Script/MXScriptClassParser.h"



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


}


void BH::GameInitializer::AfterScriptParse()
{
    auto &script = Script::get();
    script.SetPairFunctor("Gem.Exploding", []()-> float { return Context<Gem>::current()._exploding; });
    script.SetPairFunctor("Gem.ExplosionPercent", []()-> float 
    { 
        if (!Context<Gem>::isCurrent())
            return 0.0f;
        return Context<Gem>::current().explosionPercent(); 
    });
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