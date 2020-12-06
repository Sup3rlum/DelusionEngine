#include "DeferredRenderer.h"


DeferredRenderer::DeferredRenderer(Context* context)
{
	attachedContext = context;

	mBlurRenderingService = new KawaseBlurService(context, true);
	mHbaoPlusService = new HBAOPlusService(context);
	mBoundingService = new BoundingVolumeDebugService(context);

	View = FMatrix4::CreateOrthographic(0.0f, (float)context->_contextDescription->width, (float)context->_contextDescription->height, 0.0f, 0.0f, 1.0f);

	VertexTexture mQuadData[4] =
	{
		VertexTexture(FVector3(0,0,0), FVector2(0,1)),
		VertexTexture(FVector3(1,0,0), FVector2(1,1)),
		VertexTexture(FVector3(1,1,0), FVector2(1,0)),
		VertexTexture(FVector3(0,1,0), FVector2(0,0))
	};
	GLuint mQuadIndexData[6]
	{
		0,
		1,
		2,

		0,
		2,
		3
	};



	mQuadBuffer = new VertexBuffer(VertexTexture::Elements);
	mQuadBuffer->SetIndexedData<VertexTexture>(mQuadData, mQuadIndexData, 4, 6);

	mRenderState = new RenderState();
	mRenderState->CullState = CullState::Front;
	mRenderState->DepthFunction = DepthFunc::Always;
	mRenderState->SourceBlend = BlendFunc::SourceAlpha;
	mRenderState->DestinationBlend = BlendFunc::OneMinusSourceAlpha;


	Shader* geomVert = FLSLCompilerService::CompileShaderFromSourceFile(".\\shaders\\ssao_geom.vert", ShaderType::VERTEX);
	Shader* quadCommonVert = FLSLCompilerService::CompileShaderFromSourceFile(".\\shaders\\renderbatch.vert", ShaderType::VERTEX);

	Shader* geomFrag = FLSLCompilerService::CompileShaderFromSourceFile(".\\shaders\\ssao_geom.frag", ShaderType::FRAGMENT);
	Shader* ssaoFrag = FLSLCompilerService::CompileShaderFromSourceFile(".\\shaders\\ssao.frag", ShaderType::FRAGMENT);
	Shader* finalFrag = FLSLCompilerService::CompileShaderFromSourceFile(".\\shaders\\ssao_final.frag", ShaderType::FRAGMENT);



	Shader* sceneshaders[2] =
	{
		geomVert,
		geomFrag
	};

	Shader* ssaoshaders[2] =
	{
		quadCommonVert,
		ssaoFrag
	};

	Shader* combineshaders[2] =
	{
		quadCommonVert,
		finalFrag
	};

	mSceneDataShader	= new Program(sceneshaders);
	mSsaoShader			= new Program(ssaoshaders);
	mCombineShader		= new Program(combineshaders);



	mDepthBuffer = new Texture(2560, 1440, GL_RG32F, GL_RGBA, GL_FLOAT, false);
	mDepthBuffer->SetFilteringMode(TextureFiltering::ANISOTROPIC_8);
	mDepthBuffer->SetWrappingMode(TextureWrapping::REPEAT);

	mNormalBuffer = new Texture(2560, 1440, GL_RGBA32F, GL_RGBA, GL_FLOAT, false);
	mNormalBuffer->SetFilteringMode(TextureFiltering::BILINEAR);
	mNormalBuffer->SetWrappingMode(TextureWrapping::REPEAT);

	mAlbedoBuffer = new Texture(2560, 1440, GL_RGBA32F, GL_RGBA, GL_FLOAT, false);
	mAlbedoBuffer->SetFilteringMode(TextureFiltering::BILINEAR);
	mAlbedoBuffer->SetWrappingMode(TextureWrapping::REPEAT);



	mSsaoBuffer = new Texture(2560, 1440, GL_RGBA32F, GL_RGBA, GL_FLOAT, false);
	mSsaoBuffer->SetFilteringMode(TextureFiltering::BILINEAR);
	mSsaoBuffer->SetWrappingMode(TextureWrapping::REPEAT);


	mShadowmapBuffer = new Texture(2560, 1440, GL_RGBA32F, GL_RGBA, GL_FLOAT, true);
	mShadowmapBuffer->SetFilteringMode(TextureFiltering::BILINEAR);
	mShadowmapBuffer->SetWrappingMode(TextureWrapping::REPEAT);



	mFrameBuffer = new FrameBuffer(2560, 1440);
	mFrameBuffer->Bind();
	mFrameBuffer->BindTexture(mDepthBuffer, GL_COLOR_ATTACHMENT0);
	mFrameBuffer->BindTexture(mNormalBuffer, GL_COLOR_ATTACHMENT1);
	mFrameBuffer->BindTexture(mAlbedoBuffer, GL_COLOR_ATTACHMENT2);
	mFrameBuffer->EnableDepth();
	mFrameBuffer->SetAttachments(new GLuint[3]{ GL_COLOR_ATTACHMENT0 ,GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 }, 3);
	mFrameBuffer->Unbind();


	mShadowmapFrameBuffer = new FrameBuffer(2560, 1440);
	mShadowmapFrameBuffer->Bind();
	mShadowmapFrameBuffer->BindTexture(mShadowmapBuffer, GL_COLOR_ATTACHMENT0);
	mShadowmapFrameBuffer->EnableDepth();
	mShadowmapFrameBuffer->Unbind();


	mSsaoFrameBuffer = new FrameBuffer(2560, 1440);
	mSsaoFrameBuffer->Bind();
	mSsaoFrameBuffer->BindTexture(mSsaoBuffer, GL_COLOR_ATTACHMENT0);
	//mS_ssaoFrameBuffer->EnableDepth();
	mSsaoFrameBuffer->Unbind();

	_renderBatch = new RenderBatch(context);
}


void DeferredRenderer::BeginRender(Scene* scene)
{
	mShadowmapFrameBuffer->Bind();
	{
		OpenGL::Clear(Color::Transparent);

		scene->PushCamera(scene->DirectionalLightCollection[0].LightCamera());
		scene->Render();
		scene->PopCamera();
	}
	mShadowmapFrameBuffer->Unbind();

	mBlurRenderingService->ApplyFilter(mShadowmapFrameBuffer, mShadowmapBuffer, mBlurPassIndex);


	mFrameBuffer->Bind();
	{
		OpenGL::Clear(Color::Transparent);

		scene->Render();
	}
	mFrameBuffer->Unbind();

	if (enableHBAO)
	{
		mHbaoPlusService->RenderAO(mDepthBuffer, mNormalBuffer, mSsaoFrameBuffer, scene->CurrentCamera()->Projection, scene->CurrentCamera()->View);

	}
	else
	{
		mSsaoFrameBuffer->Clear(Color::White);
	}

	OpenGL::Clear(Color::CornflowerBlue);

	RenderState::Push(mRenderState);

	mCombineShader->UseProgram();
	mCombineShader->SetUniform("InverseProjection", scene->CurrentCamera()->ProjectionInverse);
	mCombineShader->SetUniform("InverseView", scene->CurrentCamera()->ViewInverse);


	for (int i = 0; i < MAX_DIRECTIONAL_LIGHTS && i < scene->DirectionalLightCollection.size(); i++)
	{
		mCombineShader->SetUniform("DirectionalLights[" + to_string(i) + "].Direction",	scene->DirectionalLightCollection[0].Direction);
		mCombineShader->SetUniform("DirectionalLights[" + to_string(i) + "].Color",		scene->DirectionalLightCollection[0].LightColor);
		mCombineShader->SetUniform("DirectionalLights[" + to_string(i) + "].Intensity",	scene->DirectionalLightCollection[0].Intensity);
		mCombineShader->SetUniform("DirectionalLights[" + to_string(i) + "].VPMatrix",	scene->DirectionalLightCollection[0].LightCamera()->Projection * scene->DirectionalLightCollection[0].LightCamera()->View);
	}

	mCombineShader->SetUniform("gDepth", 0);
	mCombineShader->SetUniform("gNormal", 1);
	mCombineShader->SetUniform("gAlbedo", 2);
	mCombineShader->SetUniform("ssao", 3);
	mCombineShader->SetUniform("_shadowMap", 4);

	mCombineShader->SetUniform("View", View);
	mCombineShader->SetUniform("MatrixTransforms", FMatrix4::Scaling(FVector3(attachedContext->_contextDescription->width, attachedContext->_contextDescription->height, 1)));


	mCombineShader->SetTexture(0, mDepthBuffer);
	mCombineShader->SetTexture(1, mNormalBuffer);
	mCombineShader->SetTexture(2, mAlbedoBuffer);
	mCombineShader->SetTexture(3, mSsaoBuffer);
	mCombineShader->SetTexture(4, mShadowmapBuffer);


	mQuadBuffer->RenderIndexed(GL_TRIANGLES);

	RenderState::Pop();

	/*
	FVector3 corn[8];

	scene->CurrentCamera()->GetFrustumCorners(corn);
	mBoundingService->Render(scene->CurrentCamera(), corn);

	/*scene->LightCollection[0].aabb.GetCorners(corn);
	for (int i = 0; i < 8; i++)
	{
		corn[i] = FMatrix3::Transpose(scene->LightCollection[0].localSpace) * corn[i];
	}
	mBoundingService->Render(scene->CurrentCamera(), corn);

	FVector3 ndc_corners[8] =
	{
		FVector3(1.0f, -1.0f, -1.0f),	 // llb
		FVector3(-1.0f, -1.0f, -1.0f), // lrb
		FVector3(-1.0f, 1.0f, -1.0f),  // urb
		FVector3(1.0f, 1.0f, -1.0f),	 // ulb


		FVector3(1.0f, -1.0f, 1.0f),   // llf
		FVector3(-1.0f, -1.0f, 1.0f),  // lrf
		FVector3(-1.0f, 1.0f, 1.0f),   // urf
		FVector3(1.0f, 1.0f, 1.0f)	 // ulf

	};

	FVector3 lightPos = FMatrix3::Transpose(scene->LightCollection[0].localSpace) * (scene->LightCollection[0].aabb.Center() + FVector3(0, 0, scene->LightCollection[0].aabb.LengthZ() / 2.0f));

	for (int i = 0; i < 8; i++)
	{
		corn[i] = (FMatrix4::Transpose(FMatrix4::Translation(lightPos)) * FVector4(ndc_corners[i] * 0.5f, 1.0f)).xyz;
	}
	mBoundingService->Render(scene->CurrentCamera(), corn);

	scene->LightCollection[0]._cam.GetFrustumCorners(corn);

	mBoundingService->Render(scene->CurrentCamera(), corn);*/

	
	if (enableDEBUGTexture)
	{
		_renderBatch->DrawTexture(mDepthBuffer, 1200, 0, 200, 200);
		_renderBatch->DrawTexture(mNormalBuffer, 1400, 0, 200, 200);
		_renderBatch->DrawTexture(mAlbedoBuffer, 1600, 0, 200, 200);
		_renderBatch->DrawTexture(mSsaoBuffer, 1800, 0, 200, 200);
		_renderBatch->DrawTexture(mShadowmapBuffer, 2000, 0, 200, 200);
	}
	scene->pUxService->Render();
}

void DeferredRenderer::EndRender()
{

}