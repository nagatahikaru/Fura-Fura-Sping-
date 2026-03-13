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
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
	return true;
}

void Background::Update()
{
}

void Background::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
