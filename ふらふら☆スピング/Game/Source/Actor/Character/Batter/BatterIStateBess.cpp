#include "stdafx.h"
#include "BatterIStateBess.h"
#include "BatterStateMachine.h"
#include "Debuff/DebuffStageStateMachine.h"
#include "Batter.h"


// BatterIStateBessクラスのGetBatter関数の実装
Batter* BatterIStateBess::GetBatter() const
{
	//以下にGetBatterをしたいクラスを記載してください。書き方は同様にしてください。
	BatterStateMachine* stateMachine = dynamic_cast<BatterStateMachine*>(m_owner);
	DebuffStageStateMachine* debuffStateMachine = dynamic_cast<DebuffStageStateMachine*>(m_owner);

	//以下にif文を追加してください。
	// GetBatterをしたいクラスが複数ある場合は、if文を追加してください。
	if (stateMachine) {
		return stateMachine->GetBatter();
	}
	if (debuffStateMachine) {
		return debuffStateMachine->GetBatter();
	}
	return nullptr;
}
