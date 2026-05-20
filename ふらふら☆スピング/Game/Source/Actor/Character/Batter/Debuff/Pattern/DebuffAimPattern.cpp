#include "stdafx.h"
#include "DebuffAimPattern.h"
#include "Source/Actor/Character/Batter/Batter.h"

/// DebuffAimPatternクラスの実装
/// 照準異常系のデバフパターンを実装するクラスです。
/// 例えば、カーソルが小さくなったり、スイートスポットが小さくなったり、
/// ヒットアシストが弱くなったり、スイングが重くなったりするデバフを実装します。

void DebuffAimPattern::SetType(AimType type)
{
	m_type = type;
}

// Update関数は、デバフの種類に応じて、バッターのカーソルを上下や左右に震わせる処理を実装します。
void DebuffAimPattern::Update(Batter* batter)
{
	switch (m_type)
	{
	case SmallCursor:
		UpdateSmallCursor(batter);
		break;
	case TinySweetSpot:
		UpdateTinySweetSpot(batter);
		break;
	case WeakHitAssist:
		UpdateWeakHitAssist(batter);
		break;
	case HeavySwing:
		UpdateHeavySwing(batter);
		break;
	default:
		break;
	}
}

//外部から呼び出される関数で、バッターのカーソル表示サイズとミート範囲を縮小する処理を実装します。
//カーソルサイズの変更はSetMeatRange関数を呼び出して行います。
void DebuffAimPattern::UpdateSmallCursor(Batter* batter)
{
	// カーソル表示サイズとミート範囲を縮小する処理	
	batter->SetMeatRange(m_meatRange); // 当たり判定の範囲を設定
}

void DebuffAimPattern::UpdateTinySweetSpot(Batter* batter)
{
	// スイートスポットを小さくする処理
	
}

void DebuffAimPattern::UpdateWeakHitAssist(Batter* batter)
{
	// ヒットアシストを弱くする処理
	
}

void DebuffAimPattern::UpdateHeavySwing(Batter* batter)
{
	// スイングを重くする処理
	
}
