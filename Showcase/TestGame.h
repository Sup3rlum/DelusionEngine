#pragma once

#include "FlameEngine\Context\GLFWContext.h"
#include "FlameEngine\Engine\EngineInstance.h"
#include "FlameEngine\Scene\GameEntity.h"



class TestGame : public Game
{
public:
	TestGame(STRING name, DVERSION dv);

	void Load();
	void Dispose();
	void Suspend();
};
