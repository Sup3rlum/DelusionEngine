#pragma once

#include "../../dll/nchfx.h"
#include "../Common/VertexBuffer.h"
#include "../Common/Vertex.h"
#include "../../Scene/GameObjects/Camera.h"
#include "../ShaderDefinitions/Program.h"
#include "RenderingServiceBase.h"

EXPORT(class, BoundingVolumeDebugService) : public RenderingServiceBase
{
public:
	BoundingVolumeDebugService(Context* context);
	void Render(Camera* cam, FVector3* Corners);

	VertexBuffer* mVertexBuffer;
	Program* mDebugShader;
	
};

