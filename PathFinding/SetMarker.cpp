#include "SetMarker.h"
#include "SpriteComponent.h"
#include "Game.h"
#include <string>

OpenSetMarker::OpenSetMarker(const std::shared_ptr<Game>& game)
	:Actor(game)
{
}

void OpenSetMarker::Initialize()
{
	mSprite = CreateComponent<SpriteComponent>(150);
	mSprite.lock()->SetTexture(GetGame().lock()->GetTexture(std::string("../Assets/OpenSetMarker.png")));
	SetScale(0.5f);
}

CloseSetMarker::CloseSetMarker(const std::shared_ptr<Game>& game)
	:Actor(game)
{
}

void CloseSetMarker::Initialize()
{
	mSprite = CreateComponent<SpriteComponent>(150);
	mSprite.lock()->SetTexture(GetGame().lock()->GetTexture(std::string("../Assets/CloseSetMarker.png")));
	SetScale(0.5f);
}