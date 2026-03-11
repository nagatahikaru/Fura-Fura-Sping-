#include "stdafx.h"
#include "Titer.h"


bool Titer::Start()
{
	m_spriteRender.Init("Assets/sprite/Titer.DDS", 1920.0f, 1080.0f);
	//m_uiManager->AddAnimation(std::make_unique<MoveAnimation>(m_transform, 1.0f, Vector2(1.0f, 1.0f), Vector2(5.0f, 5.0f)));

	return true;
}

void Titer::Update()
{
	
	
}

void Titer::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
}