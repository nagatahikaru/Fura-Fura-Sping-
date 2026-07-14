#include "stdafx.h"
#include "Game.h"
#include"Source/Actor/GameCamera/GameCamera.h"
#include"Source/Actor/Stage/Background.h"
#include"Source/UI/InGameUI/InGameUI.h"
#include"Source/Actor/Character/Batter/Batter.h"
#include"Source/Actor/Character/Pitcher/Pitcher.h"
#include"Source/Actor/Character/Catcher/Catcher.h"
#include"Source/Actor/Character/Ball/Ball.h"
#include"Source/Scene/Result/Result.h"
#include"Source/Sound/SoundManager.h"
#include"Source/UI/PauseUI/PauseUI.h"
#include"Source/UI/SoundTestUI/SoundTestUI.h"
#include"Source/Scene/Start/Start.h"


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

	// ★ Game内でNewGOしたカウントダウンUIもここで確実に道連れにする
	auto start1 = FindGO<Start1>("start1");
	if (start1) DeleteGO(start1);
}


bool Game::Start()
{

	// ★ Load で作ったオブジェクトを取得するだけ
	m_skyCube = FindGO<SkyCube>("skyCube");
	m_background = FindGO<Background>("backGround");
	m_gameCamera = FindGO<GameCamera>("gameCamera");
	m_InGameUI = FindGO<InGameUI>("inGameUI");
	m_batter = FindGO<Batter>("batter");
	m_pitcher = FindGO<Pitcher>("pitcher");
	m_ball = FindGO<Ball>("ball");
	m_catcher = FindGO<Catcher>("catcher");
	// ★ カウントダウンUIを表示
	m_start1 = NewGO<Start1>(0, "start1");

	if (m_InGameUI) {
		m_InGameUI->SetGameInstance(this); 
	}
	// ボールをカメラにセット
	if (m_gameCamera && m_ball) {
		m_gameCamera->SetBall(m_ball);
	}

	m_replayPaths.resize(3);

	return true;
}

int Game::GetGuruguruSEInterval() const
{
	switch (m_difficulty)
	{
	case Easy:
		return 7;
	case Normal:
		return 5;
	case Hard:
		return 3;
	default:
		return 3;
	}
}

void Game::Update()
{
	//当たり判定の表示
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	// ★ ぐるぐる値を毎フレーム Game に保存する
	if (m_batter) {
		SetGuruGuru(m_batter->GetGuruGuru());
	}
	// ★★★ ぐるぐる 5 回刻みで SE3 を鳴らす ★★★
	int g = GetGuruguru();
	int seInterval = GetGuruguruSEInterval();

	if (g > 0 && seInterval > 0 && g % seInterval == 0 && g != m_prevGuruGuru) {

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
		PauseUI* pause = NewGO<PauseUI>(0, "pause");
		if (pause) {
			pause->SetDifficulty(m_difficulty); 
		}
	}

	// ★ ポーズ中はゲーム停止
	if (m_isPaused) {
		return;
	}

	if (m_isReadyPhase) {
		if (g_pad[0]->IsTrigger(enButtonB)) {
			m_isReadyPhase = false;
			m_readyTimer = 0.0f;

			if (m_InGameUI) {
				m_InGameUI->SetGuruGuruTimer(0.0f);
			}
			SetGameStarted(true);

			Pitcher* pitcher = FindGO<Pitcher>("pitcher");
			if (pitcher) {
				pitcher->ResetThrow(); // 既存の初期化関数を呼んで Idle ＆ タイマー0に戻す
			}

			return; // スキップしたフレームはここで処理を抜ける
		}

		m_readyTimer -= g_gameTime->GetFrameDeltaTime();

		if (m_InGameUI) {
			m_InGameUI->SetGuruGuruTimer(m_readyTimer);
		}

		// 5秒経過したら、本格的に1球目を開始する
		if (m_readyTimer <= 0.0f) {
			m_isReadyPhase = false; // フェードアウト
			SetGameStarted(true);   // ここで初めてピッチャーの投球タイマーを動かす許可を出す

			if (m_batter) {
				m_batter->SetCursorMode(true); // カーソル操作モードを確定
			}
		}
		return; // ⭕ 5秒間はここでUpdateを抜けることで、後続の「投球開始処理」へ進ませない
	}

	// ★ ヒットストップ処理（ゲーム全体を一瞬停止）
	if (m_hitStopTimer > 0.0f) {
		m_hitStopTimer -= g_gameTime->GetFrameDeltaTime();
		if (m_hitStopTimer <= 0.0f) {
			m_isHitStop = false;
			m_hitStopTimer = 0.0f;
		}
		// ★ ここにあった return; を消去！
		// return をしないことで、この後にある m_ball->SetPosition(path[index]) まで処理が到達します。
	}
	else {
		m_isHitStop = false;  // ← 終わったら解除
	}

	// ★★★ パーフェクト確定演出中（2秒間） ★★★
	if (m_isKakutei) {

		if (m_InGameUI) {
			m_InGameUI->SetUIVisible(false);
			m_InGameUI->SetFontVisble(false);
			m_InGameUI->SetReplayVisible(false);
			m_InGameUI->SetBaisokuVisible(false);
			if (!m_InGameUI->m_isPerfectAnimActive) {
				m_InGameUI->m_isPerfectAnimActive = true;
				m_InGameUI->m_perfectAnimTimer = 0.0f; // タイマーを最初からリセット
			}
		}
		m_kakuteiTimer -= g_gameTime->GetFrameDeltaTime();

		if (m_kakuteiTimer <= 0.0f) {

			m_isKakutei = false;
			// 🌟【ここを追加】演出終了時にUI側のパーフェクトフラグも落とす
			if (m_InGameUI) {
				m_InGameUI->m_isPerfectAnimActive = false;
			}
			// カメラ凍結解除
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

		if (!m_isReplayPlaying) {
			return;  // 通常プレイ時は従来どおりゲームロジックを完全停止する
		}
	}

	if (m_InGameUI && m_InGameUI->IsFadingOut()) {
		// ★ フェードアウト開始した瞬間だけ実行
		if (!m_startFadeSE2) {
			m_startFadeSE2 = true;

			if (g_soundManager) {
				g_soundManager->FadeOutSE2(2.5f);   // ← 0.7秒フェードアウト
			}
		}
		m_timeScale = 1.0f;
	}
	else if (!m_hasTriggered100m) {
		// ★ 100m演出前だけ倍速を許可
		if (m_canFastForward && g_pad[0]->IsPress(enButtonB)) {
			m_timeScale = 15.0f;
		}
		else if (m_canFastForward) {
			m_timeScale = 7.5f;
		}
		else {
			m_timeScale = 1.0f;
		}
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
		m_InGameUI->SetBaisokuVisible(false);
	}
	else if (m_cameraMode == Camera_Ball || m_cameraMode == Camera_BackBall)
	{
		m_cameraType = Camera_Ball;
		m_InGameUI->SetUIVisible(false);
		m_InGameUI->SetFontVisble(true);
		m_InGameUI->SetReplayVisible(false);
		m_InGameUI->SetBaisokuVisible(true);
	}
	else if (m_cameraMode == Camera_Replay)
	{
		m_cameraType = Camera_Replay;
		m_InGameUI->SetUIVisible(false);
		m_InGameUI->SetFontVisble(false);
		m_InGameUI->SetReplayVisible(true);
		m_InGameUI->SetBaisokuVisible(false);

	}
	else if (m_cameraMode == Camera_Kakutei) {
		m_InGameUI->SetUIVisible(false);
		m_InGameUI->SetFontVisble(false);
		m_InGameUI->SetReplayVisible(false);
		m_InGameUI->SetBaisokuVisible(false);
	}
	else if (m_batter->GetRotationSeen())
	{
		m_InGameUI->SetUIVisible(false);
		m_InGameUI->SetFontVisble(false);
		m_InGameUI->SetReplayVisible(true);
		m_InGameUI->SetGuruGuruCount(GetGuruguru());
	}

	// ★ リプレイ中は着地後の待ち処理をスキップ
	if (m_isBallLanded && !m_isReplayPlaying) {
		m_afterLandingTimer += (1.0f / 60.0f) * m_timeScale;

		if (m_afterLandingTimer >= 1.5f) {

			// ★ 3球目の着地から1秒経ったら、ここで初めて入力をロックしてフェードアウトを開始する
			if (m_shots == 2) {
				m_isInputLocked = true; // ★リプレイ直前のここでロック！
				DecideBestReplay();
				if (m_bestShotIndex != -1) {
					m_shouldStartReplay = true;
				}
				else {
					GoToResult();
					return;
				}
				StartEndFade(); // フェードアウトしてリプレイへ
				return;
			}

			// ★ 1球目・2球目の着地後1秒経ったときの処理
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
			return;
		}
	}

	// ★ ボールが飛んでいる間は距離をリアルタイム更新
	// ★ ボールを打った後だけ距離をリアルタイム更新
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
		m_replaySwingTimer += g_gameTime->GetFrameDeltaTime();

		// 振りかぶりなどの遅延処理
		if (m_replayDelayTimer > 0.0f) {
			m_replayDelayTimer -= g_gameTime->GetFrameDeltaTime();
			//float swingSec = 0.0f;
			//if (m_bestShotIndex >= 0) {
			//	swingSec = (m_pitchFrame[m_bestShotIndex] + m_swingFrame[m_bestShotIndex]) / 60.0f;
			//}

			//// 記録フレーム情報が存在すれば、その時間で再生する（ランタイムフラグには依存しない）
			//if (m_bestShotIndex >= 0 && !m_hasPlayedReplaySwing && m_replaySwingTimer >= swingSec) {
			//	m_batter->PlaySwingAnimation();
			//	m_hasPlayedReplaySwing = true;
			//}
			return;
		}

		// ★ インパクト時のヒットストップ中ならリプレイのコマ（フレーム）を進めない
		if (m_hitStopTimer > 0.0f) {
			return;
		}

		auto& path = m_currentReplay;
		int index = m_replayStartFrame;

		// 安全に位置を流し込む
		if (index < path.size()) {
			m_ball->SetPosition(path[index]);
		}

		// スイング（インパクト）の同期タイミング
		int swingTiming = m_bestSwingFrame + m_replaySwingDelayFrames;

		// 記録されたスイングフレーム情報がある場合は、必ずそのタイミングでアニメを再生する
		if (m_bestShotIndex >= 0 && !m_hasPlayedReplaySwing && index >= swingTiming) {
			m_batter->PlaySwingAnimation();
			m_batter->GetCharacterModel()->GetModelRender()->SetAnimationSpeed(2.0f);

			m_hitStopTimer = 0.05f;
			m_isHitStop = true;
			m_ball->m_hasHit = true;
			m_hasPlayedReplaySwing = true; // ★ 二重発火防止
		}

		// ★ リプレイのインデックスを毎フレーム1ずつ確実に進める
		m_replayStartFrame ++;
		m_replayTimer += g_gameTime->GetFrameDeltaTime();

		if (m_replayTimer >= m_replayDuration || m_replayStartFrame >= path.size()) {
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

	// ★ 録画中は毎フレームカウンタを進める
	if (m_isRecording) {
		m_replayFrameCounter++;
	}
}

void Game::ResetForNextShot()
{
	m_startFadeSE2 = false;
	m_isBallLanded = false;
	m_afterLandingTimer = 0.0f;
	m_zeroDistanceTimer = 0.0f;
	m_km = 0.0f;
	m_hasTriggered100m = false;
	m_hasStartedDistance = false;
	m_isHomeRun = false;

	// 1. ボールを初期位置に戻す
	if (m_ball) {
		m_ball->ResetBall();
	}

	// 2. バッター内部のスティック移動累積（m_meetPosition）を完全にゼロリセット
	if (m_batter) {
		m_batter->ResetSwing();
	}

	// 3. UI のリセット
	if (m_InGameUI) {
		m_InGameUI->SetKm(0);
		m_InGameUI->SetBaisokuVisible(false);
		m_InGameUI->ResetBatAndMeetOnly();
	}

	// バッターはカーソル操作可能に戻す
	if (m_batter) {
		m_batter->SetCursorMode(true);
	}

	m_isInputLocked = false;

	// 次のショット用のフラグ初期化（既存）
	if (m_shots < 3) {
		m_hasSwung[m_shots] = false;
	}
}
void Game::OnBallLanded()
{
	m_isBallLanded = true;
	m_afterLandingTimer = 0.0f;
	m_canFastForward = false;
	m_timeScale = 1.0f;
	m_hasStartedDistance = false;   // ★ ここでもリセット
	Pitcher* pitcher = FindGO<Pitcher>("pitcher");
	if (pitcher) {
		pitcher->ResetThrow();
	}
	// 追加: Ball内部の投球タイマーをリセットして、着地直後の即リセット／再投球を防止
	if (m_ball) {
		m_ball->ResetThrowTimer();
	}
	// スコア保存
	m_scores[m_shots] = m_km;

	// ★ UI にも保存（追加）
	if (m_InGameUI) {
		m_InGameUI->m_threeShots[m_shots] = m_km;
		m_InGameUI->m_shotDone[m_shots] = true;
	}

	// ボール軌道保存（ヒットした時だけ）
	if (m_ball->m_replayPath.size() > 0) {
		m_replayPaths[m_shots] = m_ball->m_replayPath;
	}
	else {
		m_replayPaths[m_shots].clear();
	}

	// 3球目が終わった？
	if (m_shots == 2) {
		DecideBestReplay();
		m_isInputLocked = true;
		if (m_bestShotIndex != -1) {
			// ベストショットがある → リプレイ開始を予約
			m_shouldStartReplay = true;
		}
		else {
			// 全部空振り → そのままリザルト
			GoToResult();
		}

		// ここでエンディング用フェード開始（真っ黒になったらリプレイ or リザルト）
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

			// ★ 完全に黒くなった瞬間にカメラ切り替え
			m_cameraMode = Camera_Catcher;

			// ★ カメラの凍結解除もここで行う（重要）
			if (m_gameCamera) {
				m_gameCamera->UnfreezeCamera();
			}

			// ★ ここでリプレイ開始
			if (m_shouldStartReplay) {
				StartReplay(m_bestShotIndex);
				m_shouldStartReplay = false;

				// ★ リプレイ開始したらフェードインする
				if (m_InGameUI) {
					m_InGameUI->StartFadeIn(0.5f);
				}

				return; // ← リプレイ開始したのでここで終了
			}

			// ★ フェードアウト完了 → ここで20倍速にする
			m_timeScale = 300.0f;

			if (m_shots == 2) {
				m_fadeInDelayTimer = -1.0f;
				return;
			}

			// ★ フェードインは 1.1 秒後に実行
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
	m_replaySwingTimer = 0.0f;  // ★ スイング用タイマー
	m_hasPlayedReplaySwing = false;
	m_replayStartFrame = 0;
	m_replayFrameCounter = 0;   // ★★★ これが絶対必要 ★★★
	m_isReplayPlaying = true;
	m_replayTimer = 0.0f;
	m_hasAppliedHitMoment = false;
	// ▼ 追加：タイマーとアキュムレータの初期化
	m_replayDelayTimer = 2.0f;  // 2秒待機
	m_replayAccumulator = 0.0f; // アキュムレータ初期化
	m_cameraMode = Camera_Replay;
	m_currentReplay = m_replayPaths[index];
	m_replayPitchFrame = m_pitchFrame[index];  // ← ★追加
	// ★★★ 追加：ピッチャーとボールをリセットしてタイミングを合わせる ★★★

	if (m_pitcher) {
		m_pitcher->PlayPitchAnimation();        // ← 投球アニメを再生
	}

	// バッターも初期姿勢に戻す（スイング前）
	if (m_batter) {
		m_batter->ResetSwing();
		m_batter->AnimationUpdate();
	}

	// ★★★ ボールを初期位置に戻して完全同期 ★★★
	if (m_ball) {
		m_ball->ResetBall();              // ← 初期位置へ
		m_ball->SetVelocity(Vector3::Zero); // ← 速度ゼロ
		m_ball->m_isMove = false;           // ← 動作停止
		m_ball->m_hasHit = false;           // ← ヒットフラグ解除
	}


	if (m_InGameUI) {
		m_InGameUI->SetReplayVisible(true);
	}
}

void Game::StartReplayRecording()
{
	SetGameStarted(true);
	m_isRecording = true;        // ★ 録画フラグ ON
	m_replayFrameCounter = 0;    // ★ 毎回 0 からスタート
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

	// ★★★ ここを追加：ベストショットのスイングフレームを確定 ★★★
	if (m_bestShotIndex != -1) {
		m_bestSwingFrame = m_swingFrame[m_bestShotIndex];
		m_bestPitchFrame = m_pitchFrame[m_bestShotIndex];
	}
}

void Game::GoToResult()
{
	// ★ 記録が1でもあれば SE2 を鳴らす
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

	auto start1 = FindGO<Start1>("start1");
	if (start1) DeleteGO(start1);

	auto pause = FindGO<PauseUI>("pause");
	if (pause) DeleteGO(pause);

	int best = max(m_scores[0], max(m_scores[1], m_scores[2]));
	Result* result = NewGO<Result>(0);
	if (result) {
		result->SetDifficulty(GetDifficulty());
	}
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

	// ★★★ 3球目リプレイ用：SE2 をここでもフェードアウト開始 ★★★
	if (g_soundManager) {
		g_soundManager->FadeOutSE2(2.0f);   // ← あなたの好きな秒数
	}

	m_InGameUI->m_onFadeOutFinished = [this]() {

		// 画面は真っ黒の状態

		if (m_bestShotIndex != -1) {
			// ベストショットがある → リプレイ開始
			StartReplay(m_bestShotIndex);

			// リプレイを見せるためにフェードイン
			if (m_InGameUI) {
				m_InGameUI->StartFadeIn(0.5f);
			}
		}
		else {
			// 全部空振り → そのままリザルトへ
			GoToResult();
		}
		};
}

int Game::GetReplayFrameCount() const {
	return m_replayFrameCounter;
}
void Game::OnPitcherThrow()
{
	int shot = m_shots;

	// ★ ピッチャーが投げた瞬間のフレームを保存
	m_pitchFrame[shot] = m_replayFrameCounter;

	if (m_batter) {
		m_batter->SetCursorMode(true); // これで2球目・3球目もスティックで動くようになります！
	}

	// ★ ボールを飛ばす
	if (m_ball) {
		m_ball->Throw({ 0, -20, 0 });
	}
}

void Game::Render(RenderContext& rc)
{

}