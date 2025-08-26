#pragma once
#pragma once
#include "Actor.h"
class SpriteComponent;

class OpenSetMarker : public Actor
{
public:
	OpenSetMarker(const std::shared_ptr<Game>& game);
	virtual void Initialize() override;

private:
	std::weak_ptr<SpriteComponent> mSprite;
};

class CloseSetMarker : public Actor
{
public:
	CloseSetMarker(const std::shared_ptr<Game>& game);
	virtual void Initialize() override;

private:
	std::weak_ptr<SpriteComponent> mSprite;
};
