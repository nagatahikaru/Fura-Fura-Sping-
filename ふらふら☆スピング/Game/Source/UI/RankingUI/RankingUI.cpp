#include "stdafx.h"
#include"RankingUI.h"

bool RankingUI::Start() {
	m_1.Init("Assets/sprite/1st.DDS", 150.0f,150.0f);
	m_2.Init("Assets/sprite/2nd.DDS", 150.0f, 150.0f);
	m_3.Init("Assets/sprite/3sd.DDS", 150.0f, 150.0f);
	m_4.Init("Assets/sprite/4.DDS", 150.0f, 150.0f);
	m_5.Init("Assets/sprite/5.DDS",150.0f, 150.0f);
	m_11.Init("Assets/sprite/1st.DDS", 150.0f, 150.0f);
	m_22.Init("Assets/sprite/2nd.DDS", 150.0f, 150.0f);
	m_33.Init("Assets/sprite/3sd.DDS", 150.0f, 150.0f);
	m_44.Init("Assets/sprite/4.DDS", 150.0f, 150.0f);
	m_55.Init("Assets/sprite/5.DDS", 150.0f, 150.0f);
	m_B.Init("Assets/sprite/Bback.dds", 220.0f, 170.0f);
	m_B.SetPosition({ 830.0f, -400.0f, 0.0f });

	m_grobu.Init("Assets/sprite/guro-bu.dds", 450.0f, 430.0f);
	m_grobu.SetPosition({ 830.0f, -400.0f, 0.0f });
	return true;
}
void RankingUI::Update() {

}
void RankingUI::Render(RenderContext& rc) {
	m_1.SetPosition({ -520,70,0 });
	m_1.Update();
	m_1.Draw(rc);
	m_2.SetPosition({ -520,-20,0 });
	m_2.Update();
	m_2.Draw(rc);
	m_3.SetPosition({ -520,-110,0 });
	m_3.Update();
	m_3.Draw(rc);
	m_4.SetPosition({ -520,-200,0 });
	m_4.Update();
	m_4.Draw(rc);
	m_5.SetPosition({ -520,-290,0 });
	m_5.Update();
	m_5.Draw(rc);
	m_11.SetPosition({ 180,70,0 });
	m_11.Update();
	m_11.Draw(rc);
	m_22.SetPosition({ 180,-20,0 });
	m_22.Update();
	m_22.Draw(rc);
	m_33.SetPosition({ 180,-110,0 });
	m_33.Update();
	m_33.Draw(rc);
	m_44.SetPosition({ 180,-200,0 });
	m_44.Update();
	m_44.Draw(rc);
	m_55.SetPosition({ 180,-290,0 });
	m_55.Update();
	m_55.Draw(rc);
	m_grobu.Update();
	m_grobu.Draw(rc);
	m_B.Update();
	m_B.Draw(rc);
}