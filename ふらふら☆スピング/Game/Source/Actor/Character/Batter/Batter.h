#pragma once
#include "Source/Actor/Character/Character.h"

class Batter :public Character
{
public:
	Batter();
	virtual ~Batter();
	virtual bool Start();
	virtual void Update();
	virtual void Render(RenderContext& rc);


private:


};

