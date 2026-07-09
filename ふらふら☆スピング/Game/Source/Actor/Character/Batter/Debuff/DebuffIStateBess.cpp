#include "stdafx.h"
#include "DebuffIStateBess.h"
#include "DebuffStageStateMachine.h"
#include "DebuffStage/DebuffStageManager.h"



// DebuffIStateBessクラスのGetBatter関数の実装
DebuffStageManager* DebuffIStateBess::GetDebuffStageManager() const
{
	//以下にGetDebuffStageをしたいクラスを記載してください。書き方は同様にしてください。	
	DebuffStageStateMachine* debuffStateMachine = dynamic_cast<DebuffStageStateMachine*>(m_owner);

	//以下にif文を追加してください。
	if (debuffStateMachine) {
		return debuffStateMachine->GetDebuffStageManager();
	}
	return nullptr;
}
