#pragma once


#include "Camera.h"
#include "DebugView.h"
#include "Actor.h"
#include "../Graphics/Shader.h"
#include "../Graphics/RenderBatch.h"
#include "../Graphics/FrameBuffer.h"
#include "../Context/Context.h"


EXPORT_CLASS Scene
{
public:
	Scene(Context* _context);
	~Scene();

	void Update();
	void Render();

	Camera* _camera;

	void AddActor(STRING id, Actor* ac);
	void RemoveActor(STRING id);

private:
	
	std::unordered_map<STRING, Actor*> actorCollection;
};
