// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "SDL/SDL.h"
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"
#include <memory>

class Actor;
class SpriteComponent;
class Grid;

class Game : public std::enable_shared_from_this<Game>
{
public:
	static std::shared_ptr<Game> Get();
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	template<class T, class... Param>
	std::shared_ptr<T> CreateActor(Param&&... _Args)
	{
		static_assert(std::is_base_of<Actor, T>::value, "Template argument T must be a derived class from the Actor class");

		std::shared_ptr<T> actor = std::make_shared<T>(shared_from_this(), std::forward<Param>(_Args)...);
		AddActor(std::static_pointer_cast<Actor>(actor));
		actor->Initialize();
		return actor;
	}

	void AddSprite(const std::shared_ptr<SpriteComponent>& sprite);
	void RemoveSprite(const std::shared_ptr<SpriteComponent>& sprite);
	
	SDL_Texture* GetTexture(const std::string& fileName);
	
	std::shared_ptr<Grid> GetGrid() { return mGrid; }

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();
	void AddActor(const std::shared_ptr<Actor>& actor);
	
	std::unordered_map<std::string, SDL_Texture*> mTextures;

	std::vector<std::shared_ptr<Actor>> mActors;
	std::vector<std::shared_ptr<Actor>> mPendingActors;
	std::vector<std::shared_ptr<SpriteComponent>> mSprites;

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	Uint32 mTicksCount;
	bool mIsRunning;
	bool mUpdatingActors;

	std::shared_ptr<Grid> mGrid;
};
