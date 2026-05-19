#include "stdafx.h"
#include "Game.h"
#include "Source/Actor/GameCamera/GameCamera.h"
#include "Source/Actor/Stage/Background.h"
#include "Source/UI/InGameUI/InGameUI.h"
#include "Source/Actor/Character/Batter/Batter.h"
#include "Source/Actor/Character/Pitcher/Pitcher.h"
#include "Source/Actor/Character/Catcher/Catcher.h"
#include "Source/Actor/Character/Ball/Ball.h"
#include "Source/Scene/Result/Result.h"
#include "Source/Sound/SoundManager.h"
#include "Source/UI/PauseUI/PauseUI.h"
#include "Source/UI/SoundTestUI/SoundTestUI.h"
#include "Source/Scene/Start/Start.h"

Game::~Game()
{
	DeleteGO(m_gameCamera);
	DeleteGO(m_background);
	DeleteGO(m_batter);
	DeleteGO(m_pitcher);
	DeleteGO(m_catcher);
	DeleteGO(m_ball);
	DeleteGO(m_skyCube);
	DeleteGO(m_InGameUI);
}

bool Game::Start()
{
	// ★ Load で作ったオブジェクトを取得するだけ
	m_skyCube = FindGO<SkyCube>("skycube");
	m_background = FindGO<Background>("backGround");
	m_gameCamera = FindGO<GameCamera>("gameCamera");
	m_InGameUI = FindGO<InGameUI>("inGameUI");
	m_batter = FindGO<Batter>("batter");
	m_pitcher = FindGO<Pitcher>("pitcher");
	m_ball = FindGO<Ball>("ball");
	m_catcher = FindGO<Catcher>("catcher");

	// ★ カウントダウンUIを表示
	m_start1 = NewGO<Start1>(0, "start1");

	// ボールをカメラにセット
	if (m_gameCamera && m_ball) {
		m_gameCamera->SetBall(m_ball);
	}

	m_replayPaths.resize(3);

	// フラグ類の初期化
	m_ballLandedDuringFade = false;

	return true;
}

void Game::Update()
{
	// 当たり判定の表示
	// PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	// 🌟【安全パッチ：UI更新の強制保証】
	// フェードアウト中、またはフェードイン処理が走っている間は、ゲーム側の数々の早期 return
	// (カウントダウン、ポーズ、ヒットストップ等)に巻き込まれないよう、ここで必ず UI の更新を回します。
	if (m_InGameUI && (m_InGameUI->IsFadingOut() || m_InGameUI->IsFadingIn() || m_fadeInDelayTimer >= 0.0f)) {
		m_InGameUI->Update();
	}

	// ★ ぐるぐる値を毎フレーム Game に保存する
	if (m_batter) {
		SetGuruGuru(m_batter->GetGuruGuru());
	}

	// ★★★ ぐるぐる 5 回刻みで SE3 を鳴らす ★★★
	int g = GetGuruguru();

	// 5 の倍数になった瞬間だけ鳴らす
	if (g > 0 && g % 5 == 0 && g != m_prevGuruGuru) {
		// ★ サウンドテストで SE 音量が 0 のときは鳴らさない
		if (g_soundManager->m_seVolume > 0) {
			g_soundManager->PlaySE(enSound_SE3, 1.0f);
		}
	}

	// 前回値を更新
	m_prevGuruGuru = g;

	if (m_InGameUI) {
		m_InGameUI->SetBallCount(3 - m_shots);
	}

	// ★ カウントダウン中はポーズボタン無効 & ゲームロジック停止
	if (FindGO<Start1>("start1") != nullptr) {
		if (m_InGameUI) {
			m_InGameUI->SetUIVisible(false);
			m_InGameUI->SetFontVisble(false);
			m_InGameUI->SetReplayVisible(false);
		}

		if (m_batter) m_batter->AnimationUpdate();
		if (m_pitcher) m_pitcher->AnimationUpdate();
		if (m_catcher) m_catcher->AnimationUpdate();

		return; // ← START ボタンも完全に無効
	}

	// ★ START ボタン処理（ここに1回だけ）
	if (g_pad[0]->IsTrigger(enButtonStart)) {
		if (FindGO<PauseUI>("pause") != nullptr) return;
		if (FindGO<SoundTestUI>("soundtest") != nullptr) return;

		// ★ ここでポーズ突入SE
		if (g_soundManager && g_soundManager->m_seVolume > 0) {
			g_soundManager->PlaySE(enSound_SE);   // 好きなSEに
		}
		m_isPaused = true;

		if (m_InGameUI) {
			m_InGameUI->SetPause(true);
		}

		NewGO<PauseUI>(0, "pause");
	}

	// ★ ポーズ中はゲーム停止
	if (m_isPaused) {
		return;
	}

	// ★ ヒットストップ処理（ゲーム全体を一瞬停止）
	if (m_hitStopTimer > 0.0f) {
		m_isHitStop = true;   // ← これが絶対必要！
		m_hitStopTimer -= g_gameTime->GetFrameDeltaTime();
		return; // ★ これでゲーム全体が停止する
	}
	else {
		m_isHitStop = false;  // ← 終わったら解除
	}

	if (m_InGameUI && m_InGameUI->IsFadingOut()) {
		// ★ フェードアウト開始した瞬間だけ実行
		if (!m_startFadeSE2) {
			m_startFadeSE2 = true;
		}
		m_timeScale = 1.0f;
		if (m_InGameUI) m_InGameUI->SetBaisokuVisible(false); // フェードアウト時は非表示
	}
	else if (!m_hasTriggered100m) {
		// ★ 100m演出前だけ倍速を許可
		if (m_canFastForward && g_pad[0]->IsPress(enButtonB)) {
			m_timeScale = 10.0f;
			if (m_InGameUI) m_InGameUI->SetBaisokuVisible(true);  // ★ 10倍速UIを表示
		}
		else if (m_canFastForward) {
			m_timeScale = 5.0f;
			if (m_InGameUI) m_InGameUI->SetBaisokuVisible(true);  // ★ 5倍速UIを表示
		}
		else {
			m_timeScale = 1.0f;
			if (m_InGameUI) m_InGameUI->SetBaisokuVisible(false); // ★ 通常速度時は非表示
		}
	}
	else {
		// 100m演出がトリガーされた後（20倍速や100倍速の処理時など）
		if (m_InGameUI) m_InGameUI->SetBaisokuVisible(false);      // ★ 念のため非表示
	}

	switch (m_cameraMode) {
	case Camera_Catcher:
		m_gameCamera->SetCatcherCamera();
		break;

	case Camera_Replay:
		m_gameCamera->SetReplayCamera();
		break;

	case Camera_Ball:
		m_gameCamera->SetFollowBallCamera();
		break;

	case Camera_BackBall:
		m_gameCamera->SetFollowBallBackCamera();
		break;
	}

	if (m_cameraMode == Camera_Catcher) {
		m_cameraType = Camera_Catcher;
		m_InGameUI->SetUIVisible(true);
		m_InGameUI->SetFontVisble(true);
		m_InGameUI->SetReplayVisible(false);
		m_InGameUI->SetniceVisible(false);
	}
	else if (m_cameraMode == Camera_Ball || m_cameraMode == Camera_BackBall) {
		m_cameraType = Camera_Ball;
		m_InGameUI->SetUIVisible(false);
		m_InGameUI->SetFontVisble(true);
		m_InGameUI->SetReplayVisible(false);
	}
	else if (m_cameraMode == Camera_Replay) {
		m_cameraType = Camera_Replay;
		m_InGameUI->SetUIVisible(false);
		m_InGameUI->SetFontVisble(false);
		m_InGameUI->SetReplayVisible(true);
	}
	else if (m_batter->GetRotationSeen()) {
		m_InGameUI->SetUIVisible(false);
		m_InGameUI->SetFontVisble(false);
		m_InGameUI->SetReplayVisible(true);
		m_InGameUI->SetGuruGuruCount(GetGuruguru());
	}

	if (m_isBallLanded && !m_isReplayPlaying) {
		m_afterLandingTimer += (1.0f / 60.0f) * m_timeScale;

		// ★ 修正：打撃ありなら2秒、空振りなら0秒（即時）待つように切り替え
		float waitTime = (m_ball && m_ball->IsHit()) ? 1.3f : 0.0f;

		if (m_afterLandingTimer >= waitTime) { // ⏰ 指定時間経過したら次へ
			if (m_shots == 2) {
				return;
			}

			// 🌟 ゴロ（打撃あり）だった場合、2秒間転がしたあとのここでカメラを戻す
			if (!m_hasTriggered100m && (m_ball && m_ball->IsHit())) {
				m_cameraMode = Camera_Catcher;

				if (m_InGameUI) {
					m_InGameUI->SetUIVisible(true);
					m_InGameUI->SetFontVisble(true);
					m_InGameUI->SetReplayVisible(false);
				}
			}

			// ボール位置やタイマー、ピッチャーの構えをリセット
			ResetForNextShot();

			m_shots++; // リセットが完全に終わった後に次の球へカウントアップ
			return;
		}
	}

	// ★ ボールが飛んでいる間は距離をリアルタイム更新
	if (m_ball && m_ball->m_isMove && m_hasStartedDistance && !m_isBallLanded) {
		float dist = m_hitStartZ - m_ball->GetPosition().z;
		if (dist < 0) dist = 0;

		m_km = dist;

		if (m_InGameUI) {
			m_InGameUI->m_threeShots[m_shots] = m_km;
			m_InGameUI->m_shotDone[m_shots] = true;

			m_InGameUI->SetKm(m_km);
		}
	}

	// ★ フェードイン遅延処理
	if (m_fadeInDelayTimer >= 0.0f) {
		if (m_shots == 2) {
			m_fadeInDelayTimer = -1.0f;
			return;
		}
		m_fadeInDelayTimer -= g_gameTime->GetFrameDeltaTime();

		if (m_fadeInDelayTimer <= 0.0f) {
			// ★ 1.1秒後にフェードイン開始
			if (m_InGameUI) {
				m_InGameUI->StartFadeIn(0.5f);
				m_fadeInDelayTimer = -1.0f;
			}
		}
	}

	if (m_isReplayPlaying) {
		// ▼ スイングタイマーは常に進める
		m_replaySwingTimer += g_gameTime->GetFrameDeltaTime();

		// ▼ ボール再生は遅延が終わるまで止める
		if (m_replayDelayTimer > 0.0f) {
			m_replayDelayTimer -= g_gameTime->GetFrameDeltaTime();

			// ★ スイングだけは遅延中でも再生する
			float swingSec = (m_swingFrame[m_bestShotIndex] - m_pitchFrame[m_bestShotIndex]) / 60.0f;
			if (m_replaySwingTimer >= swingSec && !m_hasPlayedReplaySwing) {
				m_batter->PlaySwingAnimation();
				m_hasPlayedReplaySwing = true;
			}

			// 🌟【重要】遅延時間中（1.2秒間）も、ピッチャーのアニュアルだけは動かす！
			if (m_pitcher) {
				m_pitcher->AnimationUpdate(); // ピッチャーのコマを進める
			}
			if (m_batter) {
				m_batter->AnimationUpdate();
			}

			return; // ← ボールはまだ動かさない（ここで関数を抜ける）
		}

		// ★★★ 遅延が終わった瞬間にボールを打った瞬間の位置へ戻す ★★★
		if (!m_hasAppliedHitMoment) {
			m_ball->SetPosition(m_hitStartPos[m_bestShotIndex]);
			m_ball->SetVelocity(m_hitVelocities[m_bestShotIndex]);
			m_hasAppliedHitMoment = true;
		}

		// ▼ 遅延が終わったのでボール再生開始
		auto& path = m_currentReplay;
		int index = m_replayStartFrame;

		if (index < path.size()) {
			m_ball->SetPosition(path[index]);
		}

		int swingTiming = m_bestSwingFrame - m_bestPitchFrame;

		if (index == swingTiming) {
			m_batter->PlaySwingAnimation();
			m_batter->GetCharacterModel()
				->GetModelRender()
				->SetAnimationSpeed(4.0f);
			m_ball->SetPosition(m_hitStartPos[m_bestShotIndex]);
			m_ball->SetVelocity(m_hitVelocities[m_bestShotIndex]);
			m_ball->m_isMove = true;
			m_ball->m_hasHit = true;
		}

		m_replayStartFrame++;
		m_replayTimer += g_gameTime->GetFrameDeltaTime();

		// 🌟【重要】遅延が明けたあとのリプレイ中も、ピッチャーとバッターを動かし続ける
		if (m_pitcher) m_pitcher->AnimationUpdate();
		if (m_batter)  m_batter->AnimationUpdate();

		if (m_replayTimer >= m_replayDuration) {
			m_isReplayPlaying = false;
			m_cameraMode = Camera_Catcher;
			GoToResult();
			return;
		}

		if (g_pad[0]->IsTrigger(enButtonB)) {
			m_isReplayPlaying = false;
			m_cameraMode = Camera_Catcher;
			GoToResult();
			return;
		}
		return;
	}

	// ★ SE2 フェードアウト（フェード演出中だけ）
	if (m_startFadeSE2 && g_soundManager) {
		g_soundManager->FadeOutSE2(0.003f);
	}

	// ★ 録画中は毎フレームカウンタを進める
	if (m_isRecording) {
		m_replayFrameCounter++;
	}
}

void Game::ResetForNextShot()
{
	m_isBallLanded = false;
	m_afterLandingTimer = 0.0f;
	m_zeroDistanceTimer = 0.0f;
	m_km = 0.0f;
	m_hasTriggered100m = false;
	m_hasStartedDistance = false;
	m_isHomeRun = false;

	// 1. ボールを初期位置に戻す（この内部で UI の位置同期とミート位置リセットが走る）
	if (m_ball) {
		m_ball->ResetBall();
	}

	// 2. ピッチャーのモーションリセット
	Pitcher* pitcher = FindGO<Pitcher>("pitcher");
	if (pitcher) {
		pitcher->ResetThrow();
	}

	// 3. バッター内部のスティック移動累積（m_meetPosition）を完全にゼロリセット
	if (m_batter) {
		m_batter->ResetSwing();
		m_batter->SetCursorMode(true);
	}

	// 4. UI の最終的な帳尻合わせ
	if (m_InGameUI) {
		m_InGameUI->SetKm(0);
		m_InGameUI->SetBaisokuVisible(false);
		// 念のため、ここでもUI側のリセット関数を叩いて同期を保証する
		m_InGameUI->ResetBatAndMeetOnly();
	}
}

void Game::OnBallLanded()
{
	// 🌟【フェードパッチ対応】
	// フェードアウト中にボールが着地したケースを検知し、最低限のスコア保存と、
	// 「フェード中着地フラグ」を立てて早期終了します。
	if (m_InGameUI && m_InGameUI->IsFadingOut()) {
		m_isBallLanded = true;
		m_timeScale = 1.0f;
		m_canFastForward = false;
		m_hasStartedDistance = false;

		m_scores[m_shots] = m_km;
		if (m_InGameUI) {
			m_InGameUI->m_threeShots[m_shots] = m_km;
			m_InGameUI->m_shotDone[m_shots] = true;
		}
		if (m_ball && m_ball->m_replayPath.size() > 0) {
			m_replayPaths[m_shots] = m_ball->m_replayPath;
		}

		// ここが重要：フェード中に着地したことを覚えておく
		m_ballLandedDuringFade = true;
		OutputDebugStringA("Game::OnBallLanded captured during FadeOut\n");
		return;
	}

	m_isBallLanded = true;
	m_afterLandingTimer = 0.0f;
	m_canFastForward = false;
	m_timeScale = 1.0f;
	m_hasStartedDistance = false;

	if (!m_hasTriggered100m) {
		// 🌟 空振りの場合は、この瞬間にカメラとUIをキャッチャー視点に戻す
		if (m_ball && !m_ball->IsHit()) {
			m_cameraMode = Camera_Catcher;

			if (m_InGameUI) {
				m_InGameUI->SetUIVisible(true);
				m_InGameUI->SetFontVisble(true);
				m_InGameUI->SetReplayVisible(false);

				if (m_batter) {
					m_batter->ResetSwing();
				}
				m_InGameUI->ResetBatAndMeetOnly();
			}
		}
		else {
			if (m_InGameUI && m_ball && m_ball->IsHit()) {
				m_InGameUI->ShowPrediction(m_km, true);
			}
		}
	}

	// スコア保存（通常時）
	m_scores[m_shots] = m_km;
	if (m_InGameUI) {
		m_InGameUI->m_threeShots[m_shots] = m_km;
		m_InGameUI->m_shotDone[m_shots] = true;
	}

	if (m_ball && m_ball->m_replayPath.size() > 0) {
		m_replayPaths[m_shots] = m_ball->m_replayPath;
	}
	else {
		m_replayPaths[m_shots].clear();
	}

	if (m_shots == 2) {
		DecideBestReplay();
		if (m_bestShotIndex != -1) {
			m_shouldStartReplay = true;
		}
		else {
			GoToResult();
		}
		StartEndFade();
		return;
	}
}

void Game::OnOver100m()
{
	if (m_gameCamera) {
		m_gameCamera->FreezeCamera();
	}

	if (m_InGameUI) {
		m_InGameUI->StartFadeOut(0.5f);

		m_InGameUI->m_onFadeOutFinished = [this]() {
			// 🌟【フェードパッチ追加】最優先でフェード中の着地復帰を確認
			if (m_ballLandedDuringFade) {
				m_ballLandedDuringFade = false;
				m_cameraMode = Camera_Catcher;
				if (m_gameCamera) m_gameCamera->UnfreezeCamera();
				if (m_InGameUI) m_InGameUI->StartFadeIn(0.5f);
				return;
			}

			// ★ 完全に黒くなった瞬間にカメラ切り替え
			m_cameraMode = Camera_Catcher;

			// ★ カメラの凍結解除
			if (m_gameCamera) {
				m_gameCamera->UnfreezeCamera();
			}

			// ★ ここでリプレイ開始
			if (m_shouldStartReplay) {
				StartReplay(m_bestShotIndex);
				m_shouldStartReplay = false;

				if (m_InGameUI) {
					m_InGameUI->StartFadeIn(0.5f);
				}
				return;
			}

			// ★ フェードアウト完了 → ここで100倍速にする
			m_timeScale = 100.0f;

			if (m_shots == 2) {
				m_fadeInDelayTimer = -1.0f;
				return;
			}

			// ★ フェードインは 1.0 秒後に実行
			m_fadeInDelayTimer = 1.0f;
			};
	}
	m_canFastForward = false;
	m_hasTriggered100m = true;
}

void Game::StartReplay(int index)
{
	m_isRecording = false;
	if (m_ball) {
		m_ball->m_isRecording = false;
	}
	m_replaySwingTimer = 0.0f;
	m_hasPlayedReplaySwing = false;
	m_replayStartFrame = 0;
	m_replayFrameCounter = 0;
	m_isReplayPlaying = true;
	m_replayTimer = 0.0f;
	m_hasAppliedHitMoment = false;

	m_replayDelayTimer = 1.2f;  // 1.2秒待機
	m_replayAccumulator = 0.0f;
	m_cameraMode = Camera_Replay;
	m_currentReplay = m_replayPaths[index];
	m_replayPitchFrame = m_pitchFrame[index];

	if (m_pitcher) {
		m_pitcher->ResetThrow();
		m_pitcher->PlayPitchAnimation();
	}

	if (m_batter) {
		m_batter->ResetSwing();
		m_batter->AnimationUpdate();
	}

	if (m_ball) {
		m_ball->ResetBall();
		m_ball->SetVelocity(Vector3::Zero);
		m_ball->m_isMove = false;
		m_ball->m_hasHit = false;
	}

	if (m_InGameUI) {
		m_InGameUI->SetReplayVisible(true);
	}
}

void Game::StartReplayRecording()
{
	SetGameStarted(true);
	m_isRecording = true;
	m_replayFrameCounter = 0;
}

void Game::DecideBestReplay()
{
	m_bestShotIndex = -1;
	float best = -1.0f;

	for (int i = 0; i < 3; i++) {
		if (m_scores[i] > best && m_replayPaths[i].size() > 0) {
			best = m_scores[i];
			m_bestShotIndex = i;
		}
	}

	if (m_bestShotIndex != -1) {
		m_bestSwingFrame = m_swingFrame[m_bestShotIndex];
		m_bestPitchFrame = m_pitchFrame[m_bestShotIndex];
	}
}

void Game::GoToResult()
{
	bool hasScore = false;
	for (int i = 0; i < 3; i++) {
		if (m_scores[i] > 0) {
			hasScore = true;
			break;
		}
	}

	if (hasScore && g_soundManager->m_seVolume > 0) {
		g_soundManager->PlaySE(enSound_SE2);
	}
	int best = max(m_scores[0], max(m_scores[1], m_scores[2]));
	Result* result = NewGO<Result>(0);
	result->SetResultValues(m_guruguru, best, m_scores);
	DeleteGO(this);
}

void Game::StartEndFade()
{
	if (!m_InGameUI) {
		GoToResult();
		return;
	}

	m_InGameUI->StartFadeOut(0.5f);

	m_InGameUI->m_onFadeOutFinished = [this]() {
		// 🌟【フェードパッチ追加】最優先でフェード中の着地復帰を確認
		if (m_ballLandedDuringFade) {
			m_ballLandedDuringFade = false;
			m_cameraMode = Camera_Catcher;
			if (m_gameCamera) m_gameCamera->UnfreezeCamera();
			if (m_InGameUI) m_InGameUI->StartFadeIn(0.5f);
			return;
		}

		if (m_bestShotIndex != -1) {
			StartReplay(m_bestShotIndex);

			if (m_InGameUI) {
				m_InGameUI->StartFadeIn(0.5f);
			}
		}
		else {
			GoToResult();
		}
		};
}

int Game::GetReplayFrameCount() const
{
	return m_replayFrameCounter;
}

void Game::OnPitcherThrow()
{
	if (m_isReplayPlaying) {
		return;
	}

	if (m_InGameUI && (m_InGameUI->IsFadingOut() || m_InGameUI->IsFadingIn())) {
		return;
	}

	int shot = m_shots;
	m_pitchFrame[shot] = m_replayFrameCounter;

	if (m_batter) {
		m_batter->SetCursorMode(true);
	}

	if (m_ball) {
		m_ball->Throw({ 0, -20, 0 });
	}
}

void Game::Render(RenderContext& rc)
{
}