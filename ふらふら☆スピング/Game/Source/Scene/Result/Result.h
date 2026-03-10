#pragma once
#include "Source/Source.h"

class Result : public Source
{
public:
	Result() {}
	~Result() {}
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	
};

