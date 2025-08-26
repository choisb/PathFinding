// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Actor.h"
#include <vector>

class SpriteComponent;

enum class TileState
{
	Default,
	Start,
	End,
	Wall
};

class Tile : public Actor
{
	friend class Grid;
public:
	Tile(const std::shared_ptr<Game>& game);
	
	virtual void Initialize() override;

	void SetSize(Vector2 size);
	void SetTileState(TileState state);
	TileState GetTileState() const { return mTileState; }

private:
	void UpdateTexture();
	std::weak_ptr<SpriteComponent> mSprite;
	TileState mTileState;
};
