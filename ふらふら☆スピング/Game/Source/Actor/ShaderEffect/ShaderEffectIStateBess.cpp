#include "stdafx.h"
#include "ShaderEffectIStateBess.h"
#include "ShaderEffectStageStateMachine.h"
#include "ShaderEffectStage/ShaderEffectStage.h"



// DebuffIStateBessクラスのGetBatter関数の実装
ShaderEffectStage* ShaderEffectIStateBess::GetShaderEffectStage() const
{
	//以下にGetDebuffStageをしたいクラスを記載してください。書き方は同様にしてください。	
	ShaderEffectStageStateMachine* shaderEffectStateMachine = dynamic_cast<ShaderEffectStageStateMachine*>(m_owner);

	//以下にif文を追加してください。
	if (shaderEffectStateMachine) {
		return shaderEffectStateMachine->GetShaderEffectStage();
	}
	return nullptr;
}
