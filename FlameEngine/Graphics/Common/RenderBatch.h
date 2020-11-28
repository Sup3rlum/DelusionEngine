#pragma once

#include "../../dll/nchfx.h"
#include "../../Graphics/Texture.h"
#include "../../Graphics/MultisampleTexture.h"
#include "../../Graphics/ShaderDefinitions/Shader.h"
#include "../../Graphics/VertexBuffer.h"
#include "../../Graphics/Vertex.h"
#include "../../Graphics/UserInterface/UxFont.h"
#include "../../Graphics/RenderState.h"
#include "../../Context/ContextDescription.h"
#include "../../Context/Context.h"


using namespace std;

EXPORT_CLASS RenderBatch
{

public:
	RenderBatch(Context*_context);

	void DrawTexture(Texture* _tex, float x, float y, float width, float height);
	void DrawMultisampleTexture(MultisampleTexture* _tex, float x, float y, float width, float height);
	void DrawTexture(Texture* _tex, float x, float y, float width, float height, Shader* _shader);
	void DrawTextures(int count, Texture** _tex, float x, float y, float width, float height);
	void DrawTextures(int count, Texture** _tex, float x, float y, float width, float height, Shader* _shader);
	void DrawTextures(int count, Texture** _tex, float x, float y, float width, float height, fMatrix4 _m, Shader* _shader);

	void DrawString(string text, UxFont* font, float x, float y, Color color);


	Shader* _shader;
	Shader* _msShader;
	Shader* _shaderString;
	VertexBuffer* _vb;

	fMatrix4 View;

	RenderState* State;

};

