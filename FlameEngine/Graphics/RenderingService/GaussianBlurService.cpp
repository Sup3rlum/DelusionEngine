#include "GaussianBlurService.h"


GaussianBlurService::GaussianBlurService(Context* context, bool halfRes) : RenderingServiceBase(context)
{

	mShader = Shader::FromSource(".\\shaders\\renderbatch.vert", ".\\shaders\\gauss_blur.frag");

	mBuffer = new Texture(2560, 1440, GL_RGBA32F, GL_RGBA, GL_FLOAT, true);
	mBuffer->SetFilteringMode(TextureFiltering::BILINEAR);
	mBuffer->SetWrappingMode(TextureWrapping::REPEAT);

	mFrameBuffer = new FrameBuffer(2560,1440);
	mFrameBuffer->Bind();
	mFrameBuffer->BindTexture(mBuffer, GL_COLOR_ATTACHMENT0);
	mFrameBuffer->Unbind();


}


void GaussianBlurService::ApplyFilter(FrameBuffer* frameBuffer, Texture* texture)
{
	//glViewport(0, 0, 1280, 720);

	mFrameBuffer->Bind();
	{

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		RenderState::Push(mRenderState);

		mShader->UseProgram();

		mShader->SetVector("Direction", FVector2(0.0f, 1.0f));
		mShader->SetMatrix("View", View);
		mShader->SetMatrix("MatrixTransforms", FMatrix4::Scaling(FVector3(mAttachedContext->_contextDescription->width, mAttachedContext->_contextDescription->height, 1)));

		mShader->SetTexture(0, texture);

		mQuadBuffer->RenderIndexed(GL_TRIANGLES);

		RenderState::Pop();
	}
	mFrameBuffer->Unbind();


	frameBuffer->Bind();
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		RenderState::Push(mRenderState);

		mShader->UseProgram();

		mShader->SetVector("Direction", FVector2(1.0f, 0.0f));
		mShader->SetMatrix("View", View);
		mShader->SetMatrix("MatrixTransforms", FMatrix4::Scaling(FVector3(mAttachedContext->_contextDescription->width, mAttachedContext->_contextDescription->height, 1)));

		mShader->SetTexture(0, mBuffer);

		mQuadBuffer->RenderIndexed(GL_TRIANGLES);

		RenderState::Pop();
	}
	frameBuffer->Unbind();

	//glViewport(0, 0, 2560, 1440);
}