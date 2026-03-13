/**
* Actor.h
* 見た目が存在するオブジェクトの基底クラス
*/
#pragma once
#include "Source/Actor/ActorStateMachine.h"
#include "BatterIStateBess.h"


class Batter;


class BatterStateMachine : public StatePatternBase
{
	public:
		BatterStateMachine();
		~BatterStateMachine();
		void Update() override;
		void SetBatter(Batter*player)
		{
			m_batter = player;
		}

		Batter* GetBatter()const
		{
			return m_batter;
		}

private:
	Batter* m_batter = nullptr;
};

class BatterIdleState :public BatterIStateBess
{
	appState(BatterIdleState);

public:
	BatterIdleState() :BatterIStateBess() {};
	virtual ~BatterIdleState() {};

	virtual void Enter()override;
	virtual void Update()override;
	virtual void Exit()override;

	virtual bool RequestState(uint32_t& request)override;
};

class BatterRotationState :public BatterIStateBess
{
	appState(BatterRotationState);

public:
	BatterRotationState():BatterIStateBess() {};
	virtual ~BatterRotationState() {};

	virtual void Enter()override;
	virtual void Update()override;
	virtual void Exit()override;

	virtual bool RequestState(uint32_t& request)override;
};

//class BatterAttackState :public BatterIStateBess
//{
//	appState(BatterAttackState);
//
//public:
//	BatterAttackState() :BatterIStateBess() {};
//	virtual ~BatterAttackState() {};
//
//	virtual void Enter()override;
//	virtual void Update()override;
//	virtual void Exit()override;
//	
//	virtual bool RequestState(uint32_t& request)override;
//};
//
//class BatterDamageState :public BatterIStateBess
//{
//	appState(BatterDamageState);
//
//public:
//	BatterDamageState() :BatterIStateBess() {};
//	virtual ~BatterDamageState() {};
//
//	virtual void Enter()override;
//	virtual void Update()override;
//	virtual void Exit()override;
//
//	virtual bool RequestState(uint32_t& request)override;
//};
//
