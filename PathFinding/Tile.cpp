// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Tile.h"
#include "SpriteComponent.h"
#include "Game.h"

Tile::Tile(const std::shared_ptr<Game>& game)
	: Actor(game)
	, mTileState(TileState::Default)	
{
}

void Tile::Initialize()
{
	mSprite = CreateComponent<SpriteComponent>(shared_from_this());
	UpdateTexture();
}

void Tile::UpdateTexture()
{
	std::shared_ptr<SpriteComponent> sprite = mSprite.lock();
	
	if (sprite == nullptr)
	{
		return;
	}

	std::string text;
	switch (mTileState)
	{
	case TileState::Start:
		text = "../Assets/Start.png";
		break;
	case TileState::End:
		text = "../Assets/End.png";
		break;
	case TileState::Wall:
		text = "../Assets/Wall.png";
		break;
	case TileState::Default:
	default:
		text = "../Assets/Default.png";
		break;
	}

	sprite->SetTexture(GetGame().lock()->GetTexture(text));
}


void Tile::SetSize(Vector2 size)
{
	if (std::shared_ptr<SpriteComponent> sprite = mSprite.lock())
	{
		sprite->SetSize(size);
	}
}

void Tile::SetTileState(TileState state)
{
	mTileState = state;
	UpdateTexture();
}