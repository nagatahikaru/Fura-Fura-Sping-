#include "stdafx.h"
#include "DebuffNoisePattern.h"
#include "Source/Actor/Character/Batter/Batter.h"

/// <summary>
/// この関数は誘導系のデバフの処理を行います。
/// Verticalはカーソルを上下に、Horizontalは左右に、
/// Heavyはランダムな方向にカーソルを引き寄せる処理を実装します。
/// </summary>




void DebuffNoisePattern::SetType(NoiseType type)
{
	m_type = type;
}

// Update関数は、デバフの種類に応じて、バッターのカーソルを上下や左右に震わせる処理を実装します。
void DebuffNoisePattern::Update(Batter* batter)
{
	switch (m_type)
	{
	case Noise_Vertical:
		NoiseVertical(batter);
		break;
	case Noise_Horizontal:
		NoiseHorizontal(batter);
		break;
	case Noise_Random:
		NoiseRandom(batter);
		break;
	default:
		break;
	}
}

void DebuffNoisePattern::NoiseVertical(Batter* batter)
{
	m_timer -= g_gameTime->GetFrameDeltaTime();

	if (m_timer <= 0.0f)
	{
		float power = GetPower();

		m_target.x =
			0.0f;

		m_target.y =
			RandomRange(-power, power);

		m_timer = m_noiseDuration;
	}

	m_current = Lerp(
		m_current,
		m_target,
		0.2f
	);

	batter->SetNoiseCursorOffset(
		Vector3(m_current.x, m_current.y, 0.0f)
	);
}

void DebuffNoisePattern::NoiseHorizontal(Batter* batter)
{		 
	m_timer -= g_gameTime->GetFrameDeltaTime();

	if (m_timer <= 0.0f)
	{
		float power = GetPower();

		m_target.x =
			RandomRange(-power, power);

		m_target.y =
			0.0f;

		m_timer = m_noiseDuration;
	}

	m_current = Lerp(
		m_current,
		m_target,
		0.2f
	);

	batter->SetNoiseCursorOffset(
		Vector3(m_current.x, m_current.y, 0.0f)
	);
}

void DebuffNoisePattern::NoiseRandom(Batter* batter)
{	
	m_timer -= g_gameTime->GetFrameDeltaTime();
	
	if (m_timer <= 0.0f)
	{
		float power = GetPower();

		m_target.x =
			RandomRange(-power, power);

		m_target.y =
			RandomRange(-power, power);

		m_timer = m_noiseDuration;
	}

	m_current = Lerp(
		m_current,
		m_target,
		0.2f
	);

	batter->SetNoiseCursorOffset(
		Vector3(m_current.x, m_current.y, 0.0f)
	);
}