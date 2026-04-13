#include "stdafx.h"
#include "EffectManager.h"

EffectManager* g_effectManager = nullptr;

EffectManager::EffectManager()
{
	for (int i = 0; i < enEffect_Num; i++)
	{
		std::string file =
			std::string(m_filePath) +
			m_files[i] +
			m_ext;

		std::u16string u16 = ToU16(file);

		EffectEngine::GetInstance()->ResistEffect(i, /*u16.c_str()*/u"Game/Assets/effect/DownArrow.efk");
	}
}

void EffectManager::SetEffect(
	EffectType type,
	const Vector3& pos,
	const Quaternion& rot,
	const Vector3& scale)
{
	auto effectEmitter = NewGO<EffectEmitter>(0);
	m_effectEmitter = effectEmitter;
	effectEmitter->Init(type);
	effectEmitter->SetPosition(pos);
	effectEmitter->SetRotation(rot);
	effectEmitter->SetScale(scale);
	effectEmitter->Play();
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

void EffectManager::StopEffect(int handle)
{
	EffectEngine::GetInstance()->Stop(handle);
}

std::u16string EffectManager::ToU16(const std::string& str)
{
	int size_needed =
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

	std::wstring wstr(size_needed, 0);

	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], size_needed);

	return std::u16string(wstr.begin(), wstr.end());
}