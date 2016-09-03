#include "MainGame.h"
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



void MainGame::Run()
{
    MX::FullscreenDisplayScene::Run();
    time = MX::Time::Timer::current().total_seconds();

    if (!_gameActive)
        return;

    int newTime = levelTime - time;
    if (newTime < 0)
    {
        GameOver(false);
        newTime = 0;
    }
        
    remainingTime = newTime;
}

void MainGame::GainPoints(int p)
{
    points = points + p;

    if (points > maxPoints)
        GameOver(true);
}

void MainGame::onExit()
{

}

void MainGame::GameOver(bool win)
{
    if (!_gameActive)
        return;
    Pause(true);
    _gameActive = false;
}

class PlayerActor;
class EmojiActor;

class EmojiShape : public MX::Collision::SignalizingCircleShape
{
public:
    EmojiShape(EmojiActor* emoji) : _emoji(emoji) { SetClassID(ClassID<EmojiShape>::id()); };
    EmojiActor* emoji() { return _emoji; }
protected:
    EmojiActor* _emoji;
};

class PlayerShape : public MX::Collision::SignalizingCircleShape
{
public:
    PlayerShape(PlayerActor* player) : _player(player) { SetClassID(ClassID<PlayerShape>::id()); };
    PlayerActor* player() { return _player; }
protected:
    PlayerActor* _player;
};



class EmojiActor : public MX::ScImageSpriteActor
{
public:
    EmojiActor(const EmojiActor& other) : MX::ScImageSpriteActor(other)
    {
        SetSize(other._size);
        _points = other._points;
        _speed = other._speed;
        _acceleration = other._acceleration;
    }

    EmojiActor(LScriptObject& script) : MX::ScImageSpriteActor(script)
    {
        script.load_property(_points, "Points");
        script.load_property(_size, "Size");
        script.load_property(_speed, "Speed");
        script.load_property(_acceleration, "Acceleration");
        SetSize(_size);
    }

    void Run() override
    {
        MX::ScImageSpriteActor::Run();

        geometry.position.y += _speed;
        _speed = _speed.getOriginalValue() + _acceleration;

        _shape->SetPosition(geometry.position);

        if (geometry.position.y > 1200)
            Unlink();
    }

    void Draw(float x, float y) override
    {
        MX::ScImageSpriteActor::Draw(x, y);
    }

    void OnLinkedToScene() override
    {
        MX::ScImageSpriteActor::OnLinkedToScene();
        if (!MainGame::isCurrent())
            return;
        MainGame::current().obstaclesArea()->AddShape(_shape);
    }

    void OnUnlinkedFromScene() override
    {
        MX::ScImageSpriteActor::OnUnlinkedFromScene();
        if (_shape)
            _shape->Unlink();
    }

    std::shared_ptr<ScriptableSpriteActor> cloneSprite() override
    {
        return std::make_shared<EmojiActor>(*this);
    }

    void SetSize(float size)
    {
        _size = size;
        auto circle = MX::make_shared<EmojiShape>(this);
        circle->Set(geometry.position, _size);
        _shape = circle;
    }


    auto &points()
    {
        return _points;
    }
protected:
    int _points = 10;
    std::shared_ptr<MX::Collision::SignalizingCircleShape> _shape;
    float                _size = 10;
    Time::FloatPerSecond _acceleration = 0.0f;
    Time::FloatPerSecond _speed = 0.0f;
};


class PlayerActor : public MX::ScImageSpriteActor
{
public:
    auto& game()
    {
        return MainGame::current();
    }

    PlayerActor(const PlayerActor& other) : MX::ScImageSpriteActor(other)
    {
        SetSize(other._size);
    }

    PlayerActor(LScriptObject& script) : MX::ScImageSpriteActor(script)
    {
        script.load_property(_size, "Size");
        SetSize(_size);
    }

    void Run() override
    {
        MX::ScImageSpriteActor::Run();

        auto &mousePos = MX::Window::current().mouse()->position();
        geometry.position.x = mousePos.x;
        geometry.position.y = 600;
        
        _shape->SetPosition(geometry.position);
    }

    void Draw(float x, float y) override
    {
        MX::ScImageSpriteActor::Draw(x, y);
    }

    void SetSize(float size)
    {
        _size = size;
        auto circle = MX::make_shared<PlayerShape>(this);
        circle->Set(geometry.position, _size);
        _shape = circle;

        
        circle->with<EmojiShape>()->onCollided.connect([&](const Collision::Shape::pointer& shape, auto...)
        {
            auto emojiShape = std::static_pointer_cast<EmojiShape>(shape);
            if (!emojiShape)
                return;
            collidedWith(emojiShape->emoji());
        });
    }

    void OnLinkedToScene() override
    {
        MX::ScImageSpriteActor::OnLinkedToScene();
        if (!MainGame::isCurrent())
            return;
        MainGame::current().obstaclesArea()->AddShape(_shape);
    }

    void OnUnlinkedFromScene() override
    {
        MX::ScImageSpriteActor::OnUnlinkedFromScene();
        if (_shape)
            _shape->Unlink();
    }

    void collidedWith(EmojiActor* emoji)
    {
        game().GainPoints(emoji->points());
        emoji->Unlink();
        
    }

    std::shared_ptr<ScriptableSpriteActor> cloneSprite() override
    {
        return std::make_shared<PlayerActor>(*this);
    }

    std::shared_ptr<MX::Collision::SignalizingCircleShape> _shape;
    float                _size = 10;
};



class EmojiSpawnDecorator : public MX::ActorDecorator
{
public:
    EmojiSpawnDecorator() {}
    EmojiSpawnDecorator(LScriptObject& script) : ActorDecorator(script)
    {
        script.load_property(_pos, "Pos");
    }

    EmojiSpawnDecorator(const EmojiSpawnDecorator& other) : ActorDecorator(other)
    {
        _pos = other._pos;
    }

    void DecorateActor(const std::shared_ptr<ScriptableSpriteActor> &actor)
    {
        actor->geometry.position.y = -100;
        actor->geometry.position.x = Random::randomRange(_pos);
    }

    ActorDecorator::pointer clone() { return MX::make_shared<EmojiSpawnDecorator>(*this); }
    std::pair<float, float> _pos;
};

MainGame::MainGame()
{
    auto size = MX::Window::current().display()->size();
    MX::Rectangle rect(size.x, size.y);
    rect.Expand(400);
    _obstaclesArea = MX::make_shared<Collision::SimplestWeakArea>();
    //_obstaclesArea->DefineLayerCollision(ClassID<PlayerActor>::id(), ClassID<EmojiActor>::id());

    MainGame::SetCurrent(*this);
    _cheats = BH::CreateCheats();
    Context<BaseGraphicScene>::SetCurrent(*this);


    ScriptObjectString script("Game");

    std::shared_ptr<PlayerActor> player;
    script.load_property(player, "Player.P1");

    if (player)
        AddActor(player);

    script.load_property(_factory, "Enemies.Factory");
    if (_factory)
        AddActor(_factory);

    script.load_property(levelTime, "Time");
    remainingTime = levelTime;

}

void GameInit::Initialize()
{
    ScriptClassParser::AddCreator(L"Scene.ActorFactory.Decorator.EmojiSpawn", new OutsideScriptClassCreatorContructor<EmojiSpawnDecorator>());

    ScriptClassParser::AddCreator(L"Game.Player", new OutsideScriptClassCreatorContructor<PlayerActor>());
    ScriptClassParser::AddCreator(L"Game.Emoji", new OutsideScriptClassCreatorContructor<EmojiActor>());
}

