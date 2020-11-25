#pragma once

#include "../../dll/nchfx.h"


EXPORT_ENUM ShaderSourceType
{
	VERTEX,
	GEOMETRY,
	FRAGMENT
};


EXPORT_CLASS ShaderSourceDescription
{
public:
	STRING mSource;
	ShaderSourceType mType;

	ShaderSourceDescription(STRING, ShaderSourceType);

};

