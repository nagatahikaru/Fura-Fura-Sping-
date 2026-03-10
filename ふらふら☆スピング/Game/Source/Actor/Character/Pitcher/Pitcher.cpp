#include "stdafx.h"
#include "Pitcher.h"

Pitcher::Pitcher()
{
}

Pitcher::~Pitcher()
{
}

bool Pitcher::Start()
{
	//ƒ‚ƒfƒ‹‚Ì“Ç‚İ‚İ
	m_modelRender.Init("batter.cmo");
	m_modelRender.SetScale({ 0.01f,0.01f,0.01f });
	return true;
}

void Pitcher::Update()
{

}

void Pitcher::Render(RenderContext& rc)
{
	//ƒ‚ƒfƒ‹‚Ì•`‰æ
	m_modelRender.Draw(rc);
}