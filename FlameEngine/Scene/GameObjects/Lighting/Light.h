#pragma once

#include "../../../dll/nchfx.h"
#include "../../../Graphics/Color.h"
#include "../../../Mathematics/Module.h"

EXPORT_STRUCT Light
{
public:
	Color LightColor;

	fMatrix4 View;
	fMatrix4 Projection;

	Light(Color _color);
	Light();
};

