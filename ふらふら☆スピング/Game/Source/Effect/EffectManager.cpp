#include "stdafx.h"
#include "EffectManager.h"

EffectManager* g_effectManager = nullptr;

EffectManager::EffectManager()
{
	//エフェクトの登録。
	for (int i = 0; i < enEffect_Num; i++)
	{
		std::u16string path =
			std::u16string(m_filePath) + m_files[i] + m_ext;

		EffectEngine::GetInstance()->ResistEffect(i, path.c_str());
	}
}

/// <summary>
/// エフェクトの再生。
/// </summary>
/// <param name="type">再生したいエフェクトの種類。</param>
/// <param name="pos">エフェクトの位置。</param>
/// <param name="rot">エフェクトの回転。</param>
/// <param name="scale">エフェクトのスケール。</param>
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

// <summary>
// エフェクトの停止。
// </summary>
void EffectManager::StopEffect()
{
	if (m_effectEmitter == nullptr)return;
	m_effectEmitter->Stop();
}