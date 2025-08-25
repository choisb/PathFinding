// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Component.h"
#include "SDL/SDL.h"
#include "Math.h"

class Actor;

class SpriteComponent : public Component
{
public:
	SpriteComponent(const std::shared_ptr<Actor>& owner, int drawOrder = 100);

	virtual void Draw(SDL_Renderer* renderer);
	virtual void SetTexture(SDL_Texture* texture);
	virtual void Initialize() override;

	void SetSize(Vector2 size) { mSize = size; }

	Vector2 GetSize() const { return mSize; }
	int GetDrawOrder() const { return mDrawOrder; }

private:
	SDL_Texture* mTexture;
	int mDrawOrder;
	Vector2 mSize;
};
