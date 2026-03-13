#include "stdafx.h"
#include "BatterIStateBess.h"
#include "BatterStateMachine.h"
#include "Batter.h"

Batter* BatterIStateBess::GetBatter() const
{
	BatterStateMachine* stateMachine = dynamic_cast<BatterStateMachine*>(m_owner);
	if (stateMachine) {
		return stateMachine->GetBatter();
	}
	return nullptr;
}
