#include "stdafx.h"
#include "Ball.h"
#include <stdlib.h>

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
	m_modelRender.SetScale({ 50.0f,50.0f,50.0f });

	m_position = { -200.0f, 200.0f, 1000.0f };
	m_modelRender.SetPosition(m_position);

	return true;
}

void Ball::Update()
{
	//float dt = 1.0f/60.0f;

	//if (m_isMove)
	//{
	//	m_position +=  m_velocity * dt;

	//	if (m_position.y < 0.0f)
	//	{
	//		m_position.y = 0.0f;
	//		m_isMove = false;
	//	}

	//	//モデルの位置更新
	//	m_modelRender.SetPosition(m_position);

	//}

	m_modelRender.Update();
}

void Ball::SetPosition(const Vector3& pos)
{
	m_position = pos;
	m_modelRender.SetPosition(m_position);
}

void Ball::Throw(Vector3 targetPos)
{
	m_targetPos = targetPos;

	Vector3 dir = m_targetPos - m_position;
	dir.Normalize();

	float speed = 25.0f + (float)(rand() % 11);

	m_velocity = dir * speed;

	m_isMove = true;

}

void Ball::Render(RenderContext& rc)
{
	//モデルの描画
		m_modelRender.Draw(rc);
	
}