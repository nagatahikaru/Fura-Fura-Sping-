#include "stdafx.h"
#include "DebuffAimPattern.h"
#include "Source/Actor/Character/Batter/Batter.h"
#include "Source/UI/InGameUI/InGameUI.h"

/// DebuffAimPatternクラスの実装
/// 照準異常系のデバフパターンを実装するクラスです。
/// 例えば、カーソルが小さくなったり、スイートスポットが小さくなったり、
/// ヒットアシストが弱くなったり、スイングが重くなったりするデバフを実装します。

void DebuffAimPattern::SetType(AimType type)
{
	m_type = type;
}

void DebuffAimPattern::Update(Batter* batter)
{
	switch (m_type)
	{
	case SmallCursor:
		UpdateSmallCursor(batter);
		break;
	default:
		UpdateSmallCursor(batter);
		break;
	}
}

//外部から呼び出される関数で、バッターのカーソル表示サイズとミート範囲を縮小する処理を実装します。
//カーソルサイズの変更はSetMeatRange関数を呼び出して行います。
void DebuffAimPattern::UpdateSmallCursor(Batter* batter)
{
	if(m_InGameUI==nullptr)
	{
		m_InGameUI = FindGO<InGameUI>("inGameUI");
		if(m_InGameUI==nullptr)
		{
			return; // UIが見つからない場合は処理をスキップ
		}
	}
	m_InGameUI->SetCursorScale(m_meatRange);
	// カーソル表示サイズとミート範囲を縮小する処理
	float hitdir = m_meatRange * 100.0f; // 例えば、当たり判定の範囲を縮小する場合
	batter->SetMeatRange(hitdir); // 当たり判定の範囲を設定
	DebugLogFloat("判定デバフ倍率", hitdir);
}