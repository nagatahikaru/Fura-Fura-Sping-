#pragma once
#include "Source/Actor/ActorStateMachine.h"


class Batter;
class BatterStateMachine;  // ‘O•ûéŒ¾‚ğ’Ç‰Á

class BatterIStateBess : public virtual IState
{
public:
	BatterIStateBess() : IState() {};
	virtual ~BatterIStateBess() {};

	virtual void Enter() = 0;
	virtual void Update() = 0;
	virtual void Exit() = 0;

	virtual bool RequestState(uint32_t& request) = 0;


private:
	

protected:
	Batter* GetBatter() const;
	float m_swingTimer = 0.0f;

};