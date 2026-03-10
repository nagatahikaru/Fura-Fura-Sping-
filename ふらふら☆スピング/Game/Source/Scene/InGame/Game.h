#pragma once
#include "Source/Source.h"

class Game : public Source
{
public:
	Game() {}
	~Game() {}
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	
};

