#pragma once
#include "Source/Source.h"
class GameCamera;
class Background;

class Game : public Source
{
public:
	Game() {}
	~Game() {}
	bool Start();
	void Update();
	void Render(RenderContext& rc);
private:
	GameCamera* m_gameCamera; //ƒQ[ƒ€ƒJƒƒ‰B
	Background* m_background;  //”wŒiB
	SkyCube* m_skyCube;
};

