// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Grid.h"
#include "Tile.h"
#include "Game.h"

#include <algorithm>

Grid::Grid(const std::shared_ptr<Game>& game)
	: Actor(game)
{
	SetPosition(Vector2(0.f, 100.0f));

	mTiles.resize(mNumRows);
	for (size_t i = 0; i < mTiles.size(); i++)
	{
		mTiles[i].resize(mNumCols);
	}
	
	// Create tiles
	for (size_t i = 0; i < mNumRows; i++)
	{
		for (size_t j = 0; j < mNumCols; j++)
		{
			std::shared_ptr<Tile> NewTile = game->CreateActor<Tile>(game);
			mTiles[i][j] = NewTile;
			NewTile->SetPosition(GetPosition() + Vector2(mTileSize * 0.5f + j * mTileSize, i * mTileSize));
			NewTile->SetSize(Vector2(mTileSize, mTileSize));
		}
	}
	
	std::shared_ptr<Tile> startTile = GetTile(mStartPos);
	std::shared_ptr<Tile> endTile = GetTile(mEndPos);

	if (startTile && endTile)
	{
		startTile->SetTileState(TileState::Start);
		endTile->SetTileState(TileState::End);
	}
}

void Grid::HandleMouseDown(int x, int y)
{
	std::shared_ptr<Tile> selectTile = GetSelectTile(x, y);
	if (selectTile == nullptr)
	{
		return;
	}

	mCapturedTile = selectTile;

	switch (selectTile->GetTileState())
	{
	case TileState::Default:
		selectTile->SetTileState(TileState::Wall);
		break;
	case TileState::Wall:
		selectTile->SetTileState(TileState::Default);
		break;
	}
}

void Grid::HandleMouseMove(int x, int y)
{
	std::shared_ptr<Tile> capturedTile = mCapturedTile.lock();
	if (capturedTile == nullptr)
	{
		return;
	}

	std::shared_ptr<Tile> selectTile = GetSelectTile(x, y);
	if (selectTile == nullptr)
	{
		return;
	}

	if (capturedTile == selectTile)
	{
		return;
	}

	switch (capturedTile->GetTileState())
	{
	case TileState::Default:
	case TileState::Wall:
		selectTile->SetTileState(capturedTile->GetTileState());
		break;

	case TileState::Start:
	case TileState::End:
		if (selectTile->GetTileState() == TileState::Default)
		{
			selectTile->SetTileState(capturedTile->GetTileState());
			capturedTile->SetTileState(TileState::Default);
			mCapturedTile = selectTile;
		}
		break;
	}
}

void Grid::HandleMouseUp(int x, int y)
{
	mCapturedTile.reset();
}

std::shared_ptr<Tile> Grid::GetSelectTile(int x, int y)
{
	if (std::shared_ptr<Tile> firstTile = GetTile(0, 0))
	{
		x -= static_cast<int>(firstTile->GetPosition().x - mTileSize / 2);
		y -= static_cast<int>(firstTile->GetPosition().y - mTileSize / 2);

		if (y >= 0)
		{
			x /= static_cast<int>(mTileSize);
			y /= static_cast<int>(mTileSize);
			if (x >= 0 && x < static_cast<int>(mNumCols) && y >= 0 && y < static_cast<int>(mNumRows))
			{
				return GetTile(x, y);
			}
		}
	}
	return std::shared_ptr<Tile>();
}

void Grid::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
}

std::shared_ptr<Tile> Grid::GetTile(Vector2 pos)
{
	return GetTile(static_cast<size_t>(pos.x), static_cast<size_t>(pos.y));
}

std::shared_ptr<Tile> Grid::GetTile(size_t x, size_t y)
{
	if (mTiles.size() > y && mTiles[y].size() > x)
	{
		return mTiles[y][x].lock();
	}
	else
	{
		return std::shared_ptr<Tile>();
	}
}
