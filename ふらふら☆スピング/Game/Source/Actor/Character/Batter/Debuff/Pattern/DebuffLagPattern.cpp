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
	case Delay:		
		LagCursor(batter);
		break;
	default:
		LagCursor(batter);
		break;
	}
}

void DebuffLagPattern::LagCursor(Batter* batter)
{
	m_inputHistory.push_back(batter->GetInputScale());

	const int delayFrame = 12;

	if (m_inputHistory.size() > delayFrame)
	{
		Vector2 delayedInput = m_inputHistory.front();

		m_inputHistory.pop_front();
		batter->SetDelayFrag(true);
		batter->SetInputMoveScale(Vector2(delayedInput.x, delayedInput.y));
	}

}
