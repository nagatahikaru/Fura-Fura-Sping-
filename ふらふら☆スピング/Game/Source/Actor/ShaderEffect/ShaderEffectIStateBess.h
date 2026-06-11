#pragma once
#include "Source/Actor/ActorStateMachine.h"

class ShaderEffectStage;

class ShaderEffectIStateBess : public virtual IState
{
public:
	ShaderEffectIStateBess() : IState() {};
	virtual ~ShaderEffectIStateBess() {};
	virtual void Enter() = 0;
	virtual void Update() = 0;
	virtual void Exit() = 0;

	virtual bool RequestState(uint32_t& request) = 0;


private:
	

protected:	
		ShaderEffectStage* GetShaderEffectStage() const;
};