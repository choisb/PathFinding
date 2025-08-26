#include "PathMarker.h"
#include "SpriteComponent.h"
#include "Game.h"
#include <string>

PathMarker::PathMarker(const std::shared_ptr<Game>& game)
	:Actor(game)
{
}

void PathMarker::Initialize()
{
	mSprite = CreateComponent<SpriteComponent>(200);
	mSprite.lock()->SetTexture(GetGame().lock()->GetTexture(std::string("../Assets/PathMarker.png")));
}