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
    if (m_collisionObject) {
        delete m_collisionObject;
        m_collisionObject = nullptr;
    }
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

	m_collisionObject = new CollisionObject();
	m_collisionObject->CreateSphere(m_position, Quaternion::Identity, 25.0f);
	m_collisionObject->Update();

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

        SetPosition(m_position);
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
	m_collisionObject->SetPosition(m_position);
    m_collisionObject->Update();
}

bool Ball::IsMoving() const
{
	return m_isMove;
}

// ボールと指定した位置・半径の球との衝突判定
// ボールの中心と指定した位置との距離が、両方の半径の和より小さい場合に衝突とみなす
// 例: ボールの半径が10、指定した半径が5の場合、両方の半径の和は15
// ボールの中心と指定した位置との距離が15未満なら衝突と判定
// つまり、ボールの中心が指定した位置から15未満の距離にある場合に衝突とみなす
bool Ball::CheckCollision(const Vector3& pos, float radius)
{
	float dist = (m_position - pos).Length();
	return dist < (m_radius + radius);
}

void Ball::HitBall(const Vector3& hitDirection, float hitPower)
{
    Vector3 dir = hitDirection;
    dir.Normalize();
    m_velocity = dir * hitPower;
    m_isMove = true;
}

void Ball::Render(RenderContext& rc)
{
	//モデルの描画
		m_modelRender.Draw(rc);
	
}