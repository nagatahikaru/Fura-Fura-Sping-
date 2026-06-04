#pragma once
#include "DebuffPatternBase.h"
#include "Source/UI/InGameUI/InGameUI.h"


/// DebuffAimPatternクラスの実装
/// 照準異常系のデバフパターンを実装するクラスです。
/// 例えば、カーソルが小さくなったり、スイートスポットが小さくなったり、
/// ヒットアシストが弱くなったり、スイングが重くなったりするデバフを実装します。
class DebuffAimPattern : public DebuffPatternBase
{
public:
    enum AimType
    {
		SmallCursor,    // カーソル表示サイズとミート範囲を縮小
		TinySweetSpot,  // スイートスポットが小さくなるデバフ
		HeavySwing,     // スイングが重くなるデバフ
    };

    /// <summary>
    /// デバフの詳細な効果を設定する関数です。
    /// </summary>
	/// <param name="type"> デバフの種類を指定します。
    /// <para>SmallCursor はカーソル表示サイズとミート範囲を縮小するデバフ。</para>
    /// <para>TinySweetSpot はスイートスポットが小さくなるデバフ。</para>
    /// <para>HeavySwing はスイングが重くなるデバフです。</para>
    /// </param>
    void SetType(AimType type);

    void Update(Batter* batter) override;
    void SetMeatRange(float range)
    {
		m_meatRange = powf(0.2, range / 40.0f); // 0.2を基準にして、rangeが大きくなるほどm_meatRangeが小さくなるように調整
    }

    void Reset()
    {
		m_meatRange = 1.0f;
    }

private:

    void UpdateSmallCursor(Batter* batter);

private:	
	InGameUI* m_InGameUI;
    AimType m_type;
	float m_meatRange = 1.0f; // 当たり判定の範囲を追加
};

