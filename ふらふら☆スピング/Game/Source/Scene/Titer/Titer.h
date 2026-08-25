#pragma once
#include "Source/Source.h"
#include "Source/UI/UIManager.h"
#include"Source/Sound/SoundManager.h"
class TiterUI;
class Titer : public Source
{
public:
	Titer() {}
	~Titer() {}
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	//UIAnimationManager* m_uiManager;
	SpriteRender m_spriteRender;
	Transform* m_transform;
	SoundSource* m_bgm=nullptr;
	TiterUI* m_titerUI;
	int m_titleCallFrameCount = 0;
	bool m_bgmStarted = false;
	bool m_titleCallPending = false;
};

