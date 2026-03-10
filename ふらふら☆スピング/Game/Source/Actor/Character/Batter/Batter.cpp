#include "stdafx.h"
#include "Batter.h"

Batter::Batter()
{
}

Batter::~Batter()
{
}

bool Batter::Start()
{
	//ƒ‚ƒfƒ‹‚Ì“Ç‚İ‚İ
	m_modelRender.Init("batter.cmo");
	m_modelRender.SetScale({ 0.01f,0.01f,0.01f });
	return true;
}

void Batter::Update()
{

}

void Batter::Render(RenderContext& rc)
{
	//ƒ‚ƒfƒ‹‚Ì•`‰æ
	m_modelRender.Draw(rc);
}