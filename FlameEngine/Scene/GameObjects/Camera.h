#pragma once

#include "../../dll/nchfx.h"

#include "../../Engine/FrameTime.h"
#include "../../Context/ContextDescription.h"
#include "../../Context/Context.h"


using namespace glm;


EXPORT_CLASS Camera
{

public:
	Camera();
	~Camera();

	fMatrix4 View;
	fMatrix4 DebugView;
	fMatrix4 Projection;
	fMatrix4 ProjectionInverse;

	fVector3 Position;
	fVector3 Target;
	fVector3 Right;
	fVector3 Up;


	virtual void Update();


};

