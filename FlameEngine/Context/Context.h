#pragma once

#include "../dll/nchfx.h"
#include "ContextDescription.h"
#include "../Engine/FrameTime.h"
#include "../Input/Keyboard.h"
#include "../Input/Mouse.h"
#include "../flameRT/Runtime.h"


class Context
{
public:

	virtual void Initialize(ContextDescription* _contextDescription);
	virtual void SwapChain();
	virtual void Destroy();
	virtual void PollClose();

	virtual void GetCursorPosition(double* x, double* y);
	virtual void SetCursorPosition(double x, double y);


	virtual void SetCursorVisible(bool visible);


	virtual KeyState GetKeyState(Keys key);
	virtual MouseButtonState GetMouseButtonState(MouseButton mbutton);

	bool shouldTerminate;
	ContextDescription* _contextDescription;
};

