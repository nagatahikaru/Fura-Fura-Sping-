#pragma once
#include "DebuffPatternBase.h"

/// DebuffNoisePatternクラスの実装
/// ノイズ系のデバフパターンを実装するクラスです。
/// 例えば、カーソルがランダムにノイズのように動くデバフを実装します。
class DebuffNoisePattern : public DebuffPatternBase
{
public:

    enum NoiseType
    {
		Noise_Vertical,    // カーソルが上下にノイズのように動くデバフ
		Noise_Horizontal,  // カーソルが左右にノイズのように動くデバフ
		Noise_Random       // カーソルがランダムにノイズのように動くデバフ
    };

    /// <summary>
    /// ノイズの方向性を設定する関数です。
    /// </summary>
    /// <param name="type"> デバフの種類を指定します。
    /// <para>Noise_Vertical はカーソルが上下にノイズのように動くデバフ。</para>
    /// <para>Noise_Horizontal はカーソルが左右にノイズのように動くデバフ。</para>
    /// <para>Noise_Random はカーソルがランダムにノイズのように動くデバフ。</para>
    /// </param>
    void SetType(NoiseType type);

    void Update(Batter* batter) override;

	// ノイズの更新タイマーを設定する関数です。
	// float timer はノイズの更新間隔を秒単位で指定します。
	// 基本は0.05f程度が適切ですが、デバフの強さやゲームのバランスに応じて調整してください。
    void SetNoiseTimer(float timer)
    {        
		m_noiseDuration = timer;
	}

    void Reset()
    {
        m_timer = 0.0f;
		m_noiseDuration = 0.05f;
        m_target = Vector3::Zero;
        m_current = Vector3::Zero;
	}

private:

    void NoiseVertical(Batter* batter);
	void NoiseHorizontal(Batter* batter);
	void NoiseRandom(Batter* batter);

private:
    NoiseType m_type;
	// ノイズの更新タイマー
	float m_timer = 0.0f;
	float m_noiseDuration = 0.05f; // ノイズの持続時間
	Vector3 m_target = Vector3::Zero; // ノイズの目標位置
	Vector3 m_current = Vector3::Zero; // 現在のノイズオフセット
};

