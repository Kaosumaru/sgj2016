#ifndef BHMGUIMANAGER
#define BHMGUIMANAGER
#include<memory>
#include "Scene/Sprites/MXSpriteScene.h"
#include "Scene/Managers/MXSceneStackManager.h"
#include "Utils/MXUtils.h"


namespace bs2 = boost::signals2;

namespace BH
{
    
class GuiManager : public MX::SpriteSceneStackManager, public DeinitSingleton<GuiManager>
{
public:
	GuiManager();

	void reloadScripts();	
};
    
}


#endif
