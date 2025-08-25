// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"

SpriteComponent::SpriteComponent(const std::shared_ptr<Actor>& owner, int drawOrder)
	: Component(owner)
	, mTexture(nullptr)
	, mDrawOrder(drawOrder)
	, mSize(32.0f, 32.0f)
{

}


void SpriteComponent::Draw(SDL_Renderer* renderer)
{
	std::shared_ptr<Actor> owner = mOwner.lock();
	if (owner == nullptr)
	{
		return;
	}

	if (mTexture)
	{
		SDL_Rect r;
		// Scale the width/height by owner's scale
		r.w = static_cast<int>(mSize.x * owner->GetScale());
		r.h = static_cast<int>(mSize.y * owner->GetScale());
		// Center the rectangle around the position of the owner
		r.x = static_cast<int>(owner->GetPosition().x - r.w / 2);
		r.y = static_cast<int>(owner->GetPosition().y - r.h / 2);

		// Draw (have to convert angle from radians to degrees, and clockwise to counter)
		SDL_RenderCopyEx(renderer,
			mTexture,
			nullptr,
			&r,
			-Math::ToDegrees(owner->GetRotation()),
			nullptr,
			SDL_FLIP_NONE);
	}
}

void SpriteComponent::SetTexture(SDL_Texture* texture)
{
	mTexture = texture;
}

void SpriteComponent::Initialize()
{
	if (std::shared_ptr<Actor> owner = mOwner.lock())
	{
		if (std::shared_ptr<Game> game = owner->GetGame().lock())
		{
			game->AddSprite(std::static_pointer_cast<SpriteComponent>(shared_from_this()));
		}
	}
}
