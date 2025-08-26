#pragma once
#include "Actor.h"
class SpriteComponent;

class PathMarker : public Actor
{
public:
	PathMarker(const std::shared_ptr<Game>& game);
	virtual void Initialize() override;

private:
	std::weak_ptr<SpriteComponent> mSprite;
};