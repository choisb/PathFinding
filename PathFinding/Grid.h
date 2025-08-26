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

class Game;
class Tile;

class Grid : public Actor
{
public:
	Grid(const std::shared_ptr<Game>& game);
	
	void HandleMouseDown(int x, int y);
	void HandleMouseMove(int x, int y);
	void HandleMouseUp(int x, int y);

	void UpdateActor(float deltaTime) override;
	std::shared_ptr<Tile> GetTile(Vector2 pos);
	std::shared_ptr<Tile> GetTile(size_t x, size_t y);

private:
	std::shared_ptr<Tile> GetSelectTile(int x, int y);
	std::weak_ptr<Tile> mCapturedTile;
	std::vector<std::vector<std::weak_ptr<Tile>>> mTiles;
	
	const size_t mNumCols = 32;
	const size_t mNumRows = 20;
	const float mTileSize = 32.0f;

	Vector2 mStartPos {4,9};
	Vector2 mEndPos {26,9};
};
