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

	//角度の増加速度を設定する関数	
    void SetSpeed(float speed)
    {
        m_speed *= speed*0.5f;
	}

	//速度の揺れの時間を設定する関数
    void SetWaveSpeed(float speed)
    {
        m_waveSpeed *= speed*0.8f;
    }

	//勝手に流れていく力を設定する関数	
    void SetForce(float force)
    {
        m_force *= force*0.4f;
	}

    void Reset()
    {
        m_force = 1.0f;
        m_waveTime = 0.0f;
        m_speed = 1.0f;
        m_angle = 0.0f;
        m_waveSpeed = 1.0f;
        m_inputOffset = Vector2::Zero;
	}

private:
    void UpdateTime()
    {
        m_waveTime += g_gameTime->GetFrameDeltaTime();
    }

    void AngleUpdate()
    {
        if (m_angle >= 360.0f)
        {
            m_angle -= 360.0f;
        }
        m_angle += m_waveTime * m_speed;
    }

	void RandomDrift(Batter* batter);

private:	
    DriftType m_type;
	float m_force = 1.0f; // デバフの強さを管理する変数
	float m_waveTime = 0.0f;// デバフの時間経過を管理する変数
	float m_speed = 1.0f; // デバフの強さを管理する変数
	float m_angle = 0.0f; // 入力が流される角度（単位は度）
	float m_waveSpeed = 1.0f; // デバフの時間経過を管理する変数
	Vector2 m_inputOffset = Vector2::Zero; // 入力が流されるオフセット値
};

