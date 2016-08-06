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

    if (players == 1)
    {
        auto &player = _players.front();
        for (auto &m : player->stats().manas())
        {
            m->_max = 100.0f;
            m->Gain(100.0f);
        }
            
    }
}

void Game::Update()
{
    auto g1 = Context<Game>::Lock(this);
    if (_end)
        return;
    for (auto& player : _players)
        player->Update();

    bool lostFirst = false;
    bool lostSecond = false;

    if (_players.size() > 0)
        lostFirst = _players[0]->_lost;

    if (_players.size() > 1)
        lostSecond = _players[1]->_lost;

    if (!lostFirst && !lostSecond)
        return;

    _end = true;

    int result = 0;
    if (lostFirst)
        result = 1;
    if (lostSecond)
        result = 0;
    if (lostSecond && lostFirst)
        result = -1;
    onGameWon(result);

}