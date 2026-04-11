#pragma once
class Load : public IGameObject
{
public:
    bool Start() override;
    void Update() override;
    void Render(RenderContext& rc) override;
private:
    SpriteRender m_spriteRender;

    SpriteRender m_gaugeFill;   // 中身（伸びる）
    SpriteRender m_gaugeFrame;  // 枠（固定）
    SpriteRender m_guruguru;
    SpriteRender m_B;
    SpriteRender m_grobu;
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
};
