#include "stdafx.h"
#include "Source/Actor/ActorStateMachine.h"
#include "BatterStateMachine.h"
#include "Batter.h" 

namespace {
	float LSTICK_MIN_THRESHOLD = 0.000001f;
	float RSTICK_MIN_THRESHOLD = 0.000001f;
}


BatterStateMachine::BatterStateMachine()
{
	RegisterState<BatterIdleState>();
	RegisterState<BatterRotationState>();
	RegisterState<BatterSwingState>();
	RegisterState<BatterCursorSetState>();
	RegisterState<BatterReplayState>();
	m_currentState = FindState(BatterIdleState::ID());
	
}

BatterStateMachine::~BatterStateMachine()
{
	
}

void BatterStateMachine::Update()
{
	_ASSERT(m_currentState != nullptr);
	if (m_currentState) {
		uint32_t request;
		if (m_currentState->RequestState(request)) {
			m_currentState->Exit();
			m_currentState = FindState(request);
			m_currentState->Enter();
		}
		m_currentState->Update();
	}
}

void BatterIdleState::Enter()
{ 
	Batter* batter = GetBatter();
	batter->SetPlayAnimation(batter->GetEnAnimationClip());
	
}

void BatterIdleState::Update()
{
	Batter* batter = GetBatter();
	batter->AnimationUpdate();
	
}

void BatterIdleState::Exit()
{
	Batter* batter = GetBatter();
	batter->SetPlayAnimation(batter->GetEnAnimationClip());
	
}

//待機状態からの状態遷移判定。
//スティック入力で回転状態。
//戻り値：状態遷移が発生したらtrue、しなかったらfalseを返す。
bool BatterIdleState::RequestState(uint32_t& request)
{
	Batter* batter = GetBatter();
	Game* game = FindGO<Game>("game");
	if (batter->GetIsReplay())
	{
		request = BatterReplayState::ID();
		return true;
	}

	if (batter->GetRotationSeen())
	{
		request = BatterRotationState::ID();
		return true;
	}

	if (g_pad[0]->IsTrigger(enButtonA))
	{
		
		if (game && game->m_isInputLocked)
		{
			if (game->GetCurrentShotIndex() == 2 && game->m_isInputLocked)
			{
				return false; // 完全に無視して待機状態を維持
			}
		}
		if (game) {
			int shot = game->GetCurrentShotIndex();
			int frame = game->GetCurrentReplayRecordFrame();

			// ★ このフレームの swingTriggered を true にする
			if (frame < game->m_replayFrames[shot].size()) {
				game->m_replayFrames[shot][frame].swingTriggered = true;
			}
		}

		request = BatterSwingState::ID();
		return true;
	}

	if (!batter->GetRotationSeen())
	{
		request = BatterCursorSetState::ID();
		return true;
	}
	return false;
}

void BatterRotationState::Enter()
{
	Batter* batter = GetBatter();
	batter->SetPlayRotation();
	batter->SetPlayAnimation(batter->GetEnAnimationClip());
}

void BatterRotationState::Update()
{
	Batter* batter = GetBatter();	
	batter->Rotation();
	batter->RotationUpdate();
	batter->RoundAndRoundBat();
	batter->AnimationUpdate();
}

void BatterRotationState::Exit()
{
	Batter* batter = GetBatter();
	batter->RotationUpdate();
}

bool BatterRotationState::RequestState(uint32_t& request)
{
	float lx = g_pad[0]->GetLStickXF();
	float ly = g_pad[0]->GetLStickYF();
	Batter* batter = GetBatter();

	if (lx< 0.0f && ly < 0.0f)
	{
		request = BatterIdleState::ID();
		return true;
	}
	if (!batter->GetRotationSeen())
	{
		request = BatterCursorSetState::ID();
		return true;
	}
	return false;
}

void BatterCursorSetState::Enter()
{
	Batter* batter = GetBatter();
	batter->SetIdleAnimation();

}

void BatterCursorSetState::Update()
{
	Batter* batter = GetBatter();
	batter->DownArrowEffect();
	batter->UpdateCursor3D();   // ★ 位置更新だけ
	batter->BatHitBoxPosition(); // ★ 当たり判定の位置更新
	batter->AnimationUpdate();
}

void BatterCursorSetState::Exit()
{
}

bool BatterCursorSetState::RequestState(uint32_t& request)
{
	Batter* batter = GetBatter();
	Game* game = FindGO<Game>("game");
		if (g_pad[0]->IsTrigger(enButtonA))
	{
			if (game && game->m_isInputLocked)
			{
				if (game->GetCurrentShotIndex() == 2 && game->m_isInputLocked)
				{
					return false; // 遷移を拒否
				}
			}
		request = BatterSwingState::ID();
		return true;
	}
	if (batter->GetIsOnGround())
	{
		request = BatterIdleState::ID();
		return true;
	}
	return false;
}

void BatterSwingState::Enter()
{
	Batter* batter = GetBatter();
	batter->GetCharacterModel()->GetModelRender()->SetAnimationSpeed(4.0f);
	batter->Swing();
	batter->RotationUpdate();
	batter->SetPlayAnimation(batter->GetEnAnimationClip());
	m_swingTimer = 0.0f;
	// ★★★ ここを修正・追記 ★★★
	Game* game = FindGO<Game>("game");
	if (game) {
		int shot = game->GetCurrentShotIndex();
		int frame = game->GetCurrentReplayRecordFrame();

		game->SetSwingFrame(shot, frame); // 既存のフレーム記録
		game->SetHasSwung(shot, true);    // ★実際に振ったフラグを true に！
	}
}

void BatterSwingState::Update()
{
	Batter* batter = GetBatter();
	// ★ スイングアニメを4.0倍速にする（見た目は最後まで振り切る）
	batter->GetCharacterModel()->GetModelRender()->SetAnimationSpeed(4.0f);
	batter->AnimationUpdate();
	// 1. スイング開始からの経過時間（秒）を計算
	// 4.0倍速で再生されているため、実際の経過時間に4.0倍を掛けて累積します
	m_swingTimer += g_gameTime->GetFrameDeltaTime() * 4.0f;

	// 2. 経過秒数を「60fps換算のフレーム数」に変換
	float elapsedFrames = m_swingTimer * 60.0f;

	// ★★★ 【ここを変更】〇〇フレーム以降は打てない処理 ★★★
	// スイング開始から「8フレーム」が経過するまでは打てる（HitBatを実行する）。
	// 8フレームを超えたら（9フレーム目以降は）HitBatが呼ばれないため、フォロースルーとなり打てなくなります。
	if (elapsedFrames <= 20.0f)
	{
		batter->DownArrowEffect();
		batter->BatHitBoxPosition();
		batter->HitBat(); // ← ここで実際に打てる判定を行う
	}
	else
	{
	}
}
void BatterSwingState::Exit()
{
	Batter* batter = GetBatter();
	// ★ アニメ速度を元に戻す
	   // ★ スイングアニメだけ 1.0倍速
	batter->GetCharacterModel()->GetModelRender()->SetAnimationSpeed(1.0f);
	m_swingTimer = 0.0f;
}

bool BatterSwingState::RequestState(uint32_t& request)
{
	Batter* batter = GetBatter();

	// ★ スイングアニメが終わったら Idle に戻す
	if (!batter->IsSwingAnimationPlaying())
	{
		request = BatterIdleState::ID();
		return true;
	}

	return false;
}

void BatterReplayState::Enter()
{
	Batter* batter = GetBatter();
	batter->SetPlayAnimation(batter->GetEnAnimationClip());
}

void BatterReplayState::Update()
{
	Batter* batter = GetBatter();
	batter->AnimationUpdate();
	batter->DownArrowEffect();

	if (!m_isReplayHitEffectPlayed)
	{
		Vector3 hitPos = batter->GetHitPosition();
		Vector3 ballPos = batter->GetBall()->GetPosition();

		float dist = (ballPos - hitPos).Length();

		if (dist < batter->GetMeatRange())
		{
			batter->HitEffect(hitPos);
			m_isReplayHitEffectPlayed = true;
		}
	}
}

void BatterReplayState::Exit()
{
	Batter* batter = GetBatter();
	batter->SetPlayAnimation(batter->GetEnAnimationClip());
}

bool BatterReplayState::RequestState(uint32_t& request)
{
	// ★ リプレイ中は状態遷移しない
	return false;
}
