#pragma once
#include "Source/Actor/Actor.h"

class Background:public Actor
{
public:
	Background();
	~Background();
	bool Start();
	void Update();
	void Render(RenderContext& rc);	
};

