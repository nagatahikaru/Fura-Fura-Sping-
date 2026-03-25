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
	void SetResultValues(int guruguru, int km);
private:
	SpriteRender m_spriteRender;
	int m_guruguru = 0;
	int m_km = 0;
	FontRender m_fontGuruguru;
	FontRender m_fontKm;
	SpriteRender m_grobu;
	SpriteRender m_B;
};

