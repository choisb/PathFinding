// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Component.h"
#include "Actor.h"

Component::Component(const std::shared_ptr<Actor>& owner, int updateOrder)
	:mOwner(owner)
	,mUpdateOrder(updateOrder)
{

}

void Component::Update(float deltaTime)
{
}
