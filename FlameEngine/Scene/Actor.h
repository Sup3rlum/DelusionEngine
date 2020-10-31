#pragma once


#include "../dll/nchfx.h"

#include "../Graphics/Shader.h"
#include "RenderableObject.h"


using namespace glm;

EXPORT_CLASS Scene;

EXPORT_CLASS Actor : public RenderableObject
{
public:
	
	std::string id;

	Actor();

	Vector3 Position;
	Quaternion Rotation;

	Scene* _scene;

};
