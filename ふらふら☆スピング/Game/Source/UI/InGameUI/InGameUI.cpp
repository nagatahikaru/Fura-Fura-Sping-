#include "stdafx.h"
#include "InGameUI.h"
#include"Source/Sound/SoundManager.h"
#include"Source/Scene/InGame/Game.h"


template <typename T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi) {
	return (v < lo) ? lo : (hi < v) ? hi : v;
}

InGameUI::InGameUI() {
	m_wakuModel.Init("Assets/stage/hurahura.tkm");
	m_spriteRenderBat.Init("Assets/sprite/batto.DDS", 330.0f, 430.0f);
	m_spriteRenderMeet.Init("Assets/sprite/mi-to.DDS", 45.0f, 45.0f);
	m_spriteRenderReplay.Init("Assets/sprite/REPLAY.DDS", 300.0f, 300.0f);
	m_spriteRenderBall.Init("Assets/sprite/ball.DDS", 30.0f, 30.0f);
	m_kiiro1.Init("Assets/sprite/kiiro.DDS", 680.0f, 600.0f);
	m_kiiro2.Init("Assets/sprite/kiiro.DDS", 680.0f, 600.0f);
	m_kiiro3.Init("Assets/sprite/kiiro.DDS", 680.0f, 600.0f);
	m_besu.Init("Assets/sprite/besu.DDS", 450.0f, 350.0f);
	m_baisoku.Init("Assets/sprite/baisoku.DDS", 350.0f, 350.0f);
	m_shuchusen.Init("Assets/sprite/shuchusen.DDS", 1920.0f, 1080.0f);
	m_shuchusen.SetPosition(Vector3{ 0.0f, 0.0f, 0.0f });
	m_shuchusen.SetMulColor({ 1,1,1,0 });
	m_imagesen.Init("Assets/sprite/imagesen.DDS", 1920.0f, 1080.0f);
	m_imagesen.SetPosition(Vector3{ 0.0f, 0.0f, 0.0f });
	m_imagesen.SetMulColor({ 1,1,1,0 });
	m_konto.Init("Assets/sprite/konto.DDS", 170.0f, 170.0f);
	m_yazirusi.Init("Assets/sprite/yazirusi.DDS", 440.0f, 320.0f);
	m_mawase.Init("Assets/sprite/mawase.DDS", 550.0f, 500.0f);
	m_taimingu.Init("Assets/sprite/taimingu.DDS", 550.0f, 500.0f);
	m_Abotan.Init("Assets/sprite/Abotann.DDS", 200.0f, 200.0f);
	m_Abotan2.Init("Assets/sprite/abotann2.DDS", 200.0f, 200.0f);
	m_gizagiza.Init("Assets/sprite/gizagiza.DDS", 200.0f, 200.0f);
	m_ballIcon[0].Init("Assets/sprite/ball.dds", 50.0f, 50.0f);
	m_ballIcon[1].Init("Assets/sprite/ball.dds", 50.0f, 50.0f);
	m_ballIcon[2].Init("Assets/sprite/ball.dds", 50.0f, 50.0f);
	m_ballIcon[0].SetPosition(Vector3{ -570, 430, 0 });
	m_ballIcon[1].SetPosition(Vector3{ -500, 430, 0 });
	m_ballIcon[2].SetPosition(Vector3{ -430, 430, 0 });
	m_spritekuro.Init("Assets/sprite/kuro.DDS", 1920.0f, 1080.0f);
	m_bbb.Init("Assets/sprite/bbb.dds", 200.0f, 200.0f);
	m_bsuki.Init("Assets/sprite/bsuki.DDS", 550.0f, 500.0f);
	m_strikeSprite.Init("Assets/sprite/strike.DDS", 600.0f, 500.0f);
	m_niceSprite.Init("Assets/sprite/nice.dds", 800, 700);
	m_greatSprite.Init("Assets/sprite/great.dds", 800, 700);
	m_excellentSprite.Init("Assets/sprite/excellent.dds", 800, 700);
	m_perfectSprite.Init("Assets/sprite/perfect.dds", 800, 700);
	m_batu[0].Init("Assets/sprite/batu.dds", 50.0f, 50.0f);
	m_batu[1].Init("Assets/sprite/batu.dds", 50.0f, 50.0f);
	m_batu[2].Init("Assets/sprite/batu.dds", 50.0f, 50.0f);
	m_guruguruSprite.Init("Assets/sprite/kiken.dds", 1000.0f,750.0f);
	m_ballMapIcon.Init("Assets/sprite/ball.dds", 30.0f, 30.0f); 
	m_gurahu.Init("Assets/sprite/gurahu.dds", 300.0f, 270.0f);
	m_kuro.Init("Assets/sprite/kuro.DDS", 300.0f, 270.0f);
	m_keisuu.Init("Assets/sprite/gurugurukeisuu.DDS", 400.0f, 300.0f);
	m_kakunin.Init("Assets/sprite/kakunin.DDS", 400.0f, 300.0f);
	m_kakin.Init("Assets/sprite/kakin.DDS", 500.0f, 500.0f);
	m_kiroku.Init("Assets/sprite/kiiro.DDS", 880.0f, 820.0f);
	m_easySprite.Init("Assets/sprite/Difficulty_Easy.DDS", 250.0f, 250.0f);
	m_normalSprite.Init("Assets/sprite/Difficulty_Normal.DDS", 180.0f, 180.0f);
	m_hardSprite.Init("Assets/sprite/Difficulty_Hard.DDS", 200.0f, 100.0f);
}

InGameUI::~InGameUI() {

}

bool InGameUI::Start() {
	m_batPositionRight = Vector3{ -50.0f, -100.0f, 0.0f };  // ← 初期位置
	m_batPositionLeft = Vector3{ 50.0f,-100.0f,0.0f };
	m_meetPositionRight = Vector3{ 39.0f, 5.0f, 0.0f };
	m_meetPositionLeft = Vector3{ -46.0f,7.0f,0.0f };
	m_ballCount = 3;
	return true;
}

void InGameUI::Update() {
	if (m_shuchusenTimer > 0.0f) {
		m_shuchusenTimer -= g_gameTime->GetFrameDeltaTime();

		float alpha = m_shuchusenTimer / 0.2f; // 0.2秒で消える
		m_shuchusen.SetMulColor({ 1.0f, 1.0f, 1.0f, alpha });
	}

	// ★ 矢印の角度を更新（毎秒180度）
	m_yazirusiAngleDeg -= 180.0f * g_gameTime->GetFrameDeltaTime();

	// ★ Z軸回転のクォータニオンを作る
	float rad = m_yazirusiAngleDeg * 3.14159265f / 180.0f;
	m_yazirusiRotation.SetRotation(Vector3::AxisZ, rad);

	if (m_buttonPressTimer > 0.0f) {
		m_buttonPressTimer -= g_gameTime->GetFrameDeltaTime();
		if (m_buttonPressTimer <= 0.0f) {
			m_isButtonPressed = false;
		}
	}

	m_uiToggleTimer += g_gameTime->GetFrameDeltaTime();
	if (m_uiToggleTimer >= 0.5f) {
		m_uiToggleTimer = 0.0f;
		m_isAltUI = !m_isAltUI;   // true / false を交互に切り替え
	}

	if (m_isFadeOut) {
		m_fadeAlpha += m_fadeSpeed * g_gameTime->GetFrameDeltaTime();

		if (m_fadeAlpha >= 1.0f) {
			m_fadeAlpha = 1.0f;
			m_isFadeOut = false;

			// ★ フェードアウト完了通知
			if (m_onFadeOutFinished) {
				m_onFadeOutFinished();
			}
		}
	}
	if (m_isFadeIn) {
		m_fadeAlpha -= m_fadeSpeed * g_gameTime->GetFrameDeltaTime();
		if (m_fadeAlpha <= 0.0f) {
			m_fadeAlpha = 0.0f;
			m_isFadeIn = false;

			// ★ フェードイン完了通知
			if (m_onFadeInFinished) {
				m_onFadeInFinished();
			}
		}
	}
	if (m_isPredictionAnim) {

		// ① 拡大アニメ（0.4秒）
		if (m_predictionAnimTimer < 0.4f) {

			m_predictionAnimTimer += g_gameTime->GetFrameDeltaTime();
			float t = m_predictionAnimTimer / 0.4f;
			if (t > 1.0f) t = 1.0f;

			if (t < 0.7f)
				m_predictionScale = Lerp(0.3f, 1.2f, t / 0.7f);
			else
				m_predictionScale = Lerp(1.2f, 1.0f, (t - 0.7f) / 0.3f);

			m_predictionAlpha = Lerp(0.0f, 1.0f, t);
		}
		else {
			// ② ホールド（1秒）
			m_predictionHoldTime -= g_gameTime->GetFrameDeltaTime();

			if (m_predictionHoldTime <= 0.0f) {
				m_isPredictionAnim = false;
				m_isPredictionVisible = false;  // 完全終了
			}
		}
	}

	// ★ 確認UI：パッと拡大 → 元に戻る の瞬間アニメ
	if (m_isKakuninFlash) {

		m_kakuninFlashTimer += g_gameTime->GetFrameDeltaTime();

		float t = fmodf(m_kakuninFlashTimer, 1.0f); // 0.4秒周期

		if (t < 0.5f) {
			// 最初の0.1秒だけ拡大
			m_kakuninScale = 1.5f;
		}
		else {
			// それ以外は通常サイズ
			m_kakuninScale = 1.0f;
		}
	}
	Game* game = FindGO<Game>("game");
	bool isKakutei = (game && game->m_isKakutei);

	// ★ 集中線用のアニメーションタイマーを進める
	m_animeTimer += g_gameTime->GetFrameDeltaTime();

	if (!isKakutei) {
		if (m_shuchusenTimer > 0.0f) {
			m_shuchusenTimer -= g_gameTime->GetFrameDeltaTime();

			float alpha = m_shuchusenTimer / 0.2f; // 0.2秒で消える
			m_shuchusen.SetMulColor({ 1.0f, 1.0f, 1.0f, 0.0f });
			m_imagesen.SetMulColor({ 1.0f, 1.0f, 1.0f, alpha }); // ★ 白も連動
		}
		else {
			m_shuchusen.SetMulColor({ 1.0f, 1.0f, 1.0f, 0.0f });
			m_imagesen.SetMulColor({ 1.0f, 1.0f, 1.0f, 0.0f }); // ★ オフの時は完全透明
		}
	}
	else {
		// ★ 確定演出中は常に最大表示
		m_shuchusen.SetMulColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		m_imagesen.SetMulColor({ 1.0f, 1.0f, 1.0f, 1.0f }); // ★ 白も全開
	}

	// ★★★ 動画編集風：超高速シャカシャカ動かす計算 ★★★
	if (m_shuchusenTimer > 0.0f || isKakutei) {

		// 白（m_imagesen）は、黒とタイミング・大きさをあえて「ズラす」ことでチカチカ感を出す
		float whiteScale = 1.0f + fabsf(cosf(m_animeTimer * 140.0f)) * 0.20f;
		float whiteRot = cosf(m_animeTimer * 100.0f) * 0.05f; // 黒とは逆のブレを作る
		Quaternion qWhite;
		qWhite.SetRotation(Vector3::AxisZ, whiteRot);

		m_imagesen.SetScale(Vector3{ whiteScale, whiteScale, 1.0f });
		m_imagesen.SetRotation(qWhite);
	}

	// ★★★ パーフェクト（kakin）専用のスケール・透明度計算 ★★★
	if (m_isPerfectAnimActive) {
		m_perfectAnimTimer += g_gameTime->GetFrameDeltaTime();

		// 【前半 0.5秒】：奥から手前へグワッとズームイン
		if (m_perfectAnimTimer <= 0.3f) {
			float t = m_perfectAnimTimer / 0.3f; // 0.5秒で規格化

			// スケールを奥（0.1倍）から手前（1.5倍）へLerp
			m_predictionScale = Lerp(0.1f, 1.5f, t);
			m_predictionAlpha = Lerp(0.0f, 1.0f, t);
		}
		// 【後半 0.5秒】：最大サイズのままピタッと静止
		else if (m_perfectAnimTimer <= 1.0f) {
			m_predictionScale = 1.5f;
			m_predictionAlpha = 1.0f;
		}
		else {
			// 1秒経ったらアニメーション終了（必要に応じてフラグを倒す、または消去フェードへ）
			// m_isPerfectAnimActive = false; 
		}
	}
}

//バットの位置を設定
void InGameUI::SetBatPosition(const Vector3& pos) {
	m_batPositionRight = pos;
}

//バットの回転を設定
void InGameUI::SetBatRotation(float rotDeg) {
	m_rad = rotDeg * 3.14159265f / 180.0f;
	m_batRotation.SetRotation(Vector3::AxisZ, m_rad);
}

void InGameUI::SetBatterSide(bool isLeft) {
	m_isLeftBatter = isLeft;
}

void InGameUI::SetPause(bool isPaused) {
	m_isPaused = isPaused;
}

void InGameUI::SetPredictedBallPos(const Vector3& pos3D) {

	// ★ 固定中は上書き禁止！
	if (m_isBallUIFixed) {
		return;
	}

	// ★ 後ろに完全に飛んだときだけエラー（誤差 20 を無視）
	if (pos3D.z > m_startZ + 20.0f) {
		m_isError = true;
	}

	m_predictedBallPos3D = pos3D;
	m_hasPredictedBall = true;

	// ★ 距離に応じて透明度を決める（非線形）
	float startZ = m_startZ;
	float currentZ = pos3D.z;

	// 0〜1 の線形 t
	float t = (startZ - currentZ) / (startZ - 6200.0f);
	t = clamp(t, 0.0f, 1.0f);

	// ★ アルファ値の計算（0.5 = 半透明、0.0 = 完全透明）
	float alpha = 0.5f;

	if (t < 0.30f) {
		// 最初の30%は 0.7f を維持
		alpha = 0.5f;
	}
	else {
		// 0.30〜1.0 を 0〜1 に圧縮
		float u = (t - 0.30f) / 0.70f;

		// 二乗でゆっくり立ち上がり、最終的に 0.0f（完全透明）にする
		// u=0 のとき alpha=0.7f、u=1 のとき alpha=0.0f
		alpha = 0.5f * (1.0f - u);
	}

	// 常に最新のアルファ値を適用（ifの外に出すことでバグを防止）
	m_ballAlpha = alpha;
}

Vector3 InGameUI::ConvertBall3DToUI(const Vector3& ballPos3D)
{
	// Z の進み具合（0 = 手前、1 = 奥）
	float minZ = 1000.0f;
	float maxZ = 6500.0f;
	float t = (ballPos3D.z - minZ) / (maxZ - minZ);
	t = clamp(t, 0.0f, 1.0f);

	// ★ 横移動 = X の動き + Z による中央寄り
	float xFromX = -ballPos3D.x * 0.22f;       // ← X の動きを8倍（調整しやすい）
	float xFromZ = (0.5f - t) * 0.5f;

	float uiX = xFromX + xFromZ;

	// 縦はそのまま
	float uiY = (ballPos3D.y - 750.0f) * 0.15f - 80.0f;

	return Vector3{ uiX, uiY, 0.0f };
}

void InGameUI::FixBallUI(const Vector3& pos3D)
{
	m_isBallUIFixed = true;
	m_fixedBallUIPos = ConvertBall3DToUI(pos3D);
}

void InGameUI::SetMeetCursorPosition(Vector3 m_inputOffset)
{

	m_batPos = m_isLeftBatter ? m_batPositionLeft : m_batPositionRight;
	Vector3 meetOffset = m_isLeftBatter ? m_meetPositionLeft : m_meetPositionRight;

	Vector3 baseMeetPos = m_batPos + meetOffset;

	m_meetPos = baseMeetPos + m_inputOffset;
	m_batPos = m_meetPos - meetOffset;
}

void InGameUI::SetStartZ(float z) {
	m_startZ = z;
	m_isError = false;   // 打つたびにリセット

	// ★ ここを追加：ボール予測UIの状態もリセット
	m_isBallUIFixed = false;
	m_hasPredictedBall = false;
	m_ballAlpha = 0.0f;
	ResetPrediction();
}

void InGameUI::SetGuruGuruTimer(float time)
{
	m_guruGuruTimer = time;
}

void InGameUI::OnButtonPressed() {
	m_isButtonPressed = true;
	m_buttonPressTimer = 0.1f; // 0.1秒だけ押し込み表示
}

void InGameUI::SetBaisokuVisible(bool isVisible)
{
	m_isBaisokuVisible = isVisible;
}

void InGameUI::SetBallCount(int count)
{
	m_ballCount = count;
}

int InGameUI::GetGuruGuruCount() const {
	return m_guruGuruCount;
}

void InGameUI::StartFadeOut(float speed)
{
	m_isFadeOut = true;
	m_fadeSpeed = speed;
}

void InGameUI::StartFadeIn(float speed)
{
	m_isFadeIn = true;
	m_isFadeOut = false;
	m_fadeSpeed = speed;
	m_fadeAlpha = 0.7f;   // 真っ黒からスタートして 0 に戻す
}

void InGameUI::StartStrikeAnim()
{
	m_strikeTimer = 0.0f;
	m_strikeHoldTime = 1.5f;   // ★ 1秒残す
	m_isStrikeAnim = true;
	m_strikeSprite.SetScale({ 0.3f, 0.3f,1.0f });
	m_strikeSprite.SetMulColor({ 1,1,1,0 }); // 透明
}

void InGameUI::ShowPrediction(float prDistance)
{
	m_predictedDistance = prDistance;
	m_isPredictionVisible = true;
	m_predictionAlpha = 1.0f; // 必要に応じてフェードインの初期値など

	// 🌟 距離が 80m 未満なら「ゴロ」として扱う
	if (m_predictedDistance < 24000.0f)
	{
		m_predictionType = Prediction_Goro;
	}
	else
	{
		if (m_predictedDistance < 80000.0f) {
			m_predictionType = Prediction_Nice;
			g_soundManager->PlaySE(Sound::enSound_SE7, 100.0f);  // ★ ナイス音
		}
		else if (m_predictedDistance < 93000.0f) {
			m_predictionType = Prediction_Great;
			g_soundManager->PlaySE(Sound::enSound_SE8, 100.0f);  // ★ グレイト音
		}
		else if (m_predictedDistance < 100000.0f) {
			m_predictionType = Prediction_Excellent;
			g_soundManager->PlaySE(Sound::enSound_SE9, 100.0f);  // ★ エクセレント音
		}
		else {
			m_predictionType = Prediction_Perfect;
			g_soundManager->PlaySE(Sound::enSound_SE10, 100.0f); // ★ パーフェクト音
		}
	}
	// ★ アニメ開始
	m_isPredictionAnim = true;
	m_predictionAnimTimer = 0.0f;
	m_predictionHoldTime = 1.5f;
	m_predictionScale = 0.3f;
	m_predictionAlpha = 0.0f;
	m_predictedDistance = floorf(m_predictedDistance + 0.005f) / 100.0f;
	m_isPredictionVisible = true;
}

void InGameUI::ResetBatAndMeetOnly()
{
	// 1. バット・ミートゾーンのベース座標を初期値にリセット
	m_batPositionRight = Vector3{ -50.0f, -100.0f, 0.0f };
	m_batPositionLeft = Vector3{ 50.0f, -100.0f, 0.0f };
	m_meetPositionRight = Vector3{ 39.0f, 5.0f, 0.0f };
	m_meetPositionLeft = Vector3{ -46.0f, 7.0f, 0.0f };

	// 2. 現在の移動先座標（m_batPos, m_meetPos）も初期位置（オフセットなし）にリセット
	m_batPos = m_isLeftBatter ? m_batPositionLeft : m_batPositionRight;
	Vector3 meetOffset = m_isLeftBatter ? m_meetPositionLeft : m_meetPositionRight;
	m_meetPos = m_batPos + meetOffset;

	// 3. バットの回転（角度）を 0 にリセット
	m_rad = 0.0f;
	m_batRotation.SetRotation(Vector3::AxisZ, 0.0f);

	// 4. スケール（左右反転フラグ）の再設定
	batScaleX = m_isLeftBatter ? -1.0f : 1.0f;
	m_meetScaleX = m_isLeftBatter ? -1.0f : 1.0f;
}

// InGameUI.cpp の一番下などでOK
void InGameUI::ResetPrediction() {
	m_isPredictionVisible = false;
	m_predictedDistance = 0.0f;
	m_predictionAlpha = 0.0f; // 透明度もリセットしておくと安全
}

void InGameUI::OnStrike(int ballIndex)
{
	if (ballIndex < 0 || ballIndex >= 3) return;

	m_isMiss[ballIndex] = true;  // ← この球は空振り
}

void InGameUI::Render(RenderContext& rc) {

	if (m_isPaused) {
		return; // ← ポーズ中は UI を一切描画しない
	}

	Game* game = FindGO <Game>("game");
	bool isReadyPhase = false;
	if (game) {
		isReadyPhase = game->m_isReadyPhase; 
	}

	bool isKakuteiMode = (game && game->GetCameraMode() == Camera_Kakutei);
	bool showShuchusen = (game && game->m_isKakutei) || isKakuteiMode || (m_shuchusenTimer > 0.0f);
	if (showShuchusen) {

		// 2. 上に白い集中線（m_imagesen）を重ねて描画
		m_imagesen.Update();
		m_imagesen.Draw(rc);

		// ★ カキーン演出のスケールと透明度を適用
		m_kakin.SetPosition(Vector3{ 0.0f, 100.0f, 0.0f });
		if (m_isPerfectAnimActive) {
			m_kakin.SetScale(Vector3{ m_predictionScale, m_predictionScale, 1.0f });
			m_kakin.SetMulColor({ 1.0f, 1.0f, 1.0f, m_predictionAlpha });
		}
		else {
			// アニメーション中でない場合のデフォルト表示（非表示にする場合はアルファを0に）
			m_kakin.SetScale(Vector3{ 1.0f, 1.0f, 1.0f });
			m_kakin.SetMulColor({ 1.0f, 1.0f, 1.0f, 0.0f });
		}

		m_kakin.Update();
		m_kakin.Draw(rc);
	}

	if (isKakuteiMode) {
		return; // ← ここでリターンしてUI全部消す
	}

	if (m_isUIVisible) {

		//赤い枠
		m_wakuModel.SetPosition(-10.0f, 318.0f, 6000.0f);
		m_wakuModel.SetScale(6.5f, 7.5f, 5.0f);
		m_wakuModel.Update();
		m_wakuModel.Draw(rc);

		// --- 左右でバット位置を切り替える ---
		Vector3 batPos = m_isLeftBatter ? m_batPositionLeft : m_batPositionRight;
		Vector3 meetOffset = m_isLeftBatter ? m_meetPositionLeft : m_meetPositionRight;

		//バット
		batScaleX = m_isLeftBatter ? -1.0f : 1.0f;
		m_spriteRenderBat.SetPosition(m_batPos);
		m_spriteRenderBat.SetRotation(m_batRotation);
		m_spriteRenderBat.SetScale(Vector3{ batScaleX,1.0f,1.0f });
		m_spriteRenderBat.Update();
		m_spriteRenderBat.Draw(rc);

		// --- ミートゾーン（バットと同じ位置・回転を参照） ---	
		m_meetScaleX = m_isLeftBatter ? -1.0f : 1.0f;
		m_spriteRenderMeet.SetPosition(m_meetPos);
		m_spriteRenderMeet.SetRotation(m_batRotation);
		m_spriteRenderMeet.SetScale({m_meetScaleX * m_meetCursorScale,m_meetCursorScale,1.0f});
		m_spriteRenderMeet.Update();
		m_spriteRenderMeet.Draw(rc);

		m_kuro.SetPosition(Vector3{ -800.0f,-70.0f,0.0f });
		m_kuro.SetMulColor({ 0,0,0,0.5 });
		m_kuro.Update();
		m_kuro.Draw(rc);

		m_gurahu.SetPosition(Vector3{ -800.0f,-70.0f,0.0f });
		m_gurahu.Update();
		m_gurahu.Draw(rc);

		m_keisuu.SetPosition(Vector3{ -800.0f,120.0f,0.0f });
		m_keisuu.Update();
		m_keisuu.Draw(rc);

		double t = (double)m_guruGuruCount / 50.0;
		t = clamp(t, 0.0, 1.0);

		double currentMultiplier = 1.0 + 49.0 * pow(t, 2.5);

		float progressRatioY = (float)((currentMultiplier - 1.0) / 49.0);
		progressRatioY = clamp(progressRatioY, 0.0f, 1.0f);

		float progressRatioX = (float)t;

		Vector3 graphBasePos = Vector3{ -930.0f, -190.0f, 0.0f };

		Vector3 ballMapPos;
		ballMapPos.x = graphBasePos.x + (progressRatioX * m_miniMapHeightX);
		ballMapPos.y = graphBasePos.y + (progressRatioY * m_miniMapHeightY);
		ballMapPos.z = 0.0f;

		m_ballMapIcon.SetPosition(ballMapPos);
		m_ballMapIcon.Update();
		m_ballMapIcon.Draw(rc);

	

		if (m_hasPredictedBall&& m_game && m_game->GetDifficulty() != Difficulty::Hard) {

			Vector3 uiPos;

			if (m_isBallUIFixed) {
				uiPos = m_fixedBallUIPos;   // ← 変換しない
			}
			else {
				uiPos = ConvertBall3DToUI(m_predictedBallPos3D);
			}

			m_spriteRenderBall.SetPosition(uiPos);

			// ★ 距離に応じた透明度を適用！
			Vector4 color = Vector4(1.0f, 1.0f, 1.0f, m_ballAlpha);
			m_spriteRenderBall.SetMulColor(color);

			m_spriteRenderBall.Update();
			m_spriteRenderBall.Draw(rc);
		}


		// ★ ぐるぐる中 or 打った後は Aボタン UI を出さない
		if (m_guruGuruTimer <= 0.0f && !m_isBallUIFixed && !isReadyPhase)
		{
			m_taimingu.SetPosition(Vector3{ 800.0f, 100.0f, 0.0f });
			m_taimingu.Update();
			m_taimingu.Draw(rc);

			// ★ 0.5秒ごとに m_isAltUI が true / false になる
			if (m_isAltUI) {
				// 交互UI：Aボタン2
				m_Abotan2.SetPosition(Vector3{ 800.0f, -130.0f, 0.0f });
				m_Abotan2.Update();
				m_Abotan2.Draw(rc);

				m_gizagiza.SetPosition(Vector3{ 800.0f, -30.0f, 0.0f });
				m_gizagiza.Update();
				m_gizagiza.Draw(rc);
			}
			else {
				// 交互UI：Aボタン
				m_Abotan.SetPosition(Vector3{ 800.0f, -130.0f, 0.0f });
				m_Abotan.Update();
				m_Abotan.Draw(rc);
			}
		}
		if (m_isStrikeAnim) {

			// ① 拡大アニメ（0.4秒）
			if (m_strikeTimer < 0.4f) {

				m_strikeTimer += g_gameTime->GetFrameDeltaTime();
				float t = m_strikeTimer / 0.4f;
				if (t > 1.0f) t = 1.0f;

				float scale;
				if (t < 0.7f)
					scale = Lerp(0.3f, 1.2f, t / 0.7f);
				else
					scale = Lerp(1.2f, 1.0f, (t - 0.7f) / 0.3f);

				m_strikeSprite.SetScale(Vector3{ scale, scale, 1.0f });

				float alpha = Lerp(0.0f, 1.0f, t);
				m_strikeSprite.SetMulColor({ 1,1,1,alpha });
			}
			else {
				// ② アニメ終了後の待機時間（1秒）
				m_strikeHoldTime -= g_gameTime->GetFrameDeltaTime();

				if (m_strikeHoldTime <= 0.0f) {
					m_isStrikeAnim = false;  // 完全終了
				}
			}

			// ★ 描画
			m_strikeSprite.Update();
			m_strikeSprite.Draw(rc);
		}

		bool shouldShowStageUI = false;
		if (game) {
			bool isReadyPhase = game->m_isReadyPhase; // 操作確認フラグを取得
			bool isCatcherAndNotGuruGuru = (game->GetCameraMode() == Camera_Catcher && m_guruGuruTimer <= 0.0f);

			if (isReadyPhase || isCatcherAndNotGuruGuru) {
				shouldShowStageUI = true;
			}
		}

		// 難易度別の回転数設定（1回だけ）
		int rotationPerLevel = 3;
		Difficulty diff = game->GetDifficulty();

		if (diff == Difficulty::Easy) {
			rotationPerLevel = 10;
		}
		else if (diff == Difficulty::Normal) {
			rotationPerLevel = 5;
		}
		else {
			rotationPerLevel = 3;
		}

		// ★ 警告スプライト表示（難易度別）
		if (shouldShowStageUI && m_guruGuruCount >= rotationPerLevel)
		{
			m_guruguruSprite.SetMulColor({ 1,1,1,1 });
			m_guruguruSprite.SetPosition(Vector3{ 0.0f,465.0f, 0.0f });
			m_guruguruSprite.Update();
			m_guruguruSprite.Draw(rc);
		}
		else
		{
			m_guruguruSprite.SetMulColor({ 1,1,1,0 });
		}

		// ★ ステージテキスト表示（同じ rotationPerLevel を使う）
		if (shouldShowStageUI && m_guruGuruCount >= rotationPerLevel)
		{
			const wchar_t** stageList = nullptr;
			int maxStage = 0;

			if (diff == Difficulty::Easy) {
				stageList = m_stageTextEasy;
				maxStage = 3;
			}
			else if (diff == Difficulty::Normal) {
				stageList = m_stageTextNormal;
				maxStage = 7;
			}
			else {
				stageList = m_stageTextHard;
				maxStage = 15;
			}

			int stage = (m_guruGuruCount / rotationPerLevel) - 1;
			stage = clamp(stage, 0, maxStage - 1);

			m_fontStage[stage].SetText(stageList[stage]);
			m_fontStage[stage].SetPosition(-270.0f, 500.0f, 0.0f);
			m_fontStage[stage].SetScale(0.8f);
			m_fontStage[stage].SetColor(0, 0, 0, 1);
			m_fontStage[stage].Draw(rc);
		}
	}

	if (m_isFontVisible) {

		wchar_t kyu[64];
		swprintf_s(kyu, 64, L"のこり%d球", m_ballCount);

		m_fontBallCount.SetText(kyu);
		m_fontBallCount.SetPosition(-600.0f, 500.0f, 0.0f); // 位置は調整してOK
		m_fontBallCount.SetColor(0.0f, 0.0f, 0.0f, 1.0f);
		m_fontBallCount.Draw(rc);

		for (int i = 0; i < 3; i++) {
			if (i < m_ballCount) {
				m_ballIcon[i].SetMulColor({ 1,1,1,1 });   // 表示
			}
			else {
				m_ballIcon[i].SetMulColor({ 1,1,1,0 });   // 非表示
			}
			m_ballIcon[i].Update();
			m_ballIcon[i].Draw(rc);
		}

		m_kiiro1.SetPosition(Vector3{ 905.0f, 470.0f, 0.0f });
		m_kiiro1.Update();
		m_kiiro1.Draw(rc);
		m_kiiro2.SetPosition(Vector3{ 905.0f, 370.0f, 0.0f });
		m_kiiro2.Update();
		m_kiiro2.Draw(rc);
		m_kiiro3.SetPosition(Vector3{ 905.0f, 270.0f, 0.0f });
		m_kiiro3.Update();
		m_kiiro3.Draw(rc);

		m_besu.SetPosition(Vector3{ -780.0f, 360.0f, 0.0f });
		m_besu.Update();
		m_besu.Draw(rc);

		// ★ ストライク（バツ）スプライトを縦に並べて表示
		for (int i = 0; i < 3; i++) {

			if (m_isMiss[i]) {
				// 位置：左上あたりに縦並び（好きな位置に調整OK）
				float baseX = 760.0f;
				float baseY = 482.5f;

				m_batu[i].SetPosition(Vector3{
					baseX,
					baseY - i * 100.0f,   // ← 縦に並べる
					0.0f
					});

				m_batu[i].SetScale({ 2.0f, 2.0f, 1.0f }); // 大きさ調整（必要なら）
				m_batu[i].SetMulColor({ 1, 0, 0, 1 });      // 赤色で表示
			}
			else {
				m_batu[i].SetMulColor({ 1, 1, 1, 0 });      // 非表示
			}

			m_batu[i].Update();
			m_batu[i].Draw(rc);
		}

		// --- ぐるぐるカウント色変更 ---
	// --- ぐるぐるカウント色変更（5刻み） ---
		// --- ぐるぐるカウント色変更（5刻み） ---
		//int c = m_guruGuruCount;
		//float r2, g2, b2;

		//// 0〜4：黒
		//if (c < 10) {
		//	r2 = 0.0f; g2 = 0.0f; b2 = 0.0f;
		//}
		//// 5〜9：青
		//else if (c < 15) {
		//	r2 = 0.0f; g2 = 0.0f; b2 = 1.0f;
		//}
		//// 15〜19：黄緑（ライム）
		//else if (c < 20) {
		//	r2 = 0.0f; g2 = 1.0f; b2 = 0.0f;
		//}
		//// 20〜24：オレンジ
		//else if (c < 25) {
		//	r2 = 1.0f; g2 = 0.5f; b2 = 0.0f;
		//}
		//// 25以上：赤
		//else {
		//	r2 = 1.0f; g2 = 0.0f; b2 = 0.0f;
		//}

		m_fontRender.SetColor(1, 1, 1, 1.0f);
		wchar_t text[256];
		swprintf_s(text, 256, L"ぐるぐる:%d", (int)m_guruGuruCount);
		m_fontRender.SetText(text);
		m_fontRender.SetScale(0.9);
		m_fontRender.SetPosition(-895.0f, 375.0f, 0.0f);
		m_fontRender.Draw(rc);

		wchar_t timerText[256];
		// 小数1桁で表示（例：4.8）
	// 小数1桁で表示（例：4.8）
		float displayTime = max(0.0f, m_guruGuruTimer);
		if (isReadyPhase) {
			swprintf_s(timerText, 256, L"スタート:%.1f", displayTime);
			m_Count.SetScale(0.7);
		}
		else {
			// ② 通常時（バットを回している最中など）
			swprintf_s(timerText, 256, L"タイム: %.1f", displayTime);
			m_Count.SetScale(0.7);
		}
		m_Count.SetText(timerText);
		m_Count.SetPosition(-880.0f, 430.0f, 0.0f);
		// 🌟【重要：文字色の制御】
		if (isReadyPhase) {
			// 操作確認フェーズ中は点滅させず、黒色でハッキリ5秒間カウントダウンさせる
			m_Count.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else if (displayTime <= 0.0f) {
			m_Count.SetColor(1.0f, 1.0f, 1.0f, 0.0f);
		}
		else if (displayTime > 3.0f) {
			m_Count.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else {
			// 残り3秒未満の時は赤点滅
			float blink = fabsf(sinf(displayTime * 10.0f));
			m_Count.SetColor(1.0f, 0.0f, 0.0f, blink);
		}
		m_Count.Draw(rc);

		if (isReadyPhase) {
			m_kakunin.SetPosition(Vector3{ 800.0f, 80.0f, 0.0f });
			m_kakunin.SetScale(Vector3{ m_kakuninScale, m_kakuninScale, 1.0f }); // ← 追加
			m_kakunin.Update();
			m_kakunin.Draw(rc);
			m_bbb.SetPosition(Vector3{ 800.0f, -30.0f, 0.0f });
			m_bbb.Update();
			m_bbb.Draw(rc);
			m_bsuki.SetPosition(Vector3{ 800.0f, -130.0f, 0.0f });
			m_bsuki.Update();
			m_bsuki.Draw(rc);
		}

		wchar_t boll[256];
		if (m_isError) {
			double meter = (double)m_km / 100.0;
			swprintf_s(boll, L"%.2f m", meter);
		}
		else {
			double meter = (double)m_km / 100.0;
			swprintf_s(boll, L"%.2f m", meter);
		}
		// 1球目〜3球目のスコア表示
		for (int i = 0; i < 3; i++) {

			wchar_t buf[256];

			if (!m_shotDone[i]) {
				swprintf_s(buf, L"%d:--- m", i + 1);
			}
			else {
				if (m_threeShots[i] == 0) {
					swprintf_s(buf, L"%d: ", i + 1);
				}
				else
				{
					double meter = (double)m_threeShots[i] / 100.0;
					swprintf_s(buf, L"%d:%.2f m", i + 1, meter);
				}
			}

			float y = 505.0f - i * 100.0f;  // 縦位置をずらす

			if (i == 0) {
				m_fontBollRender1.SetText(buf);
				m_fontBollRender1.SetPosition(654.0f, y, 0.0f);
				m_fontBollRender1.SetColor(0, 0, 0, 1);
				m_fontBollRender1.Draw(rc);
			}
			else if (i == 1) {
				m_fontBollRender2.SetText(buf);
				m_fontBollRender2.SetPosition(654.0f, y, 0.0f);
				m_fontBollRender2.SetColor(0, 0, 0, 1);
				m_fontBollRender2.Draw(rc);
			}
			else {
				m_fontBollRender3.SetText(buf);
				m_fontBollRender3.SetPosition(654.0f, y, 0.0f);
				m_fontBollRender3.SetColor(0, 0, 0, 1);
				m_fontBollRender3.Draw(rc);
			}
		}

		Game* game = FindGO<Game>("game");
		bool isFollowCamera = false;

		if (game) {
			CameraMode cam = game->GetCameraMode();
			isFollowCamera = (cam == Camera_Ball || cam == Camera_BackBall);
		}

		if (m_isPredictionVisible && isFollowCamera) {

			m_kiroku.SetPosition(Vector3{ 180.0f, -205.0f, 0.0f });
			m_kiroku.Update();
			m_kiroku.Draw(rc);

			SpriteRender* spr = nullptr;

			if (m_predictionType == Prediction_Nice) {
				spr = &m_niceSprite;
			}
			else if (m_predictionType == Prediction_Great) {
				spr = &m_greatSprite;
			}
			else if (m_predictionType == Prediction_Excellent) {
				spr = &m_excellentSprite;
			}
			else if (m_predictionType == Prediction_Perfect) { // ★ 追加
				spr = &m_perfectSprite;
			}

			if (spr) {
				spr->SetPosition({ 0,0,0 });
				spr->SetScale({ m_predictionScale, m_predictionScale, 1.0f });
				spr->SetMulColor({ 1,1,1, m_predictionAlpha });
				spr->Update();
				spr->Draw(rc);
			}

			wchar_t predText[64];
			swprintf_s(predText, L"%.2f m", m_predictedDistance);
			m_fontPrediction.SetText(predText);
			m_fontPrediction.SetPosition(-150.0f, -150.0f, 0.0f);
			m_fontPrediction.SetScale(1.5f);

			// ★ 1000を超えたら赤、それ以外は黒にする条件分岐
			if (m_predictedDistance > 1000.0f) {
				m_fontPrediction.SetColor(1.0f, 0.0f, 0.0f, m_predictionAlpha); // 赤色
			}
			else {
				m_fontPrediction.SetColor(0.0f, 0.0f, 0.0f, m_predictionAlpha); // 元の黒色
			}

			m_fontPrediction.Draw(rc);
		}

		if (m_guruGuruTimer > 0.0 && !isReadyPhase) {
			m_konto.SetPosition(Vector3{ 800.0f, 0.0f, 0.0f });
			m_konto.Update();
			m_konto.Draw(rc);

			// ★ 矢印の回転描画
			m_yazirusi.SetPosition(Vector3{ 800.0f, -5.0f, 0.0f }); // 位置はお好みで
			m_yazirusi.SetRotation(m_yazirusiRotation);               // ← Quaternion を渡す
			m_yazirusi.Update();
			m_yazirusi.Draw(rc);

			m_mawase.SetPosition(Vector3{ 800.0f, -230.0f, 0.0f }); // 位置はお好みで
			m_mawase.Update();
			m_mawase.Draw(rc);
		}

		if (game) {
			Difficulty diff = game->GetDifficulty();
			SpriteRender* pDiffSprite = nullptr;

			// 現在の難易度に応じて描画するスプライトを決定
			if (diff == Difficulty::Easy) {
				pDiffSprite = &m_easySprite;
			}
			else if (diff == Difficulty::Normal) {
				pDiffSprite = &m_normalSprite;
			}
			else if (diff == Difficulty::Hard) { // 必要に応じて異なるDifficulty列挙型に合わせてください
				pDiffSprite = &m_hardSprite;
			}

			// スプライトが存在すれば位置を設定して描画
			if (pDiffSprite) {
				// 表示位置（画面左上あたり、バスカットや残り球数の邪魔にならない位置に調整してください）
				pDiffSprite->SetPosition(Vector3{ -770.0f, 280.0f, 0.0f });
				pDiffSprite->Update();
				pDiffSprite->Draw(rc);
			}
		}
	}

	if (m_isBaisokuVisible) {
		m_baisoku.SetPosition(Vector3{ -800.0f, 0.0f, 0.0f });
		m_baisoku.Update();
		m_baisoku.Draw(rc);
	}

	if (m_isReplayVisible) {
		m_spriteRenderReplay.SetPosition(Vector3{ -800.0f, 450.0f, 0.0f });
		m_spriteRenderReplay.Update();
		m_spriteRenderReplay.Draw(rc);
		m_bbb.SetPosition(Vector3{ 700.0f, -300.0f, 0.0f });
		m_bbb.Update();
		m_bbb.Draw(rc);
		m_bsuki.SetPosition(Vector3{ 700.0f, -400.0f, 0.0f });
		m_bsuki.Update();
		m_bsuki.Draw(rc);
	}

	// ★ 黒フェード描画（常に最前面）
	if (m_fadeAlpha > 0.0f) {
		m_spritekuro.SetMulColor({ 0,0,0, m_fadeAlpha });
		m_spritekuro.SetPosition({ 0,0,0 });
		m_spritekuro.Update();
		m_spritekuro.Draw(rc);
	}
	
}