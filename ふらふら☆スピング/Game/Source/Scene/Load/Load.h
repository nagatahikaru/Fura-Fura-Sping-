#pragma once
#include "Source/Source.h"
#include "Source/Scene/InGame/Game.h"
class Load : public Source
{
public:
    bool Start() override;
    void Update() override;
    void Render(RenderContext& rc) override;
    void SetDifficulty(Difficulty diff) { m_difficulty = diff; }
private:
    SpriteRender m_spriteRender;
    SpriteRender m_gaugeFill;   // 中身（伸びる）
    SpriteRender m_gaugeFrame;  // 枠（固定）
    SpriteRender m_guruguru;
    SpriteRender m_B;
    SpriteRender m_grobu;
    SpriteRender m_guL;
    SpriteRender m_guR;
    SpriteRender m_guB;
    SpriteRender m_guA;
    SpriteRender m_koke;
    SpriteRender m_siri;
    float m_loadProgress = 0.0f; // 0.0 ～ 1.0
    float m_blinkTimer = 0.0f;
    SoundSource* m_bgm = nullptr;
    int m_loadStep = 0;
    int m_waitFrame = 0;
    float m_finishWait = 0.0f;
    bool m_loadFinished = false;
    bool m_bgmStarted = false;
    float m_realProgress = 0.0f;     // 実際のロード進行
    float m_displayProgress = 0.0f;  // ゲージに表示する進行
    float alpha = 0.0f;
    Difficulty m_difficulty;
    float m_timer;
    bool m_isHappened = false; // 演出が発生したかどうかを記録
    int m_luckyImage = 0;      // 0:なし, 1:こけ画像, 2:しりもち画像
};
