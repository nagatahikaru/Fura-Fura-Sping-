#pragma once
#include "Source/Scene/InGame/Game.h"
class Load : public IGameObject
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
};
