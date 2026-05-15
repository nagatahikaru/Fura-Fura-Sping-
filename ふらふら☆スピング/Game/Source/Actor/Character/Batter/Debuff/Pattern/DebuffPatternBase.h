#pragma once

#include "IDebuffPattern.h"

class DebuffPatternBase: public IDebuffPattern
{
protected:

    float m_time = 0.0f;
    float m_power = 1.0f;
	float m_seismicIntensity = 1.0f;

    void UpdateTime()
    {
        m_time += g_gameTime->GetFrameDeltaTime();
    }

	// サイン波を生成する関数
	// 例えば、震えの強さを時間と周波数に基づいて変化させるために使用できます。
    float SinWave(float freq)
    {
        return sinf(m_time * freq);
    }

	// コサイン波を生成する関数
	// 例えば、震えの強さを時間と周波数に基づいて変化させるために使用できます。
    float CosWave(float freq)
    {
        return cosf(m_time * freq);
    }

	// ランダムな値を生成する関数
    float RandomRange(float min, float max)
    {
        return min + (max - min)
            * (rand() / (float)RAND_MAX);
    }

public:
	//デバフの設定を外部から行うための関数を定義

	// デバフの強さを設定する関数
    void SetPower(float power)
    {
        m_power = power;
	}
    float GetPower() const
    {
        return m_power;
	}

    void SetSeismicIntensity(float intensity)
    {
        m_seismicIntensity = intensity;
    }

    float GetSeismicIntensity() const
    {
        return m_seismicIntensity;
    }

    void ResetTime()
    {
        m_time = 0.0f;
    }

    void ResetPower()
    {
        m_power = 1.0f;
    }

    void ResetSeismicIntensity()
    {
        m_seismicIntensity = 1.0f;
	}
};