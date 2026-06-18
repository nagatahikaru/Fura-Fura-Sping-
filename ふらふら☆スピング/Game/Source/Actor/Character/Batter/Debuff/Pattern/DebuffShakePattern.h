#pragma once
#include "DebuffPatternBase.h"


/// DebuffShakePatternクラスの実装
/// 揺れ系のデバフパターンを実装するクラスです。
/// 例えば、カーソルが上下や左右に震えるデバフを実装します。
class DebuffShakePattern : public DebuffPatternBase
{
public:

    enum ShakeType
    {
        Shake_Vertical,
        Shake_Horizontal,        
        Shake_Random
    };

    /// <summary>
    /// 揺れの方向を設定する関数です。
    /// </summary>
    /// <param name="type"> 揺れの方向を指定します。
    /// <para>Shake_Vertical は垂直方向の揺れ。</para>
    /// <para>Shake_Horizontal は水平方向の揺れ。</para>
    /// <para>Shake_Random はランダムな揺れ。</para>
    /// </param>
    void SetType(ShakeType type);

    void Update(Batter* batter) override;

    void Reset()
    {
		m_waveTime = 0.0f;        
	}

private:
    void UpdateTime()
    {
        m_waveTime += g_gameTime->GetFrameDeltaTime();
    }

    // サイン波を生成する関数
    // 例えば、震えの強さを時間と周波数に基づいて変化させるために使用できます。
    float SinWave(float freq)
    {
        return sinf(m_waveTime * freq);
    }

    // コサイン波を生成する関数
    // 例えば、震えの強さを時間と周波数に基づいて変化させるために使用できます。
    float CosWave(float freq)
    {
        return cosf(m_waveTime * freq);
    }
    void VerticalShake(Batter* batter);
    void HorizontalShake(Batter* batter);
	void RandomShake(Batter* batter);

private:	
    ShakeType m_type;
	float m_waveTime = 0.0f;
    float m_timer = 0.0f;
};

