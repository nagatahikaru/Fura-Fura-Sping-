#include "stdafx.h"
#include "Ball.h"

Ball::Ball()
{
}

Ball::~Ball()
{
}

bool Ball::Start()
{
	//モデルの読み込み
	m_modelRender.Init("batter.cmo");
	m_modelRender.SetScale({ 0.01f,0.01f,0.01f });

	m_position = { 0.0f,1.5f,0.0f };

	return true;
}

void Ball::Update()
{
	float dt = 1.0f / 60.0f;

	if (m_isMove)
	{
		m_position += m_velocity * dt;

		//モデルの位置更新
		m_modelRender.SetPosition(m_position);
	}
}

void Ball::Throw(Vector3 targetPos)
{
	m_targetPos = targetPos;

	Vector3 dir = m_targetPos - m_position;

	dir.Normalize();

	float speed = 20.0f;

	m_velocity = dir * speed;

	m_isMove = true;

}

void Ball::Render(RenderContext& rc)
{
	//モデルの描画
	m_modelRender.Draw(rc);
}