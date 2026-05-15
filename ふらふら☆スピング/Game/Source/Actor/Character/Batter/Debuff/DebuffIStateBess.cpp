#include "stdafx.h"
#include "DebuffIStateBess.h"
#include "DebuffStageStateMachine.h"
#include "DebuffStage/DebuffStage.h"



// DebuffIStateBessクラスのGetBatter関数の実装
DebuffStage* DebuffIStateBess::GetDebuffStage() const
{
	//以下にGetDebuffStageをしたいクラスを記載してください。書き方は同様にしてください。	
	DebuffStageStateMachine* debuffStateMachine = dynamic_cast<DebuffStageStateMachine*>(m_owner);

	//以下にif文を追加してください。
	if (debuffStateMachine) {
		return debuffStateMachine->GetDebuffStage();
	}
	return nullptr;
}
