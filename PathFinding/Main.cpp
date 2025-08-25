// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Game.h"
#include <memory>

int main(int argc, char** argv)
{
	std::shared_ptr<Game> game = std::make_shared<Game>();
	bool success = game->Initialize();
	if (success)
	{
		game->RunLoop();
	}
	game->Shutdown();
	return 0;
}
