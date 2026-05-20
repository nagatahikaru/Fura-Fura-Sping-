#pragma once
#include "DebuffPatternBase.h"


// DebuffDriftPatternクラスの実装
// 入力が流される系のデバフパターンを実装するクラスです。
// 例えば、Y軸に対して入力が流されるデバフや、X軸に対して入力が流されるデバフ、
// または両方の軸に対して入力が流されるデバフなどがあります。
class DebuffDriftPattern : public DebuffPatternBase
{
public:

    enum DriftType
    {
        Vertical,
        Horizontal,
        Random
    };

    /// <summary>
    /// 流される軸を設定する関数です。
    /// </summary>
    /// <param name="type"> 流される軸を指定します。
    /// <para>Vertical はY軸に対して入力が流されるデバフ。</para>
    /// <para>Horizontal はX軸に対して入力が流されるデバフ。</para>
    /// <para>Random はX軸とY軸の両方に対して入力が流されるデバフです。</para>
    /// </param>
    void SetType(DriftType type);

    void Update(Batter* batter) override;

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
    void VerticalDrift(Batter* batter);
    void HorizontalDrift(Batter* batter);
	void RandomDrift(Batter* batter);

private:	
    DriftType m_type;
    float m_waveTime = 0.0f;
};

