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

//
void DebuffMagnetPattern::ApplyRandomMagnet(Batter* batter)
{

	if (m_randomCursorUpdate)
	{
		float angle =
			RandomRange(0.0f, 360.0f)
			* (MyNamespace::PI / 180.0f);

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

	Vector3 finalPos =
		batter->GetFinalCursorPosition();

	Vector3 toTarget =
		m_randomCursorTargetPos - finalPos;

	float distance = toTarget.Length();

	//-----------------------------------
	// 旧コード寄りの蓄積型
	//-----------------------------------

	Vector3 current =
		batter->GetMagnetCursorOffset();

	float magnetPower = 0.015f;

	current += toTarget * magnetPower;

	//-----------------------------------
	// 暴走防止
	//-----------------------------------

	float maxOffset = 250.0f;

	if (current.Length() > maxOffset)
	{
		current.Normalize();
		current *= maxOffset;
	}

	//-----------------------------------
	// 減衰
	//-----------------------------------

//	current *= 0.92f;

	batter->SetMagnetCursorOffset(current);

	//-----------------------------------

	m_randomCursorMoveTimer -=
		g_gameTime->GetFrameDeltaTime();

	if (m_randomCursorMoveTimer <= 0.0f
		|| distance <= 0.1f)
	{
		m_randomCursorUpdate = true;
	}


	auto pos = toTarget;
	auto offset = m_randomCursorTargetPos;
	auto currentOffset = current;

	char buf[256];

	sprintf_s(
		buf,
		"meet:(%.2f %.2f) offset:(%.2f %.2f) current:(%.2f %.2f)\n",
		pos.x,
		pos.y,
		offset.x,
		offset.y,
		currentOffset.x,
		currentOffset.y
	);

	OutputDebugStringA(buf);
}

void DebuffMagnetPattern::ApplyHorizontalMagnet(Batter* batter)
{
	if (m_randomCursorUpdate)
	{
		float angle =
			RandomRange(0.0f, 360.0f)
			* (MyNamespace::PI / 180.0f);

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

	Vector3 finalPos =
		batter->GetFinalCursorPosition();

	Vector3 toTarget =
		m_randomCursorTargetPos - finalPos;

	float distance = toTarget.Length();

	//-----------------------------------
	// 旧コード寄りの蓄積型
	//-----------------------------------

	Vector3 current =
		batter->GetMagnetCursorOffset();

	float magnetPower = 0.015f;

	current += toTarget * magnetPower;

	//-----------------------------------
	// 暴走防止
	//-----------------------------------

	float maxOffset = 250.0f;

	if (current.Length() > maxOffset)
	{
		current.Normalize();
		current *= maxOffset;
	}

	//-----------------------------------
	// 減衰
	//-----------------------------------

	current *= 0.92f;

	batter->SetMagnetCursorOffset(current);

	//-----------------------------------

	m_randomCursorMoveTimer -=
		g_gameTime->GetFrameDeltaTime();

	if (m_randomCursorMoveTimer <= 0.0f
		|| distance <= 10.0f)
	{
		m_randomCursorUpdate = true;
	}

}

void DebuffMagnetPattern::ApplyVerticalMagnet(Batter* batter)
{
	if (m_randomCursorUpdate)
	{
		float angle =
			RandomRange(0.0f, 360.0f)
			* (MyNamespace::PI / 180.0f);

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

	Vector3 finalPos =
		batter->GetFinalCursorPosition();

	Vector3 toTarget =
		m_randomCursorTargetPos - finalPos;

	float distance = toTarget.Length();

	//-----------------------------------
	// 旧コード寄りの蓄積型
	//-----------------------------------

	Vector3 current =
		batter->GetMagnetCursorOffset();

	float magnetPower = 0.015f;

	current += toTarget * magnetPower;

	//-----------------------------------
	// 暴走防止
	//-----------------------------------

	float maxOffset = 250.0f;

	if (current.Length() > maxOffset)
	{
		current.Normalize();
		current *= maxOffset;
	}

	//-----------------------------------
	// 減衰
	//-----------------------------------

	current *= 0.92f;

	batter->SetMagnetCursorOffset(current);

	//-----------------------------------

	m_randomCursorMoveTimer -=
		g_gameTime->GetFrameDeltaTime();

	if (m_randomCursorMoveTimer <= 0.0f
		|| distance <= 10.0f)
	{
		m_randomCursorUpdate = true;
	}

}