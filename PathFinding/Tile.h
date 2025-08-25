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

	const std::shared_ptr<Tile> GetParent() const { return mParent.lock(); }
	void AddAdjacent(const std::weak_ptr<Tile>& adjacent);

	void SetSize(Vector2 size);
	void SetTileState(TileState state);
	TileState GetTileState() const { return mTileState; }

private:
	std::vector<std::weak_ptr<Tile>> mAdjacent;
	std::weak_ptr<Tile> mParent;
	float f;
	float g;
	float h;
	bool mInOpenSet;
	bool mInClosedSet;
	bool mBlocked;
	
	void UpdateTexture();
	std::weak_ptr<SpriteComponent> mSprite;
	TileState mTileState;
};
