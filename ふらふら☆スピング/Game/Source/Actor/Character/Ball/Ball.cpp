#include "stdafx.h"
#include "Ball.h"
#include <stdlib.h>
#include"Source/Scene/InGame/Game.h"
#include"Source/UI/InGameUI/InGameUI.h"

Ball::Ball()
{
}

Ball::~Ball()
{
}

bool Ball::Start()
{
	//モデルの読み込み
	m_modelRender.Init("Assets/modelData/Ball/Ball.tkm");
	m_modelRender.SetScale({ 3.5f,3.5f,3.5f });

	m_position = { -0.0f, 750.0f, 1200.0f };
	m_modelRender.SetPosition(m_position);

    
    // ★ UI に初期位置を送る（これが重要）
    Game* game = FindGO<Game>("game");
    if (game) {
        InGameUI* ui = game->GetInGameUI();
        if (ui) {
            ui->SetPredictedBallPos(m_position);
        }
    }

	//タイマー初期化
	m_throwTimer = 0.0f;

	return true;
}

void Ball::Update()
{
    if (m_isPaused) return;

    Game* game = FindGO<Game>("game");
    if (game && game->m_isPaused) return;

    float dt = 1.0f / 60.0f;

    m_throwTimer += dt;

    if (m_throwTimer >= 2.0f && !m_isMove)
    {
        Throw({ 0.0f, 0.0f, 0.0f });
        m_throwTimer = 0.0f;
    }

    if (m_isMove)
    {
        m_velocity.y -= 31.0f * dt;
        m_position += m_velocity * dt;

        // ★ Z>=5500 の瞬間に 1 回だけ固定する
        if (!m_hasFixed && m_position.z >= 5500.0f) {
            if (game) {
                InGameUI* ui = game->GetInGameUI();
                if (ui) {
                    ui->FixBallUI(m_position);
                }
            }
            m_hasFixed = true;   // ← これが重要
        }

        // ★ 地面に着いたら止める
        if (m_position.y < 0.0f)
        {
            m_position.y = 0.0f;
            m_isMove = false;
        }

        m_modelRender.SetPosition(m_position);
    }

    // ★ UI に毎フレーム位置を送る（必須）
    if (game) {
        InGameUI* ui = game->GetInGameUI();
        if (ui) {
            ui->SetPredictedBallPos(m_position);
        }
    }

    m_modelRender.Update();
}


void Ball::Throw(const Vector3& targetPos)
{
	Vector3 dir = { 0.0f,0.05f,3.0f };
	dir.Normalize();

	float speed = 1000.0f;

	m_velocity = dir * speed;

	m_isMove = true;

}

void Ball::SetPosition(const Vector3& pos)
{
	m_position = pos;
	m_modelRender.SetPosition(m_position);
}

bool Ball::IsMoving() const
{
	return m_isMove;
}

bool Ball::CheckCollision(const Vector3& pos, float radius)
{
	float dist = (m_position - pos).Length();
	return dist < (m_radius + radius);
}

void Ball::AnimationUpdate()
{
    // ボールはアニメーションが無いので空でOK
}

void Ball::Render(RenderContext& rc)
{
	//モデルの描画
		m_modelRender.Draw(rc);
	
}