#pragma once
#include "Source/Actor/Character/Character.h"

class Ball :public Character
{
public:
	Ball();
	virtual ~Ball();
	virtual bool Start();
	virtual void Update();
	virtual void Render(RenderContext& rc);


private:


};

