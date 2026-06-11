#pragma once

#include "Source/Actor/ActorStateMachine.h"
#include "ShaderEffectIStateBess.h"

class ShaderEffectStage;

class ShaderEffectStageStateMachine: public StatePatternBase
{
public:
	ShaderEffectStageStateMachine();
	~ShaderEffectStageStateMachine();
	void Update() override;


	void SetShaderEffectStage(ShaderEffectStage* debuffStage)
	{
		m_shaderEffectStage = debuffStage;
	}
	ShaderEffectStage* GetShaderEffectStage()const
	{
		return m_shaderEffectStage;
	}

private:
	
	ShaderEffectStage* m_shaderEffectStage = nullptr;
};

class ShaderEffectStageState :public ShaderEffectIStateBess
{
	appState(ShaderEffectStageState);

public:
	ShaderEffectStageState(){};
	virtual ~ShaderEffectStageState() {};
	virtual void Enter()override;
	virtual void Update()override;
	virtual void Exit()override;
	virtual bool RequestState(uint32_t& request)override;

	int m_stageLevel = -1;
};