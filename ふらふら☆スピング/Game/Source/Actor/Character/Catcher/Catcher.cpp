#include "stdafx.h"
#include "Catcher.h"

Catcher::Catcher()
{
}

Catcher::~Catcher()
{
}

bool Catcher::Start()
{
	//ƒ‚ƒfƒ‹‚Ì“Ç‚İ‚İ
	m_modelRender.Init("batter.cmo");
	m_modelRender.SetScale({ 0.01f,0.01f,0.01f });
	return true;
}

void Catcher::Update()
{

}

void Catcher::Render(RenderContext& rc)
{
	//ƒ‚ƒfƒ‹‚Ì•`‰æ
	m_modelRender.Draw(rc);
}