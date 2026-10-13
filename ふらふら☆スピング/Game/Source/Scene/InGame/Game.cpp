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
#include "Source/DifficultyParams.h"
#include "Source/Scene/Titer/Titer.h"
#include "Source/Weather/Weather.h"
#include <cstdlib>
#include <ctime>
#include <cmath>

namespace
{
	constexpr float kFrameDt = 1.0f / 60.0f;

	constexpr float kAfterLandingWait = 1.5f;		// 着地してから次の球へ進むまで
	constexpr float kTutorialSelectWait = 3.0f;		// チュートリアルで評価表示を待つ時間
	constexpr float kReplayHitStopTime = 0.05f;		// リプレイのインパクト時のヒットストップ
	constexpr float kReplaySwingAnimSpeed = 2.0f;
	constexpr float kFadeTime = 0.5f;

	constexpr float kStickThreshold = 0.7f;			// チュートリアル選択のスティック入力
	constexpr float kStickNeutral = 0.3f;
}

Game::~Game()
{
	if (m_gameCamera) DeleteGO(m_gameCamera);
	if (m_background) DeleteGO(m_background);
	if (m_batter)     DeleteGO(m_batter);
	if (m_pitcher)    DeleteGO(m_pitcher);
	if (m_catcher)    DeleteGO(m_catcher);
	if (m_ball)       DeleteGO(m_ball);
	if (m_skyCube)    DeleteGO(m_skyCube);
	if (m_InGameUI)   DeleteGO(m_InGameUI);
	if (m_weather)    DeleteGO(m_weather);
	auto start1 = FindGO<Start1>("start1");
	if (start1) DeleteGO(start1);
}

bool Game::Start()
{
	srand(static_cast<unsigned int>(time(nullptr)));

	// Load で作ったオブジェクトを取得するだけ
	m_skyCube = FindGO<SkyCube>("skyCube");
	m_background = FindGO<Background>("backGround");
	m_gameCamera = FindGO<GameCamera>("gameCamera");
	m_InGameUI = FindGO<InGameUI>("inGameUI");
	m_batter = FindGO<Batter>("batter");
	m_pitcher = FindGO<Pitcher>("pitcher");
	m_ball = FindGO<Ball>("ball");
	m_catcher = FindGO<Catcher>("catcher");

	// カウントダウンUIを表示
	NewGO<Start1>(0, "start1");

	if (m_InGameUI) {
		m_InGameUI->SetGameInstance(this);
	}
	// ボールをカメラにセット
	if (m_gameCamera && m_ball) {
		m_gameCamera->SetBall(m_ball);
	}

	m_weather = NewGO<Weather>(0, "weather");
	if (m_weather) {
		m_weather->SetWeatherType(m_weatherType);
		m_weather->SetDifficulty(m_difficulty);
	}

	const DifficultyParams& p = GetDifficultyParams(m_difficulty);
	m_maxShots = p.pitchCount;

	if (m_gameCamera) {
		m_gameCamera->SetReplayZoomMaxSpeed(p.replayZoomMaxSpeed);
	}

	m_replayPaths.resize(MAX_SHOTS);

	return true;
}

int Game::GetGuruguruSEInterval() const
{
	switch (m_difficulty)
	{
	case Tutorial:
	case Easy:
		return 7;
	case Normal:
		return 5;
	default:	// Hard
		return 3;
	}
}


//==================================================================
// 更新
//==================================================================
void Game::Update()
{
	m_replayDuration = GetDifficultyParams(m_difficulty).replayDuration;

	// ぐるぐる値を毎フレーム Game に保存する
	if (m_batter) {
		SetGuruGuru(m_batter->GetGuruGuru());
	}
	UpdateGuruguruSE();

	if (m_InGameUI) {
		m_InGameUI->SetBallCount(m_maxShots - m_shots);
	}

	UpdateGuruGuruIntroCamera();

	// カウントダウン中はポーズボタン無効 & ゲームロジック停止
	if (FindGO<Start1>("start1") != nullptr) {
		UpdateCountdown();
		return;
	}

	if (g_pad[0]->IsTrigger(enButtonStart)) {
		if (FindGO<PauseUI>("pause") != nullptr) return;
		if (FindGO<SoundTestUI>("soundtest") != nullptr) return;
		OpenPause();
	}

	// ポーズ中はゲーム停止
	if (m_isPaused) return;

	if (m_isReadyPhase) {
		UpdateReadyPhase();
		return;
	}

	// チュートリアル最終球後の「続ける/タイトルへ戻る」選択待ち
	if (m_shouldContinueTutorial) {
		UpdateTutorialSelect();
		return;
	}

	UpdateHitStop();

	if (!UpdateKakutei()) return;

	UpdateTimeScale();

	if (!UpdateHitGlance()) return;

	UpdateCameraAndUI();

	if (!UpdateAfterLanding()) return;

	UpdateLiveDistance();

	if (!UpdateFadeInDelay()) return;

	if (m_isReplayPlaying) {
		UpdateReplay();
		return;
	}

	// 録画中は毎フレームカウンタを進める
	if (m_isRecording) {
		m_replayFrameCounter++;
	}
}

// ぐるぐるの回数が一定間隔になったらSEを鳴らす
void Game::UpdateGuruguruSE()
{
	const int g = m_guruguru;
	const int seInterval = GetGuruguruSEInterval();

	if (g > 0 && seInterval > 0 && g % seInterval == 0 && g != m_prevGuruGuru) {
		// サウンドテストで SE 音量が 0 のときは鳴らさない
		if (g_soundManager && g_soundManager->m_seVolume > 0) {
			g_soundManager->PlaySE(enSound_SE3, 1.0f);
		}
	}
	m_prevGuruGuru = g;
}

// ぐるぐる旋回の開始・終了を検知してカメラの演出を切り替える
void Game::UpdateGuruGuruIntroCamera()
{
	const bool isRotationSeen = (m_batter && m_batter->GetRotationSeen());

	if (isRotationSeen && !m_hasStartedGuruIntroCamera) {
		// 旋回が始まった瞬間に1回だけ呼ぶ
		if (m_gameCamera) {
			m_gameCamera->StartGuruGuruIntroCamera();
		}
		m_hasStartedGuruIntroCamera = true;
	}
	else if (!isRotationSeen && m_hasStartedGuruIntroCamera) {
		// 旋回が終わった瞬間に元のカメラへ戻す
		if (m_gameCamera) {
			m_gameCamera->SetCatcherCamera();
		}
		m_hasStartedGuruIntroCamera = false;
	}
}

void Game::UpdateCountdown()
{
	if (m_InGameUI) {
		m_InGameUI->SetUIVisible(false);
		m_InGameUI->SetFontVisble(false);
		m_InGameUI->SetReplayVisible(false);
	}

	if (m_batter)  m_batter->AnimationUpdate();
	if (m_pitcher) m_pitcher->AnimationUpdate();
	if (m_catcher) m_catcher->AnimationUpdate();
}

void Game::OpenPause()
{
	SetIsPaused(true);

	if (m_InGameUI) {
		m_InGameUI->SetPause(true);
	}
	PauseUI* pause = NewGO<PauseUI>(0, "pause");
	if (pause) {
		pause->SetDifficulty(m_difficulty);
	}
}

void Game::SetUIVisibility(bool ui, bool font, bool replay, bool baisoku)
{
	if (!m_InGameUI) return;
	m_InGameUI->SetUIVisible(ui);
	m_InGameUI->SetFontVisble(font);
	m_InGameUI->SetReplayVisible(replay);
	m_InGameUI->SetBaisokuVisible(baisoku);
}

// 準備フェーズ（最初の5秒間。Bでスキップ可能）
void Game::UpdateReadyPhase()
{
	if (g_pad[0]->IsTrigger(enButtonB)) {
		m_isReadyPhase = false;
		m_readyTimer = 0.0f;

		if (m_InGameUI) {
			m_InGameUI->SetGuruGuruTimer(0.0f);
		}
		SetGameStarted(true);

		if (m_pitcher) {
			m_pitcher->ResetThrow();	// Idle ＆ タイマー0に戻す
		}
		return;
	}

	m_readyTimer -= g_gameTime->GetFrameDeltaTime();

	if (m_InGameUI) {
		m_InGameUI->SetGuruGuruTimer(m_readyTimer);
	}

	// 時間が来たら本格的に1球目を開始する
	if (m_readyTimer <= 0.0f) {
		m_isReadyPhase = false;
		SetGameStarted(true);	// ここで初めてピッチャーの投球タイマーを動かす許可を出す

		if (m_batter) {
			m_batter->SetCursorMode(true);	// カーソル操作モードを確定
		}
	}
}

// チュートリアル最終球後の「続ける/タイトルへ戻る」選択待ち
void Game::UpdateTutorialSelect()
{
	const float stickX = g_pad[0]->GetLStickXF();

	// スティックでカーソル移動（ニュートラルに戻ってから次を受け付ける）
	if (m_tutorialStickNeutral) {
		if (stickX < -kStickThreshold) {
			m_isTutorialSelectTitle = false;	// 右 → 続ける
			m_tutorialStickNeutral = false;
		}
		else if (stickX > kStickThreshold) {
			m_isTutorialSelectTitle = true;		// 左 → タイトルへ戻る
			m_tutorialStickNeutral = false;
		}
	}
	else if (fabsf(stickX) < kStickNeutral) {
		m_tutorialStickNeutral = true;
	}

	// Aボタンで確定
	if (!g_pad[0]->IsTrigger(enButtonA)) return;

	if (m_isTutorialSelectTitle) {
		GoToTiter();
	}
	else {
		RestartTutorial();
	}
}

// チュートリアルを最初の球からやり直す
void Game::RestartTutorial()
{
	m_shouldContinueTutorial = false;
	m_isInputLocked = false;
	m_shots = 0;

	// ※ 3球分だけ初期化している（元の挙動のまま。チュートリアルは最大5球）
	for (int i = 0; i < 3; i++) {
		m_hasSwung[i] = false;
		m_swingFrame[i] = -1;
		m_pitchFrame[i] = 0;
		m_replayPaths[i].clear();
		if (m_InGameUI) {
			m_InGameUI->m_threeShots[i] = 0;
			m_InGameUI->m_shotDone[i] = false;
		}
	}

	if (m_InGameUI) {
		m_InGameUI->ResetMissFlags();
	}

	ResetForNextShot();

	m_cameraMode = Camera_Catcher;
	m_hasStartedGuruIntroCamera = false;
	if (m_gameCamera) {
		m_gameCamera->UnfreezeCamera();		// パーフェクト確定等で凍結されていた場合の解除
		m_gameCamera->SetCatcherCamera();
	}

	if (m_InGameUI) {
		SetUIVisibility(true, true, false, false);
		m_InGameUI->ResetFade();
	}

	if (m_pitcher) {
		m_pitcher->ResetThrow();
	}
	if (m_ball) {
		m_ball->ResetThrowTimer();
	}
	SetGameStarted(true);
}

// ヒットストップ（ゲーム全体を一瞬停止）
void Game::UpdateHitStop()
{
	if (m_hitStopTimer > 0.0f) {
		m_hitStopTimer -= g_gameTime->GetFrameDeltaTime();
		if (m_hitStopTimer <= 0.0f) {
			m_isHitStop = false;
			m_hitStopTimer = 0.0f;
		}
	}
	else {
		m_isHitStop = false;
	}
}

// パーフェクト確定演出中。通常プレイ時は false（ゲームロジックを止める）
bool Game::UpdateKakutei()
{
	if (!m_isKakutei) return true;

	if (m_InGameUI) {
		SetUIVisibility(false, false, false, false);
		if (!m_InGameUI->m_isPerfectAnimActive) {
			m_InGameUI->m_isPerfectAnimActive = true;
			m_InGameUI->m_perfectAnimTimer = 0.0f;	// タイマーを最初からリセット
		}
	}

	m_kakuteiTimer -= g_gameTime->GetFrameDeltaTime();

	if (m_kakuteiTimer <= 0.0f) {
		m_isKakutei = false;

		// 演出終了時にUI側のパーフェクトフラグも落とす
		if (m_InGameUI) {
			m_InGameUI->m_isPerfectAnimActive = false;
		}
		if (m_gameCamera) {
			m_gameCamera->UnfreezeCamera();
		}

		// 通常の打球追尾カメラへ戻す
		m_cameraMode = Camera_BackBall;

		// 集中線を消す
		if (m_InGameUI) {
			m_InGameUI->m_shuchusenTimer2 = 0.0f;
		}
	}

	return m_isReplayPlaying;
}

// 倍速の切り替え（100m演出前だけ許可）
void Game::UpdateTimeScale()
{
	if (m_InGameUI && m_InGameUI->IsFadingOut()) {
		// フェードアウト開始の瞬間だけSEをフェードアウトさせる
		if (!m_startFadeSE2) {
			m_startFadeSE2 = true;
			if (g_soundManager) {
				g_soundManager->FadeOutSE2(2.5f);
			}
		}
		m_timeScale = 1.0f;
	}
	else if (!m_hasTriggered100m) {
		const DifficultyParams& p = GetDifficultyParams(m_difficulty);

		if (m_canFastForward && g_pad[0]->IsPress(enButtonB)) {
			m_timeScale = p.fastForwardScaleWithB;
		}
		else if (m_canFastForward) {
			m_timeScale = p.fastForwardScale;
		}
		else {
			m_timeScale = 1.0f;
		}
	}
}

// 打撃の瞬間の注視。注視中は false（他のカメラ切り替えを止める）
bool Game::UpdateHitGlance()
{
	if (!m_isHitGlancing) return true;

	m_hitGlanceTimer -= g_gameTime->GetFrameDeltaTime();
	if (m_hitGlanceTimer <= 0.0f) {
		m_isHitGlancing = false;

		if (m_isKakutei) {
			// パーフェクト時は確定カメラへ
			if (m_gameCamera) {
				m_gameCamera->SetkakuteiCamera();
				m_gameCamera->FreezeCamera();
			}
		}
		else {
			// 通常ヒット時はここで初めて打球追尾カメラへ切り替える
			m_cameraMode = Camera_BackBall;
		}
	}
	return false;
}

// カメラモードに応じたカメラとUIの切り替え
void Game::UpdateCameraAndUI()
{
	switch (m_cameraMode) {
	case Camera_Catcher:
		if (m_gameCamera && !(m_batter && m_batter->GetRotationSeen())) {
			m_gameCamera->SetCatcherCamera();
		}
		m_cameraType = Camera_Catcher;
		SetUIVisibility(true, true, false, false);
		break;

	case Camera_Replay:
		if (m_gameCamera && !m_hasStartedReplayZoom) {
			m_gameCamera->SetReplayCamera();
		}
		m_cameraType = Camera_Replay;
		SetUIVisibility(false, false, true, false);
		break;

	case Camera_Ball:
		if (m_gameCamera) m_gameCamera->SetFollowBallCamera();
		m_cameraType = Camera_Ball;
		SetUIVisibility(false, true, false, true);
		break;

	case Camera_BackBall:
		if (m_gameCamera) m_gameCamera->SetFollowBallBackCamera();
		m_cameraType = Camera_Ball;
		SetUIVisibility(false, true, false, true);
		break;

	case Camera_Kakutei:
		SetUIVisibility(false, false, false, false);
		break;
	}
}

// 着地後の待ち。Update をここで終えるなら false
bool Game::UpdateAfterLanding()
{
	// リプレイ中は着地後の待ち処理をスキップ
	if (!m_isBallLanded || m_isReplayPlaying) return true;

	m_afterLandingTimer += kFrameDt * m_timeScale;
	if (m_afterLandingTimer < kAfterLandingWait) return true;

	// 最終球の着地後
	if (m_shots == m_maxShots - 1) {
		if (m_difficulty == Difficulty::Tutorial) {
			// 評価スプライトの表示が終わるまで選択画面への遷移を待つ
			if (m_afterLandingTimer < kTutorialSelectWait) return false;
			m_isInputLocked = true;
			m_shouldContinueTutorial = true;
		}
		else {
			m_isInputLocked = true;
			DecideBestReplay();
			if (m_bestShotIndex != -1) {
				m_shouldStartReplay = true;
				StartEndFade();
			}
			else {
				GoToResult();
			}
			return false;
		}
	}

	// 途中の球の着地後
	if (!m_hasTriggered100m) {
		m_cameraMode = Camera_Catcher;

		// UI も通常状態に戻す
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

	m_shots++;
	ResetForNextShot();
	return false;
}

// ボールが飛んでいる間は距離をリアルタイム更新
void Game::UpdateLiveDistance()
{
	if (!m_ball || !m_ball->GetIsMove() || !m_hasStartedDistance || m_isBallLanded) return;

	m_km = fmaxf(0.0f, m_hitStartZ - m_ball->GetPosition().z);

	if (m_InGameUI) {
		m_InGameUI->m_threeShots[m_shots] = m_km;
		m_InGameUI->m_shotDone[m_shots] = true;
		m_InGameUI->SetKm(m_km);
	}
}

// フェードイン遅延処理。Update をここで終えるなら false
bool Game::UpdateFadeInDelay()
{
	if (m_fadeInDelayTimer < 0.0f) return true;

	if (m_shots == m_maxShots - 1) {
		m_fadeInDelayTimer = -1.0f;
		return false;
	}

	m_fadeInDelayTimer -= g_gameTime->GetFrameDeltaTime();

	if (m_fadeInDelayTimer <= 0.0f && m_InGameUI) {
		m_InGameUI->StartFadeIn(kFadeTime);
		m_fadeInDelayTimer = -1.0f;
	}
	return true;
}


//==================================================================
// リプレイ
//==================================================================
void Game::UpdateReplay()
{
	const float dt = g_gameTime->GetFrameDeltaTime();

	// 振りかぶりなどの遅延処理
	if (m_replayDelayTimer > 0.0f) {
		m_replayDelayTimer -= dt;
		return;
	}

	// インパクト時のヒットストップ中はリプレイのコマを進めない
	if (m_hitStopTimer > 0.0f) {
		return;
	}

	const int index = m_replayStartFrame;
	const int pathSize = static_cast<int>(m_currentReplay.size());

	// 位置を流し込み、ボール自身の Update で魔球の演出や軌跡も動かす
	if (index < pathSize) {
		m_ball->SetPosition(m_currentReplay[index]);
	}
	m_ball->Update();

	// スイング（インパクト）の同期タイミング。記録があれば必ずそこでアニメを再生する
	const int swingTiming = m_bestSwingFrame + m_replaySwingDelayFrames;
	if (m_bestShotIndex >= 0 && !m_hasPlayedReplaySwing && index >= swingTiming) {
		if (m_batter) {
			m_batter->PlaySwingAnimation();
			m_batter->GetCharacterModel()->GetModelRender()->SetAnimationSpeed(kReplaySwingAnimSpeed);
		}

		m_hitStopTimer = kReplayHitStopTime;
		m_isHitStop = true;
		m_ball->SetHasHit(true);
		m_hasPlayedReplaySwing = true;	// 二重発火防止
	}

	// 投球フレームに到達したら、指定秒数後にズーム開始
	if (!m_hasReachedPitchFrame && index >= m_replayPitchFrame) {
		m_hasReachedPitchFrame = true;
	}
	if (m_hasReachedPitchFrame && !m_hasStartedReplayZoom) {
		m_replayZoomDelayTimer += dt;

		if (m_replayZoomDelayTimer >= m_replayZoomDelaySeconds) {
			if (m_gameCamera) {
				m_gameCamera->StartReplayZoomToBall();
			}
			m_hasStartedReplayZoom = true;
		}
	}

	// インデックスを毎フレーム1ずつ確実に進める
	m_replayStartFrame++;
	m_replayTimer += dt;

	// 時間切れ・軌道の終わり・Bボタンでスキップ
	if (m_replayTimer >= m_replayDuration
		|| m_replayStartFrame >= static_cast<int>(m_currentReplay.size())
		|| g_pad[0]->IsTrigger(enButtonB))
	{
		EndReplay();
	}
}

// リプレイを終えてリザルトへ移る
void Game::EndReplay()
{
	m_isReplayPlaying = false;
	if (m_ball) {
		m_ball->StopTrajectoryEffect();	// 軌跡エフェクトが残らないようにする
	}
	m_cameraMode = Camera_Catcher;
	GoToResult();
}

void Game::StartReplay(int index)
{
	m_isRecording = false;
	if (m_ball) {
		m_ball->SetIsRecording(false);
	}

	m_hasPlayedReplaySwing = false;
	m_replayStartFrame = 0;
	m_replayFrameCounter = 0;
	m_isReplayPlaying = true;
	m_replayTimer = 0.0f;
	m_hasStartedReplayZoom = false;
	m_hasReachedPitchFrame = false;
	m_replayZoomDelayTimer = 0.0f;
	m_replayDelayTimer = 2.0f;	// 2秒待機
	m_cameraMode = Camera_Replay;
	m_currentReplay = m_replayPaths[index];
	m_replayPitchFrame = m_pitchFrame[index];

	// ピッチャーの投球アニメを再生し、バッターも初期姿勢に戻す
	if (m_pitcher) {
		m_pitcher->PlayPitchAnimation();
	}
	if (m_batter) {
		m_batter->ResetSwing();
		m_batter->AnimationUpdate();
	}

	// ボールを初期位置に戻して完全同期
	if (m_ball) {
		m_ball->ResetBall();
		m_ball->SetVelocity(Vector3::Zero);
		m_ball->SetIsMove(false);
		m_ball->SetHasHit(false);
		m_ball->SetIsMagicBall(m_isMagicBallShot[index]);
	}

	if (m_InGameUI) {
		m_InGameUI->SetReplayVisible(true);
	}
}

void Game::StartReplayRecording()
{
	SetGameStarted(true);
	m_isRecording = true;
	m_replayFrameCounter = 0;	// 毎回 0 からスタート
}

void Game::DecideBestReplay()
{
	m_bestShotIndex = -1;
	int best = 0;

	for (int i = 0; i < m_maxShots; i++) {
		// スコアが0（空振り）は対象外
		if (m_scores[i] > best && !m_replayPaths[i].empty()) {
			best = m_scores[i];
			m_bestShotIndex = i;
		}
	}

	if (m_bestShotIndex != -1) {
		m_bestSwingFrame = m_swingFrame[m_bestShotIndex];
	}
}


//==================================================================
// 進行・遷移
//==================================================================
void Game::ResetForNextShot()
{
	m_startFadeSE2 = false;
	m_isBallLanded = false;
	m_afterLandingTimer = 0.0f;
	m_km = 0.0f;
	m_hasTriggered100m = false;
	m_hasStartedDistance = false;
	m_isHomeRun = false;

	// ボールを初期位置に戻す
	if (m_ball) {
		m_ball->ResetBall();
	}

	// バッター内部のスティック移動累積を完全にゼロリセット。カーソル操作可能に戻す
	if (m_batter) {
		m_batter->ResetSwing();
		m_batter->SetCursorMode(true);
	}

	if (m_InGameUI) {
		m_InGameUI->SetKm(0);
		m_InGameUI->SetBaisokuVisible(false);
		m_InGameUI->ResetBatAndMeetOnly();
	}

	m_isInputLocked = false;

	if (m_shots < m_maxShots) {
		m_hasSwung[m_shots] = false;
	}
}

void Game::OnBallLanded()
{
	m_isBallLanded = true;
	m_afterLandingTimer = 0.0f;
	m_canFastForward = false;
	m_timeScale = 1.0f;
	m_hasStartedDistance = false;
	m_isTutorialSelectTitle = false;	// 初期カーソルは「続ける」
	m_tutorialStickNeutral = true;

	if (g_soundManager && !m_startFadeSE2) {
		m_startFadeSE2 = true;
		g_soundManager->FadeOutSE2(3.0f);
	}

	if (m_pitcher) {
		m_pitcher->ResetThrow();
	}
	// Ball内部の投球タイマーをリセットして、着地直後の即リセット／再投球を防止
	if (m_ball) {
		m_ball->ResetThrowTimer();
	}

	// スコアを保存
	m_scores[m_shots] = static_cast<int>(m_km);
	if (m_InGameUI) {
		m_InGameUI->m_threeShots[m_shots] = m_km;
		m_InGameUI->m_shotDone[m_shots] = true;
	}

	const bool isLastShot = (m_shots == m_maxShots - 1);

	if (m_difficulty == Difficulty::Tutorial) {
		if (isLastShot) {
			m_isInputLocked = true;			// ピッチャー側で見る共通フラグ
			m_shouldContinueTutorial = true;
		}
		return;
	}

	// ボール軌道を保存（ヒットした時だけ）
	std::vector<Vector3> path = m_ball->GetReplayPath();
	m_replayPaths[m_shots] = std::move(path);

	if (!isLastShot) return;

	// 最終球が終わった
	DecideBestReplay();
	m_isInputLocked = true;
	if (m_bestShotIndex != -1) {
		m_shouldStartReplay = true;		// ベストショットのリプレイを予約
	}
	else {
		GoToResult();					// 全部空振り → そのままリザルト
	}

	// 真っ黒になったらリプレイ or リザルト
	StartEndFade();
}

void Game::OnOver100m()
{
	if (m_gameCamera) {
		m_gameCamera->FreezeCamera();
	}

	if (m_InGameUI) {
		m_InGameUI->StartFadeOut(kFadeTime);

		m_InGameUI->m_onFadeOutFinished = [this]() {
			// 完全に黒くなった瞬間にカメラを切り替え、凍結も解除する
			m_cameraMode = Camera_Catcher;
			if (m_gameCamera) {
				m_gameCamera->UnfreezeCamera();
			}

			// ここでリプレイ開始
			if (m_shouldStartReplay) {
				StartReplay(m_bestShotIndex);
				m_shouldStartReplay = false;

				if (m_InGameUI) {
					m_InGameUI->StartFadeIn(kFadeTime);
				}
				return;
			}

			// フェードアウト完了 → 超高速で進める
			m_timeScale = 300.0f;

			if (m_shots == m_maxShots - 1) {
				m_fadeInDelayTimer = -1.0f;
				return;
			}

			// フェードインは 1 秒後に実行
			m_fadeInDelayTimer = 1.0f;
			};
	}
	m_canFastForward = false;
	m_hasTriggered100m = true;
}

void Game::GoToResult()
{
	// 記録が1つでもあれば SE2 を鳴らす
	bool hasScore = false;
	for (int i = 0; i < m_maxShots; i++) {
		if (m_scores[i] > 0) {
			hasScore = true;
			break;
		}
	}
	if (hasScore && g_soundManager->m_seVolume > 0) {
		g_soundManager->PlaySE(enSound_SE2);
	}

	auto start1 = FindGO<Start1>("start1");
	if (start1) DeleteGO(start1);

	auto pause = FindGO<PauseUI>("pause");
	if (pause) DeleteGO(pause);

	const int best = max(m_scores[0], max(m_scores[1], m_scores[2]));
	Result* result = NewGO<Result>(0);
	if (result) {
		result->SetDifficulty(GetDifficulty());
		result->SetResultValues(m_guruguru, best, m_scores);
	}
	DeleteGO(this);
}

void Game::GoToTiter()
{
	auto start1 = FindGO<Start1>("start1");
	if (start1) DeleteGO(start1);

	auto pause = FindGO<PauseUI>("pause");
	if (pause) DeleteGO(pause);

	if (g_soundManager) {
		g_soundManager->StopSE2();	// 歓声(SE2)を止めてからタイトルへ
	}
	NewGO<Titer>(0);

	DeleteGO(this);
}

void Game::StartEndFade()
{
	if (!m_InGameUI) {
		GoToResult();
		return;
	}

	m_InGameUI->StartFadeOut(kFadeTime);

	if (g_soundManager && !m_startFadeSE2) {
		m_startFadeSE2 = true;
		g_soundManager->FadeOutSE2(2.0f);
	}

	m_InGameUI->m_onFadeOutFinished = [this]() {
		// 画面は真っ黒の状態
		if (m_bestShotIndex != -1) {
			// ベストショットがある → リプレイを見せるためにフェードイン
			StartReplay(m_bestShotIndex);
			if (m_InGameUI) {
				m_InGameUI->StartFadeIn(kFadeTime);
			}
		}
		else {
			// 全部空振り → そのままリザルトへ
			GoToResult();
		}
		};
}

void Game::OnPitcherThrow()
{
	// ピッチャーが投げた瞬間のフレームを保存
	SetPitchFrame(m_shots, m_replayFrameCounter);

	if (m_batter) {
		m_batter->SetCursorMode(true);	// 2球目以降もスティックで動くようにする
	}

	if (m_ball) {
		m_ball->Throw({ 0, -20, 0 });
	}
}

void Game::StartHitGlance(float duration)
{
	m_hitGlanceTimer = duration;
	m_isHitGlancing = true;
	if (m_gameCamera) {
		m_gameCamera->SetImpactGlanceCamera();
	}
}

void Game::Render(RenderContext& rc)
{
}