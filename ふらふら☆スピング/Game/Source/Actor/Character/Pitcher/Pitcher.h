#pragma once
#include "Source/Actor/Character/Character.h"

class Pitcher :public Character
{
public:
	Pitcher();
	virtual ~Pitcher();
	virtual bool Start();
	virtual void Update();
	virtual void Render(RenderContext& rc);


private:


};

