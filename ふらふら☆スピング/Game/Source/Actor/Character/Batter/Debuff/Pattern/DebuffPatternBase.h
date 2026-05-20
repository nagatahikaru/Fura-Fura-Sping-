#pragma once

#include "IDebuffPattern.h"

// ファイル冒頭付近に追加（std::clampが使えない場合のため）
template <typename T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi) {
    return (v < lo) ? lo : (hi < v) ? hi : v;
}

class DebuffPatternBase: public IDebuffPattern
{
protected:

    float m_time = 0.0f;
    float m_power = 10.0f;
	float m_seismicIntensity = 1.0f;


	// ランダムな値を生成する関数
    float RandomRange(float min, float max)
    {
        return min + (max - min)
            * (rand() / (float)RAND_MAX);
    }

    inline float Lerp(float a, float b, float t) {
        return a + (b - a) * t;
    }

    inline Vector3 Lerp(const Vector3& a, const Vector3& b, float t) {
        return a + (b - a) * t;
	}

public:
	//デバフの設定を外部から行うための関数を定義

	// デバフの強さを設定する関数
	// 例えば、震えの強さを設定するために使用できます。
    void SetPower(float power)
    {
        m_power = 10*power;
	}

    float GetPower() const
    {
        return m_power;
	}

	// 震えの強さを設定する関数
    void SetSeismicIntensity(float intensity)
    {
        m_seismicIntensity = intensity;
    }

    float GetSeismicIntensity() const
    {
        return m_seismicIntensity;
    }

	// 回転数に応じてデバフの強さを設定する関数
    float GetRotationRate(float rotation)
    {
        constexpr float kMaxRotation = 40.0f;
        constexpr float kMaxRate = 6.0f;

        rotation = clamp(abs(rotation), 0.0f, kMaxRotation);

        float t = rotation / kMaxRotation;

        return 1.0f + t * (kMaxRate - 1.0f);
    }
};