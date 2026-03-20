#pragma once
#include "Source/Source.h"
#include"Source/UI/SoundTestUI/SoundTestUI.h"
class SoundTest :public Source
{
public:
	SoundTest() {}
	~SoundTest() {}
	bool Start();
	void Update();
	void Render(RenderContext& rc);
private:
	SoundTestUI* m_soundTestUI;
};

