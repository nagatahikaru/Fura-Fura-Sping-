class Load : public IGameObject
{
public:
    bool Start() override;
    void Update() override;
    void Render(RenderContext& rc) override;

private:
    SpriteRender m_spriteRender;

    SpriteRender m_gaugeFill;   // ’†giL‚Ñ‚éj
    SpriteRender m_gaugeFrame;  // ˜giŒÅ’èj
    SpriteRender m_guruguru;
    float m_loadProgress = 0.0f; // 0.0 ` 1.0
    float m_blinkTimer = 0.0f;
    SoundSource* m_bgm = nullptr;
};
