// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>

Actor::Actor(const std::shared_ptr<Game>& game)
	: mState(EActive)
	, mPosition(Vector2::Zero)
	, mScale(1.0f)
	, mRotation(0.0f)
	, mGame(game)
{
}

Actor::~Actor()
{
}

void Actor::Update(float deltaTime)
{
	if (mState == EActive)
	{
		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);
	}
}


void Actor::UpdateActor(float deltaTime)
{
}

void Actor::ProcessInput(const uint8_t* keyState)
{
	if (mState == EActive)
	{
		for (std::shared_ptr<Component>& comp : mComponents)
		{
			if (comp)
			{
				comp->ProcessInput(keyState);
			}
		}

		ActorInput(keyState);
	}
}

void Actor::ActorInput(const uint8_t* keyState)
{
}

void Actor::AddComponent(const std::shared_ptr<Component>& component)
{
	if (component == nullptr)
	{
		return;
	}

	int myOrder = component->GetUpdateOrder();
	auto iter = mComponents.begin();
	while(iter != mComponents.end())
	{
		if (*iter && myOrder < (*iter)->GetUpdateOrder())
		{
			break;
		}

		++iter;
	}

	mComponents.insert(iter, component);
}

void Actor::UpdateComponents(float deltaTime)
{
	for (std::shared_ptr<Component>& comp : mComponents)
	{
		if (comp)
		{
			comp->Update(deltaTime);
		}
	}
}