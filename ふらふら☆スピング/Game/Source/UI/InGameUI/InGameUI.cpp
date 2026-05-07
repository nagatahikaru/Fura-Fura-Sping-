#include "stdafx.h"
#include "InGameUI.h"


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
	m_kiiro1.Init("Assets/sprite/kiiro.DDS", 620.0f, 600.0f);
	m_kiiro2.Init("Assets/sprite/kiiro.DDS", 620.0f, 600.0f);
	m_kiiro3.Init("Assets/sprite/kiiro.DDS", 620.0f, 600.0f);
	m_besu.Init("Assets/sprite/besu.DDS", 400.0f, 450.0f);
	m_baisoku.Init("Assets/sprite/baisoku.DDS", 150.0f, 150.0f);
	m_shuchusen.Init("Assets/sprite/shuchusen.DDS", 1920.0f, 1080.0f);
	m_shuchusen.SetPosition(Vector3{ 0.0f, 0.0f, 0.0f });
	m_shuchusen.SetMulColor({ 1,1,1,0 }); // 最初は透明
	m_konto.Init("Assets/sprite/konto.DDS", 300.0f, 300.0f);
	m_yazirusi.Init("Assets/sprite/yazirusi.DDS", 130.0f, 100.0f);
	m_mawase.Init("Assets/sprite/mawase.DDS", 550.0f, 500.0f);
	m_taimingu.Init("Assets/sprite/taimingu.DDS", 550.0f, 500.0f);
	m_Abotan.Init("Assets/sprite/Abotann.DDS", 200.0f, 200.0f);
	m_Abotan2.Init("Assets/sprite/abotann2.DDS", 200.0f, 200.0f);
	m_gizagiza.Init("Assets/sprite/gizagiza.DDS", 200.0f, 200.0f);
	m_ballIcon[0].Init("Assets/sprite/ball.dds", 50.0f, 50.0f);
	m_ballIcon[1].Init("Assets/sprite/ball.dds", 50.0f, 50.0f);
	m_ballIcon[2].Init("Assets/sprite/ball.dds", 50.0f, 50.0f);
	m_ballIcon[0].SetPosition(Vector3{ -600, 430, 0 });
	m_ballIcon[1].SetPosition(Vector3{ -530, 430, 0 });
	m_ballIcon[2].SetPosition(Vector3{ -460, 430, 0 });
	m_spritekuro.Init("Assets/sprite/kuro.DDS",1920.0f, 1080.0f);
	m_bbb.Init("Assets/sprite/bbb.dds", 200.0f, 200.0f);
	m_bsuki.Init("Assets/sprite/bsuki.DDS", 550.0f, 500.0f);
	m_strikeSprite.Init("Assets/sprite/strike.DDS", 600.0f, 500.0f);
}

InGameUI::~InGameUI() {

}

bool InGameUI::Start() {
	m_batPositionRight = Vector3{-50.0f, -100.0f, 0.0f };  // ← 初期位置
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
	float t = (startZ - currentZ) / (startZ - 6000.0f);
	t = clamp(t, 0.0f, 1.0f);

	// ★ 中盤から上がるイージング（後半だけ立ち上がる）
	float eased = 0.0f;

	if (t < 0.30f) {
		eased = 0.0f;               // 最初の45%は完全透明
	}
	else {
		float u = (t - 0.30f) / 0.70f; // 0.45〜1.0 を 0〜1 に圧縮
		eased = u * u;               // 二乗でゆっくり立ち上がる

		m_ballAlpha = eased;
	}
}
Vector3 InGameUI::ConvertBall3DToUI(const Vector3& ballPos3D)
{
	// Z の進み具合（0 = 手前、1 = 奥）
	float minZ = 1000.0f;
	float maxZ = 6000.0f;
	float t = (ballPos3D.z - minZ) / (maxZ - minZ);
	t = clamp(t, 0.0f, 1.0f);

	// ★ 横移動 = X の動き + Z による中央寄り
	float xFromX = -ballPos3D.x * 4.5f;       // ← X の動きを8倍（調整しやすい）
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
    m_isBallUIFixed    = false;
    m_hasPredictedBall = false;
    m_ballAlpha        = 0.0f;
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
	m_strikeHoldTime = 1.0f;   // ★ 1秒残す
	m_isStrikeAnim = true;

	m_strikeSprite.SetScale({ 0.3f, 0.3f,1.0f });
	m_strikeSprite.SetMulColor({ 1,1,1,0 }); // 透明
}

void InGameUI::ShowHomeRun()
{
	m_isHomeRunVisible = true;
	m_homeRunTimer = 1.5f;  // 1.5秒表示
}

void InGameUI::Render(RenderContext& rc) {

	if (m_isPaused) {
		return; // ← ポーズ中は UI を一切描画しない
	}

	if (m_shuchusenTimer > 0.0f) {
		m_shuchusen.Update();
		m_shuchusen.Draw(rc);
	}

	if (m_isUIVisible) {

		//赤い枠
		m_wakuModel.SetPosition(50.0f, 318.0f, 6000.0f);
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
		m_spriteRenderMeet.SetScale(Vector3{ m_meetScaleX, 1.0f, 1.0f });
		m_spriteRenderMeet.Update();
		m_spriteRenderMeet.Draw(rc);

		if (m_hasPredictedBall) {

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
		if (m_guruGuruTimer <= 0.0f && !m_isBallUIFixed)
		{
			m_taimingu.SetPosition(Vector3{ -800.0f, 100.0f, 0.0f });
			m_taimingu.Update();
			m_taimingu.Draw(rc);

			// ★ 0.5秒ごとに m_isAltUI が true / false になる
			if (m_isAltUI) {
				// 交互UI：Aボタン2
				m_Abotan2.SetPosition(Vector3{ -800.0f, -130.0f, 0.0f });
				m_Abotan2.Update();
				m_Abotan2.Draw(rc);

				m_gizagiza.SetPosition(Vector3{ -800.0f, -30.0f, 0.0f });
				m_gizagiza.Update();
				m_gizagiza.Draw(rc);
			}
			else {
				// 交互UI：Aボタン
				m_Abotan.SetPosition(Vector3{ -800.0f, -130.0f, 0.0f });
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
	}

	if (m_isFontVisible) {

		wchar_t kyu[64];
		swprintf_s(kyu, 64, L"のこり%d球", m_ballCount);

		m_fontBallCount.SetText(kyu);
		m_fontBallCount.SetPosition(-630.0f, 500.0f, 0.0f); // 位置は調整してOK
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

		m_kiiro1.SetPosition(Vector3{ 915.0f, 470.0f, 0.0f });
		m_kiiro1.Update();
		m_kiiro1.Draw(rc);
		m_kiiro2.SetPosition(Vector3{ 915.0f, 370.0f, 0.0f });
		m_kiiro2.Update();
		m_kiiro2.Draw(rc);
		m_kiiro3.SetPosition(Vector3{ 915.0f, 270.0f, 0.0f });
		m_kiiro3.Update();
		m_kiiro3.Draw(rc);

		m_besu.SetPosition(Vector3{ -800.0f, 400.0f, 0.0f });
		m_besu.Update();
		m_besu.Draw(rc);

		// --- ぐるぐるカウント色変更 ---
	// --- ぐるぐるカウント色変更（5刻み） ---
		// --- ぐるぐるカウント色変更（5刻み） ---
		int c = m_guruGuruCount;
		float r2, g2, b2;

		// 0〜4：黒
		if (c < 10) {
			r2 = 0.0f; g2 = 0.0f; b2 = 0.0f;
		}
		// 5〜9：青
		else if (c < 15) {
			r2 = 0.0f; g2 = 0.0f; b2 = 1.0f;
		}
		// 15〜19：黄緑（ライム）
		else if (c < 20) {
			r2 = 0.0f; g2 = 1.0f; b2 = 0.0f;
		}
		// 20〜24：オレンジ
		else if (c < 25) {
			r2 = 1.0f; g2 = 0.5f; b2 = 0.0f;
		}
		// 25以上：赤
		else {
			r2 = 1.0f; g2 = 0.0f; b2 = 0.0f;
		}

		m_fontRender.SetColor(r2, g2, b2, 1.0f);
		wchar_t text[256];
		swprintf_s(text, 256, L"ぐるぐる:%d", (int)m_guruGuruCount);
		m_fontRender.SetText(text);
		m_fontRender.SetPosition(-930.0f, 500.0f, 0.0f);
		m_fontRender.Draw(rc);

		wchar_t timerText[256];
		// 小数1桁で表示（例：4.8）
	// 小数1桁で表示（例：4.8）
		float displayTime = max(0.0f, m_guruGuruTimer);
		swprintf_s(timerText, 256, L"タイム: %.1f", displayTime);
		m_Count.SetText(timerText);
		m_Count.SetPosition(-930.0f, 450.0f, 0.0f);

		// ★ タイムが0なら透明にする
		if (displayTime <= 0.0f) {
			m_Count.SetColor(0.0f, 0.0f, 0.0f, 0.0f);  // 完全透明
		}
		else if (displayTime > 3.0f) {
			m_Count.SetColor(0.0f, 0.0f, 0.0f, 1.0f);  // 黒固定
		}
		else {
			// ★ 点滅処理（0.2秒周期）
			float blink = fabsf(sinf(displayTime * 10.0f));  // 0〜1 の点滅
			float alpha = blink;  // 0〜1 の範囲

			// 赤で点滅
			m_Count.SetColor(1.0f, 0.0f, 0.0f, alpha);
		}
		m_Count.Draw(rc);

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
				double meter = (double)m_threeShots[i] / 100.0;
				swprintf_s(buf, L"%d:%.2f m", i + 1, meter);
			}

			float y = 505.0f - i * 100.0f;  // 縦位置をずらす

			if (i == 0) {
				m_fontBollRender1.SetText(buf);
				m_fontBollRender1.SetPosition(684.0f, y, 0.0f);
				m_fontBollRender1.SetColor(0, 0, 0, 1);
				m_fontBollRender1.Draw(rc);
			}
			else if (i == 1) {
				m_fontBollRender2.SetText(buf);
				m_fontBollRender2.SetPosition(684.0f, y, 0.0f);
				m_fontBollRender2.SetColor(0, 0, 0, 1);
				m_fontBollRender2.Draw(rc);
			}
			else {
				m_fontBollRender3.SetText(buf);
				m_fontBollRender3.SetPosition(684.0f, y, 0.0f);
				m_fontBollRender3.SetColor(0, 0, 0, 1);
				m_fontBollRender3.Draw(rc);
			}
		}


		if (m_isBaisokuVisible) {
			m_baisoku.SetPosition(Vector3{ -800.0f, 400.0f, 0.0f });
			m_baisoku.Update();
			m_baisoku.Draw(rc);

		}

		if (m_guruGuruTimer > 0.0) {
			m_konto.SetPosition(Vector3{ -800.0f, 0.0f, 0.0f });
			m_konto.Update();
			m_konto.Draw(rc);
			
			// ★ 矢印の回転描画
			m_yazirusi.SetPosition(Vector3{ -840.0f, -5.0f, 0.0f }); // 位置はお好みで
			m_yazirusi.SetRotation(m_yazirusiRotation);               // ← Quaternion を渡す
			m_yazirusi.Update();
			m_yazirusi.Draw(rc);

			m_mawase.SetPosition(Vector3{ -800.0f, -180.0f, 0.0f }); // 位置はお好みで
			m_mawase.Update();
			m_mawase.Draw(rc);
		}
	}
	if (m_isReplayVisible) {
		m_spriteRenderReplay.SetPosition(Vector3{ -800.0f, 450.0f, 0.0f });
		m_spriteRenderReplay.Update();
		m_spriteRenderReplay.Draw(rc);
		m_bbb.SetPosition(Vector3{ 700.0f, 400.0f, 0.0f });
		m_bbb.Update();
		m_bbb.Draw(rc);
		m_bsuki.SetPosition(Vector3{ 700.0f, 300.0f, 0.0f });
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
	//wchar_t dbg[256];

	//swprintf_s(dbg, 256, L"Ball Z = %.1f", m_predictedBallPos3D.z);
	//m_fontDebug1.SetText(dbg);
	//m_fontDebug1.SetPosition(300.0f, 500.0f, 0.0f);
	//m_fontDebug1.SetColor(0, 0, 0, 1);
	//m_fontDebug1.Draw(rc);

	//Vector3 uiPos = m_isBallUIFixed
	//	? m_fixedBallUIPos
	//	: ConvertBall3DToUI(m_predictedBallPos3D);
	//uiPos.y -= 0.0f;   // ★ これを追加
	//swprintf_s(dbg, 256, L"UI Pos = (%.1f, %.1f)", uiPos.x, uiPos.y);
	//m_fontDebug2.SetText(dbg);
	//m_fontDebug2.SetPosition(300.0f, 460.0f, 0.0f);
	//m_fontDebug2.SetColor(0, 0, 0, 1);
	//m_fontDebug2.Draw(rc);

	//if (m_isBallUIFixed) {
	//	swprintf_s(dbg, 256, L"Fixed UI Pos = (%.1f, %.1f)",
	//		m_fixedBallUIPos.x, m_fixedBallUIPos.y);
	//	m_fontDebug3.SetText(dbg);
	//	m_fontDebug3.SetPosition(300.0f, 420.0f, 0.0f);
	//	m_fontDebug3.SetColor(1, 0, 0, 1);
	//	m_fontDebug3.Draw(rc);
	//}

	//swprintf_s(dbg, 256, L"Ball X = %.2f", m_predictedBallPos3D.x);
	//m_fontDebug4.SetText(dbg);
	//m_fontDebug4.SetPosition(300.0f, 380.0f, 0.0f);
	//m_fontDebug4.SetColor(0, 0, 0, 1);
	//m_fontDebug4.Draw(rc);

	//swprintf_s(dbg, 256, L"UI X = %.2f", uiPos.x);
	//m_fontDebug5.SetText(dbg);
	//m_fontDebug5.SetPosition(300.0f, 340.0f, 0.0f);
	//m_fontDebug5.SetColor(0, 0, 0, 1);
	//m_fontDebug5.Draw(rc);

}