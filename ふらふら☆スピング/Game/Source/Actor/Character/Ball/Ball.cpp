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
	//ƒ‚ƒfƒ‹‚Ì“Ç‚İ‚İ
	m_modelRender.Init("batter.cmo");
	m_modelRender.SetScale({ 0.01f,0.01f,0.01f });
	return true;
}

void Ball::Update()
{

}

void Ball::Render(RenderContext& rc)
{
	//ƒ‚ƒfƒ‹‚Ì•`‰æ
	m_modelRender.Draw(rc);
}