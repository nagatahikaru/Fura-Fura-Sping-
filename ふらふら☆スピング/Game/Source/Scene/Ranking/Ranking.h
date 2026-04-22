#pragma once
#include "Source/Source.h"

class Ranking : public Source
{
public:
	Ranking() {}
	~Ranking() {}
	bool Start();
	void Update();
	void Render(RenderContext& rc);
private:
	
};
