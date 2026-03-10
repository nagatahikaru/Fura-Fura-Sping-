#pragma once
#include "Source/Actor/Character/Character.h"

class Catcher :public Character
{
public:
	Catcher();
	virtual ~Catcher();
	virtual bool Start();
	virtual void Update();
	virtual void Render(RenderContext& rc);


private:


};

