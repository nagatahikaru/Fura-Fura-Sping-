#pragma once
#include "Source/Source.h"

class Lrad : public Source
{
public:
	Lrad() {}
	~Lrad() {}
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	
};

