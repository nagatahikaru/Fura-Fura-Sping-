#pragma once
#include "Source/Actor/ActorStateMachine.h"

class DebuffStage;

class DebuffIStateBess : public virtual IState
{
public:
	DebuffIStateBess() : IState() {};
	virtual ~DebuffIStateBess() {};
	virtual void Enter() = 0;
	virtual void Update() = 0;
	virtual void Exit() = 0;

	virtual bool RequestState(uint32_t& request) = 0;


private:
	

protected:	
		DebuffStage* GetDebuffStage() const;
};