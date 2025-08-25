// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <cstdint>
#include <memory>

class Actor;

class Component : public std::enable_shared_from_this<Component>
{
public:

	Component(const std::shared_ptr<Actor>& owner, int updateOrder = 100);
	virtual void Initialize() {}
	virtual void Update(float deltaTime);
	virtual void ProcessInput(const uint8_t* keyState) {}

	int GetUpdateOrder() const { return mUpdateOrder; }

    std::weak_ptr<Actor> GetActor() { return mOwner; }

protected:
	std::weak_ptr<Actor> mOwner;
	int mUpdateOrder;
};
