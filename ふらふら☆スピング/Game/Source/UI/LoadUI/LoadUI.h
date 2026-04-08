#pragma once
#include "Source/Source.h"

class LoadUI :public Source
{
public:
	LoadUI() {}
	~LoadUI() {}
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	// ★追加：外部からヒントを強制的に進める関数
	void AdvanceTip() {
		m_currentTip = (m_currentTip + 1) % m_tips.size();
		m_timer = 0.0f; // 自動タイマーもリセット
	}
private:
	std::vector<SpriteRender*> m_tips;
	int m_currentTip = 0;
	float m_timer = 0.0f;
};
