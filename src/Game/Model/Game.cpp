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
#include "Game.h"
using namespace BH;
namespace bs2 = boost::signals2;


Game::Game(int players)
{
    for (int i = 0; i < players; i++)
    {
        auto player = std::make_shared<Player>(players == 1 ? 3 : i);
        _players.push_back(player);
    }
}

void Game::Update()
{
    for (auto& player : _players)
        player->Update();
}