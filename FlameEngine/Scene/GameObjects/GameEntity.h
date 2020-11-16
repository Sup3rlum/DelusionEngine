#pragma once

#include "Model.h"
#include "Actor.h"
#include "../../IO/AssetManager.h"



EXPORT_CLASS GameEntity : public Actor
{

public:
	GameEntity(STRING _modelName);

	void Render();
	void Update(FrameTime* _frameTime);

	Model model;
};

