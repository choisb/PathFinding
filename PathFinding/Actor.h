// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <vector>
#include "Math.h"
#include <cstdint>
#include <memory>

class Component;
class Game;

class Actor : public std::enable_shared_from_this<Actor>
{
public:
	enum State
	{
		EActive,
		EPaused,
		EDead
	};

	Actor(const std::shared_ptr<Game>& game);
	virtual ~Actor();

	template<class T, class... Param>
	std::shared_ptr<T> CreateComponent(Param&&... _Args)
	{
		static_assert(std::is_base_of<Component, T>::value, "Template argument T must be a derived class from the Component class");

		std::shared_ptr<T> component = std::make_shared<T>(shared_from_this(), std::forward<Param>(_Args)...);
		AddComponent(std::static_pointer_cast<Component>(component));
		component->Initialize();
		return component;
	}

	void Update(float deltaTime);
	void ProcessInput(const uint8_t* keyState);

	virtual void UpdateActor(float deltaTime);
	virtual void ActorInput(const uint8_t* keyState);
	virtual void Initialize() {}

	const Vector2& GetPosition() const { return mPosition; }
	void SetPosition(const Vector2& pos) { mPosition = pos; }
	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; }
	float GetRotation() const { return mRotation; }
	void SetRotation(float rotation) { mRotation = rotation; }

	Vector2 GetForward() const { return Vector2(Math::Cos(mRotation), -Math::Sin(mRotation)); }

	State GetState() const { return mState; }
	void SetState(State state) { mState = state; }

	std::weak_ptr<Game> GetGame() { return mGame; }

private:
	void AddComponent(const std::shared_ptr<Component>& component);
	void UpdateComponents(float deltaTime);

	State mState;

	Vector2 mPosition;
	float mScale;
	float mRotation;

	std::vector<std::shared_ptr<Component>> mComponents;
	std::weak_ptr<Game> mGame;
};
