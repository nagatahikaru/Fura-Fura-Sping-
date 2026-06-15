#include "stdafx.h"
#include "Pitcher.h"
#include"Source/Scene/InGame/Game.h"
#include "Source/Actor/Character/Batter/Batter.h"



Pitcher::Pitcher()
{
}

Pitcher::~Pitcher()
{
}

bool Pitcher::Start()
{
	// アニメーション初期化
	for (int j = 0; j < enAnimationClip_Num; j++)
	{
		if (j == enAnimationClip_Throw)
		{
			PITCHER::InitAnimation(m_animationClips, j, false);
		}
		else
		{
			PITCHER::InitAnimation(m_animationClips, j, true);
		}
	}

	//初期位置の設定
	PITCHER::InitModelRender(
		&m_modelRender[m_UniformNumber],
		m_animationClips,
		enAnimationClip_Num,
		Vector3(-0.0f, 140.0f, 1000.0f),
		Vector3(10.0f, 10.0f, 10.0f),
		PITCHER::GetBatterUniformNumberFilePath(m_UniformNumber));


	//アニメーション再生
	m_modelRender[m_UniformNumber].PlayAnimation(enAnimationClip_Idle);

	m_timer = 0.0f;
	m_isThrowing = false;

	m_modelRender->SetPosition(Vector3{ 0.0f,150.0f,1100.0f });

	return true;
}

void Pitcher::Update()
{
	if (m_isPaused) {
		return; // 完全停止
	}

	float dt = 1.0f / 60.0f;

	// ★ ポーズ中はアニメーションを止める
	Game* game = FindGO<Game>("game");
	if (game && game->m_isPaused) {
		return;   // ← これで投球アニメが途中で停止する
	}

	// 🌟【ここを追加】5秒間の操作確認フェーズ中は、ピッチャーの処理を完全にストップさせる
	if (game && game->IsReplayPlaying()) {
		// アニメーションの更新（Idle）だけは行い、タイマー更新や投球判定には進ませない
		m_modelRender[m_UniformNumber].Update();
		return;
	}

	// 🌟【ここも注意】ゲームがまだ始まっていない（ぐるぐる中など）もタイマーを進めない
	if (game && !game->IsGameStarted()) {
		m_modelRender[m_UniformNumber].Update();
		return;
	}

	Batter* batter = FindGO<Batter>("batter");
	if (batter && batter->GetRotationSeen()) {
		SetPlayAnimation(enAnimationClip_Idle);
		AnimationUpdate();
		return;
	}

	m_timer += dt;

	if (!m_isThrowing && m_timer > 2.0f)
	{
		SetPlayAnimation(enAnimationClip_Throw);
		m_isThrowing = true;
		m_timer = 0.0f;
		// ★★★ ここで録画開始 ★★★
		Game* game = FindGO<Game>("game");
		if (game) {
			game->StartReplayRecording();
			game->SetGameStarted(true);   // ★ これを足す
			// ★ 投球開始フレームを記録
			int shotIndex = game->GetShots();
			game->m_pitchFrame[shotIndex] = game->GetReplayFrameCount();
		}
	}

	if (m_isThrowing)
	{
		if (m_timer > 1.0f)
		{
			m_isThrowing = false;
		}
	}
	m_modelRender[m_UniformNumber].Update();
}

void Pitcher::SetPlayAnimation(int enAnimationClip)
{
	m_modelRender[m_UniformNumber].PlayAnimation(enAnimationClip);
}

void Pitcher::AnimationUpdate()
{
	// Pitcher のアニメーション更新
	m_modelRender[m_UniformNumber].Update();
}

void Pitcher::ResetThrow()
{
	m_timer = 0.0f;
	m_isThrowing = false;

	// Idle に戻す
	SetPlayAnimation(enAnimationClip_Idle);
}

void Pitcher::PlayPitchAnimation()
{
	// ★ 投球アニメーションを最初から再生
	m_modelRender[m_UniformNumber].PlayAnimation(enAnimationClip_Throw, 0.0f);
}

void Pitcher::Render(RenderContext& rc)
{
	//モデルの描画
	m_modelRender[m_UniformNumber].Draw(rc);
}