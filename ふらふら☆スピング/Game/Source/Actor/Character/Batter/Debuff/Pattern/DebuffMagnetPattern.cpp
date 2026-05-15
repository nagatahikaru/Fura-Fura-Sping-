#include "stdafx.h"
#include "DebuffMagnetPattern.h"
#include "Source/Actor/Character/Batter/Batter.h"

/// <summary>
/// この関数は誘導系のデバフの処理を行います。
/// Verticalはカーソルを上下に、Horizontalは左右に、
/// Heavyはランダムな方向にカーソルを引き寄せる処理を実装します。
/// </summary>

namespace MyNamespace
{
	float PI = 3.1415f / 180.0f;
	float ZERO_FLOAT = 0.0f;

}

void DebuffMagnetPattern::SetType(MagnetType type)
{
	m_type = type;
}

// Update関数は、デバフの種類に応じて、バッターのカーソルを上下や左右に震わせる処理を実装します。
void DebuffMagnetPattern::Update(Batter* batter)
{
	switch (m_type)
	{
	case Vertical:
		ApplyVerticalMagnet(batter);
		break;
	case Horizontal:
		ApplyHorizontalMagnet(batter);
		break;
	case Heavy:
		ApplyRandomMagnet(batter);
		break;
	default:
		break;
	}
}

//
void DebuffMagnetPattern::ApplyRandomMagnet(Batter* batter)
{
	//ランダムなベクトルを取得
	if (m_randomCursorUpdate)
	{
		//そのベクトルに向かって移動
		// 基準点から半径250の円範囲内でランダムに出現位置を決定
		//ランダムな角度と距離を生成
		float angle = (rand() % 360) * MyNamespace::PI;
		float radius = RandomRange(0, m_randomSpotRadius);

		// 円の中のランダム位置を生成

		m_randomCursorTargetPos.x = cosf(angle) * radius;
		m_randomCursorTargetPos.y = sinf(angle) * radius;
		m_randomCursorMoveTimer = RandomRange(0, m_randomMoveDuration);
		m_randomCursorUpdate = false;
	}
	Vector3 toTarget = m_randomCursorTargetPos - batter->GetMeetCursorPosition();
	float distance = toTarget.Length();

	// 少しずつ寄せる（ここがデバフの強さ）
	m_randomCursorMovePwer = toTarget * 0.05f;
	batter->AddCursorOffset(m_randomCursorMovePwer);
	//一定時間経過後、再度ランダムなベクトルを取得
	m_randomCursorMoveTimer -= g_gameTime->GetFrameDeltaTime();
	if (m_randomCursorMoveTimer <= MyNamespace::ZERO_FLOAT || distance <= 0.0f)
	{
		m_randomCursorUpdate = true;
	}
}

void DebuffMagnetPattern::ApplyHorizontalMagnet(Batter* batter)
{
	//ランダムなベクトルを取得
	if (m_randomCursorUpdate)
	{
		//そのベクトルに向かって移動
		// 基準点から半径250の円範囲内でランダムに出現位置を決定
		//ランダムな角度と距離を生成
		float angle = (rand() % 360) * MyNamespace::PI;
		float radius = RandomRange(0, m_randomSpotRadius);

		// 円の中のランダム位置を生成

		m_randomCursorTargetPos.x = cosf(angle) * radius;
		m_randomCursorTargetPos.y = 0.0f;
		m_randomCursorMoveTimer = RandomRange(0, m_randomMoveDuration);
		m_randomCursorUpdate = false;
	}
	Vector3 toTarget = m_randomCursorTargetPos - batter->GetMeetCursorPosition();
	float distance = toTarget.Length();

	// 少しずつ寄せる（ここがデバフの強さ）
	m_randomCursorMovePwer = toTarget * 0.05f;
	batter->AddCursorOffset(m_randomCursorMovePwer);
	//一定時間経過後、再度ランダムなベクトルを取得
	m_randomCursorMoveTimer -= g_gameTime->GetFrameDeltaTime();
	if (m_randomCursorMoveTimer <= MyNamespace::ZERO_FLOAT || distance <= 0.0f)
	{
		m_randomCursorUpdate = true;
	}
}

void DebuffMagnetPattern::ApplyVerticalMagnet(Batter* batter)
{
	//ランダムなベクトルを取得
	if (m_randomCursorUpdate)
	{
		//そのベクトルに向かって移動
		// 基準点から半径250の円範囲内でランダムに出現位置を決定
		//ランダムな角度と距離を生成
		float angle = (rand() % 360) * MyNamespace::PI;
		float radius = RandomRange(0, m_randomSpotRadius);

		// 円の中のランダム位置を生成

		m_randomCursorTargetPos.x = 0.0f;
		m_randomCursorTargetPos.y = sinf(angle) * radius;
		m_randomCursorMoveTimer = RandomRange(0, m_randomMoveDuration);
		m_randomCursorUpdate = false;
	}
	Vector3 toTarget = m_randomCursorTargetPos - batter->GetMeetCursorPosition();
	float distance = toTarget.Length();

	// 少しずつ寄せる（ここがデバフの強さ）
	m_randomCursorMovePwer = toTarget * 0.05f;
	batter->AddCursorOffset(m_randomCursorMovePwer);
	//一定時間経過後、再度ランダムなベクトルを取得
	m_randomCursorMoveTimer -= g_gameTime->GetFrameDeltaTime();
	if (m_randomCursorMoveTimer <= MyNamespace::ZERO_FLOAT || distance <= 0.0f)
	{
		m_randomCursorUpdate = true;
	}
}