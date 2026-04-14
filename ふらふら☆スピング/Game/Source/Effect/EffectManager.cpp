#include "stdafx.h"
#include "EffectManager.h"

EffectManager* g_effectManager = nullptr;

EffectManager::EffectManager()
{
	for (int i = 0; i < enEffect_Num; i++)
	{
		std::u16string path =
			std::u16string(m_filePath) + m_files[i] + m_ext;

		EffectEngine::GetInstance()->ResistEffect(i, path.c_str());
	}
}

void EffectManager::SetEffect(
	EffectType type,
	const Vector3& pos,
	const Quaternion& rot,
	const Vector3& scale)
{
	auto effectEmitter = NewGO<EffectEmitter>(0);	
	effectEmitter->Init(type);
	effectEmitter->SetPosition(pos);
	effectEmitter->SetRotation(rot);
	effectEmitter->SetScale(scale);
	effectEmitter->Play();
	m_effectEmitter = effectEmitter;
}

//void EffectManager::PlayEffect(
//	EffectType type,
//	const Vector3& pos,
//	const Quaternion& rot,
//	const Vector3& scale)
//{
//	m_effectEmitter->Init(type);
//	m_effectEmitter->SetPosition(pos);
//	m_effectEmitter->SetRotation(rot);
//	m_effectEmitter->SetScale(scale);
//	m_effectEmitter->Play();		
//}

void EffectManager::StopEffect()
{
	if (m_effectEmitter == nullptr)return;
	m_effectEmitter->Stop();
}