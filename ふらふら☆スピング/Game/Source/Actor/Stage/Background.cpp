#include "stdafx.h"
#include "Background.h"

Background::Background()
{
}

Background::~Background()
{
}

bool Background::Start()
{
	m_modelRender.Init("Assets/stage/baseballField.tkm");
	return true;
}

void Background::Update()
{
}

void Background::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
