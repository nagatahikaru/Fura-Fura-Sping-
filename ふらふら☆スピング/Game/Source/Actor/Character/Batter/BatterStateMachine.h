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
		//GetBatter関数を使うには、BatterIStateBess.Cppファイル内でコメント通りに記載してください。
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

class BatterSwingState :public BatterIStateBess
{
	appState(BatterSwingState);
private:
	float m_swingTimer = 0.0f;
public:
	BatterSwingState() :BatterIStateBess() {};
	virtual ~BatterSwingState() {};

	virtual void Enter()override;
	virtual void Update()override;
	virtual void Exit()override;

	virtual bool RequestState(uint32_t& request)override;
};

class BatterCursorSetState :public BatterIStateBess
{
	appState(BatterCursorSetState);

public:
	BatterCursorSetState() :BatterIStateBess() {};
	virtual ~BatterCursorSetState() {};
	virtual void Enter()override;
	virtual void Update()override;
	virtual void Exit()override;
	virtual bool RequestState(uint32_t& request)override;
};

class BatterReplayState :public BatterIStateBess
{
	appState(BatterReplayState);
	
private:
	bool m_isReplayHitEffectPlayed = false; // リプレイ中のヒットエフェクト再生フラグ
	
public:
	BatterReplayState() :BatterIStateBess() {};
	virtual ~BatterReplayState() {};
	virtual void Enter()override;
	virtual void Update()override;
	virtual void Exit()override;
	virtual bool RequestState(uint32_t& request)override;
};