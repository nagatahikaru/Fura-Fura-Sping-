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

EffectManager::~EffectManager()
{
	if (g_effectManager == this) {
		g_effectManager = nullptr;
	}
}

/// <summary>
/// エフェクトの更新。
/// </summary>
/// エフェクトの更新。再生が終了したエフェクトは削除します。
/// 再生が終了したエフェクトは削除します。エフェクトの削除はDeleteGOを使用してください。
/// DeleteGOを使用しないと、エフェクトの参照が残ってしまい、
/// エフェクトの再生が終了しているのにエフェクトの参照が残っている状態になってしまいます。
void EffectManager::Update()
{
	//再生が終了したエフェクトは削除します。
	for (auto it = m_effects.begin(); it != m_effects.end(); )
	{
		//エフェクトの参照を取得。
		auto emitter = it->second;

		//エフェクトの再生が終了しているか判定。
		if (emitter == nullptr || !emitter->IsPlay())
		{
			//エフェクトの参照が残っている場合は削除。
			if (emitter)
			{
				DeleteGO(emitter); // ← これ重要
				it->second = nullptr; // ← これも重要
			}
			it = m_effects.erase(it);
		}
		//再生が終了していない場合は次のエフェクトへ。
		else
		{
			++it;
		}
	}
}

/// <summary>
/// エフェクトの再生。
/// </summary>
/// <param name="type">再生したいエフェクトの種類。</param>
/// <param name="pos">エフェクトの位置。</param>
/// <param name="rot">エフェクトの回転。</param>
/// <param name="scale">エフェクトのスケール。</param>
uint32_t EffectManager::SetEffect(
	EffectType type,
	const Vector3& pos,
	const Vector3& scale,
	const Quaternion& rot)
{
	auto effectEmitter = NewGO<EffectEmitter>(0);	
	effectEmitter->Init(type);
	effectEmitter->SetPosition(pos);
	effectEmitter->SetRotation(rot);
	effectEmitter->SetScale(scale);
	effectEmitter->Play();
	uint32_t id = m_nextId++;
	m_effects[id] = effectEmitter;

	return id;
}

/// <summary>
/// エフェクトの移動。
/// </summary>
/// <param name="id">移動したいエフェクトのID。</param>
/// <param name="pos">エフェクトの位置。</param>
/// <param name="rot">エフェクトの回転。</param>
/// <param name="scale">エフェクトのスケール。</param>
/// <returns>移動に成功したらtrue。エフェクトのIDが存在しない場合はfalse。</returns>
uint32_t EffectManager::MoveEffect(
	uint32_t id,
	const Vector3& pos,	
	const Vector3& scale,
	const Quaternion& rot)
{
	//エフェクトの参照を取得。
	auto it = m_effects.find(id);
	//エフェクトのIDが存在しない場合はfalseを返す。
	if (it == m_effects.end()) return false;
	//エフェクトの参照が存在しない場合はfalseを返す。
	auto emitter = it->second;
	if (emitter == nullptr) return false;
	emitter->SetPosition(pos);
	emitter->SetRotation(rot);
	emitter->SetScale(scale);
	return true;
}

// <summary>
// エフェクトの停止。
// </summary>
void EffectManager::StopEffect(uint32_t id)
{
	auto it = m_effects.find(id);
	if (it == m_effects.end()) return;

	it->second->Stop();
	DeleteGO(it->second);
	m_effects.erase(it);
}