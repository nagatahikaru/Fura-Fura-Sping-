#include "stdafx.h"
#include "Start.h"
#include"Source/Actor/Character/Batter/Batter.h"
#include"Source/Actor/Character/Pitcher/Pitcher.h"
#include"Source/Actor/Character/Catcher/Catcher.h"
#include"Source/Actor/Character/Ball/Ball.h"
#include"Source/Sound/SoundManager.h"

bool Start1::Start()
{
    // ★ カウントダウン開始 → ゲーム全体を停止
    Game* game = FindGO<Game>("game");
    if (game) {
        game->m_isPaused = true;
    }

	m_3.Init("Assets/sprite/3.dds", 400.0f, 300.0f);
	m_3.SetPosition({ 0.0f, 0.0f, 0.0f });
	m_2.Init("Assets/sprite/2.dds", 400.0f, 300.0f);
	m_2.SetPosition({ 0.0f, 0.0f, 0.0f });
	m_1.Init("Assets/sprite/1.dds", 400.0f, 300.0f);
	m_1.SetPosition({ 0.0f, 0.0f, 0.0f });
	m_Start.Init("Assets/sprite/start!.dds", 600.0f, 500.0f);
	m_Start.SetPosition({ 0.0f, 0.0f, 0.0f });
	return true;
}

void Start1::Update()
{
    m_timer += g_gameTime->GetFrameDeltaTime();

    if (m_timer >= 4.0f) {

        // ★ カウントダウン終了 → ゲーム再開
        Game* game = FindGO<Game>("game");
        if (game) {
            game->m_isPaused = false;
        }

        DeleteGO(this);
    }

    // 0〜1秒 → 3
    if (m_timer < 1.0f) {
        m_3.SetMulColor({ 1,1,1,1 });
        m_2.SetMulColor({ 1,1,1,0 });
        m_1.SetMulColor({ 1,1,1,0 });
        m_Start.SetMulColor({ 1,1,1,0 });
    }
    // 1〜2秒 → 2
    else if (m_timer < 2.0f) {
        m_3.SetMulColor({ 1,1,1,0 });
        m_2.SetMulColor({ 1,1,1,1 });
        m_1.SetMulColor({ 1,1,1,0 });
        m_Start.SetMulColor({ 1,1,1,0 });
    }
    // 2〜3秒 → 1
    else if (m_timer < 3.0f) {
        m_3.SetMulColor({ 1,1,1,0 });
        m_2.SetMulColor({ 1,1,1,0 });
        m_1.SetMulColor({ 1,1,1,1 });
        m_Start.SetMulColor({ 1,1,1,0 });
    }
    // 3〜4秒 → START!!
    else if (m_timer < 4.0f) {
        // ★ START!! に入った瞬間だけ SE 再生
        if (!m_playedStartSE) {
            if (g_soundManager) {
                g_soundManager->PlaySE(Sound::enSound_SE, 100.0f);
            }
            m_playedStartSE = true;
        }
        m_3.SetMulColor({ 1,1,1,0 });
        m_2.SetMulColor({ 1,1,1,0 });
        m_1.SetMulColor({ 1,1,1,0 });
        m_Start.SetMulColor({ 1,1,1,1 });
    }
    // 4秒後 → 自動削除
    else {
        // ★ カウントダウン終了 → キャラ動作開始
        auto batter = FindGO<Batter>("batter");
        auto pitcher = FindGO<Pitcher>("pitcher");
        auto ball = FindGO<Ball>("ball");

        if (batter) batter->m_isPaused = false;
        if (pitcher) pitcher->m_isPaused = false;
        if (ball) ball->m_isPaused = false;

        DeleteGO(this);
    }

}

void Start1::Render(RenderContext& rc)
{
	m_3.Update();
	m_3.Draw(rc);
	m_2.Update();
	m_2.Draw(rc);
	m_1.Update();
	m_1.Draw(rc);
	m_Start.Update();
	m_Start.Draw(rc);
}