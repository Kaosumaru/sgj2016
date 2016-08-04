#ifndef BHTESTMANAGER
#define BHTESTMANAGER
#include<memory>
#include "Scene/Sprites/MXSpriteScene.h"
#include "Scene/Managers/MXSceneManager.h"
#include "Utils/MXUtils.h"

namespace BH
{
class TestManager : public MX::DisplaySceneTimer, public DeinitSingleton<TestManager>
{
public:
    TestManager();

	void clearReloadScripts();
	void reloadScripts();


	void AddSomething();
	void Draw(float x = 0.0f, float y = 0.0f);
protected: 
	
};
    
}


#endif
