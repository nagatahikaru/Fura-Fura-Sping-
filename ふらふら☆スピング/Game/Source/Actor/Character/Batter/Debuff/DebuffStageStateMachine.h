#pragma once

#include "Source/Actor/ActorStateMachine.h"
#include "Source/Actor/Character/Batter/BatterIStateBess.h"
#include "DebuffIStateBess.h"

class Batter;
class DebuffStage;

class DebuffStageStateMachine: public StatePatternBase
{
public:
	DebuffStageStateMachine();
	~DebuffStageStateMachine();
	void Update() override;
	void SetBatter(Batter* player)
	{
		m_batter = player;
	}
	Batter* GetBatter()const
	{
		return m_batter;
	}
	void SetDebuffStageManager(DebuffStageManager* debuffStage)
	{
		m_debuffStage = debuffStage;
	}
	DebuffStageManager* GetDebuffStageManager()const
	{
		return m_debuffStage;
	}

private:
	Batter* m_batter = nullptr;
	DebuffStageManager* m_debuffStage = nullptr;
};

class DebuffStageState :public BatterIStateBess, public DebuffIStateBess
{
	appState(DebuffStageState);

public:
	DebuffStageState() :BatterIStateBess(), DebuffIStateBess() {};
	virtual ~DebuffStageState() {};
	virtual void Enter()override;
	virtual void Update()override;
	virtual void Exit()override;
	virtual bool RequestState(uint32_t& request)override;

	int m_stageLevel = -1;
};