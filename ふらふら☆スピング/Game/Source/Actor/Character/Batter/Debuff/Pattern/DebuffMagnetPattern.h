#pragma once
#include "DebuffPatternBase.h"

/// DebuffMagnetPatternクラスの実装
/// 誘導系のデバフの処理を行います。
/// 誘導系のデバフは、プレイヤーのカーソルを特定の方向に引き寄せることで、
/// プレイヤーの操作を妨害する効果を持ちます。
class DebuffMagnetPattern : public DebuffPatternBase
{
public:

    

    enum MagnetType
    {
        Vertical,
        Horizontal, 
        Random
    };

    /// <summary>
    /// 誘導の大まかな方向を設定する関数です。
    /// </summary>
    /// <param name="type">誘導の種類を指定します。
    /// <para>Vertical はカーソルを上下に引き寄せるデバフ。</para>
    /// <para>Horizontal はカーソルを左右に引き寄せるデバフ。</para>
    /// <para>Heavy はランダムな方向にカーソルを引き寄せるデバフです。</para>
    /// </param>
    void SetType(MagnetType type);

    /// <summary>
	/// 目的地を決める範囲を設定する関数です。
    /// </summary>
	/// <param name="radius">この関数には直接回転回数を渡すことを想定しています。
    /// <para>0.0f は範囲が最小、40.0f は範囲が最大です。</para>
    /// </param>
    void SetRandomSpotRadius(float radius)
    {
        radius = clamp(radius, 0.0f, 40.0f);

        float t = radius / 40.0f;

        // EaseOut
        t = 1.0f - (1.0f - t) * (1.0f - t);

        m_randomSpotRadius=Lerp(80.0f, 400.0f, t);


	}

    /// <summary>
    /// 目的地の変更頻度を設定する関数です。
    /// </summary>
    /// <param name="duration">この関数には直接回転回数を渡すことを想定しています。
    /// <para>0.0f は範囲が最小、40.0f は範囲が最大です。</para>
    /// </param>
    void SetRandomMoveDuration(float duration)
    {
        duration = clamp(duration, 0.0f, 40.0f);

        float t = duration / 40.0f;

        t = 1.0f - (1.0f - t) * (1.0f - t);

        m_randomMoveDuration =Lerp(5.0f, 0.8f, t);
	}

    void Update(Batter* batter) override;

    void Reset()
    {
        m_randomCursorUpdate = true;
        m_randomSpotRadius = 50.0f;
		m_randomMoveDuration = 6.5f;
		m_randomCursorMoveTimer = 0.0f;
		m_randomCursorMovePwer = Vector3::Zero;
		m_randomCursorTargetPos = Vector3::Zero;
	}

private:

    void ApplyRandomMagnet(Batter* batter);
    void ApplyHorizontalMagnet(Batter* batter);
	void ApplyVerticalMagnet(Batter* batter);

private:	
    MagnetType m_type;

	// ランダムなカーソル移動のための変数
	bool m_randomCursorUpdate = true;        // ランダムなカーソル移動を更新するかどうかのフラグ
	float m_randomCursorMoveTimer = 0.0f;    // ランダムなカーソル移動のタイマー
	float m_randomMoveDuration = 6.5;       // ランダムなカーソル移動の持続時間
	float m_randomSpotRadius = 50.0f;        // ランダムなカーソル移動の半径    
	Vector3 m_randomCursorMovePwer;          // ランダムなカーソル移動の強さ
	Vector3 m_randomCursorTargetPos;         // ランダムなカーソルの目標位置
};

