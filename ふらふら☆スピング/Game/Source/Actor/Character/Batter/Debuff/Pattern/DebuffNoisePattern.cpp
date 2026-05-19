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
	case Vertical:
		Noise(batter);
		break;
	case Horizontal:
		Noise(batter);
		break;
	case Heavy:
		Noise(batter);
		break;
	default:
		break;
	}
}

void DebuffNoisePattern::Noise(Batter* batter)
{		 

}






