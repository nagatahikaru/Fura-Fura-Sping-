#include "stdafx.h"
#include "DebuffLagPattern.h"
#include "Source/Actor/Character/Batter/Batter.h"

void DebuffLagPattern::SetType(LagType type)
{
	m_type = type;
}

// Update関数は、デバフの種類に応じて、処理を実装します。
void DebuffLagPattern::Update(Batter* batter)
{
	switch (m_type)
	{
	case Vertical:		
		break;
	case Horizontal:		
		break;
	case Random:		
		break;
	default:
		break;
	}
}
