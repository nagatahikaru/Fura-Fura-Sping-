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
	case Random:
		ApplyRandomMagnet(batter);
		break;
	default:
		break;
	}
}

// Horizontalは左右にカーソルを引き寄せる処理を実装します。
void DebuffMagnetPattern::ApplyRandomMagnet(Batter* batter)
{

	if (m_randomCursorUpdate)
	{
		float angle =
			RandomRange(0.0f, 360.0f)
			* MyNamespace::PI;

		float radius =
			RandomRange(0.0f, m_randomSpotRadius);

		m_randomCursorTargetPos.x =
			cosf(angle) * radius;

		m_randomCursorTargetPos.y =
			sinf(angle) * radius;

		m_randomCursorTargetPos.z = 0.0f;

		m_randomCursorMoveTimer =
			RandomRange(0.5f, m_randomMoveDuration);

		m_randomCursorUpdate = false;
	}

	Vector3 finalPos = batter->GetFinalCursorPosition();

	Vector3 toTarget =	m_randomCursorTargetPos	- finalPos;	

	float distance = toTarget.Length();

	m_randomCursorMoveTimer -=
		g_gameTime->GetFrameDeltaTime();

	if (m_randomCursorMoveTimer <= 0.0f
		|| distance <= 0.1f)
	{
		m_randomCursorUpdate = true;
		batter->SetMagnetCursorOffset(Vector3::Zero);
	}

	batter->SetMagnetCursorOffset(toTarget * 0.005f);
}

// ApplyHorizontalMagnet関数は、バッターのカーソルを左右に引き寄せる処理を実装します。
void DebuffMagnetPattern::ApplyHorizontalMagnet(Batter* batter)
{

	if (m_randomCursorUpdate)
	{
		float angle =
			RandomRange(0.0f, 360.0f)
			* MyNamespace::PI;

		float radius =
			RandomRange(0.0f, m_randomSpotRadius);

		m_randomCursorTargetPos.x =
			cosf(angle) * radius;

		m_randomCursorTargetPos.y =
			0.0f;

		m_randomCursorTargetPos.z = 0.0f;

		m_randomCursorMoveTimer =
			RandomRange(0.5f, m_randomMoveDuration);

		m_randomCursorUpdate = false;
	}

	Vector3 finalPos = batter->GetFinalCursorPosition();

	Vector3 toTarget = m_randomCursorTargetPos - finalPos;

	float distance = toTarget.Length();

	m_randomCursorMoveTimer -=
		g_gameTime->GetFrameDeltaTime();

	if (m_randomCursorMoveTimer <= 0.0f
		|| distance <= 0.1f)
	{
		m_randomCursorUpdate = true;
		batter->SetMagnetCursorOffset(Vector3::Zero);
	}

	batter->SetMagnetCursorOffset(toTarget * 0.005f);
}

// ApplyVerticalMagnet関数は、バッターのカーソルを上下に引き寄せる処理を実装します。
void DebuffMagnetPattern::ApplyVerticalMagnet(Batter* batter)
{

	if (m_randomCursorUpdate)
	{
		float angle =
			RandomRange(0.0f, 360.0f)
			* MyNamespace::PI;

		float radius =
			RandomRange(0.0f, m_randomSpotRadius);

		m_randomCursorTargetPos.x =
			0.0f;

		m_randomCursorTargetPos.y =
			sinf(angle) * radius;

		m_randomCursorTargetPos.z = 0.0f;

		m_randomCursorMoveTimer =
			RandomRange(0.5f, m_randomMoveDuration);

		m_randomCursorUpdate = false;
	}

	Vector3 finalPos = batter->GetFinalCursorPosition();

	Vector3 toTarget = m_randomCursorTargetPos - finalPos;

	float distance = toTarget.Length();

	m_randomCursorMoveTimer -=
		g_gameTime->GetFrameDeltaTime();

	if (m_randomCursorMoveTimer <= 0.0f
		|| distance <= 0.1f)
	{
		m_randomCursorUpdate = true;
		batter->SetMagnetCursorOffset(Vector3::Zero);
	}

	batter->SetMagnetCursorOffset(toTarget * 0.005f);
}