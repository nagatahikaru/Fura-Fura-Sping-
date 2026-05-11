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

		MessageBoxW(
			nullptr,
			(LPCWSTR)path.c_str(),
			L"EffectPath",
			MB_OK
		);

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
	for (auto& follow : m_followEffects)
	{
		auto& data = follow.second;


		if (data.emitter == nullptr ||
			data.targetTransform == nullptr)
		{
			continue;
		}

		data.emitter->SetPosition(
			data.targetTransform->m_position + data.offset);

		data.emitter->SetRotation(
			data.targetTransform->m_rotation * data.rotOffset);
	}

	//再生が終了したエフェクトは削除します。
	for (auto it = m_effects.begin(); it != m_effects.end(); )
	{
		//エフェクトの参照を取得。
		auto emitter = it->second;

		//エフェクトの再生が終了しているか判定。
		if (emitter == nullptr || !emitter->IsPlay())
		{
			if (emitter)
			{
				DeleteGO(emitter);
				it->second = nullptr;
			}
			m_followEffects.erase(it->first);
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
uint32_t EffectManager::PlayEffect(
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
bool EffectManager::MoveEffect(
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

/// <summary>
/// エフェクトの追従再生。
/// エフェクトのIDと座標、スケール、回転を指定してエフェクトを追従再生します。
/// </summary>
/// <param name="type"></param>
/// <param name="targetTransform"></param>
/// <param name="offset"></param>
/// <param name="scale"></param>
/// <param name="rot"></param>
/// <returns></returns>
uint32_t EffectManager::PlayFollowEffect(
	EffectType type,
	Transform* targetTransform,
	const Vector3& offset,
	const Vector3& scale,
	const Quaternion& rot)
{
	if (targetTransform == nullptr)
	{
		return 0;
	}

	auto effectEmitter = NewGO<EffectEmitter>(0);

	effectEmitter->Init(type);
	effectEmitter->SetPosition(
		targetTransform->m_position + offset);
	effectEmitter->SetScale(scale);
	effectEmitter->SetRotation(
		targetTransform->m_rotation * rot);

	effectEmitter->Play();

	uint32_t id = m_nextId++;

	m_effects[id] = effectEmitter;

	FollowData data;
	data.emitter = effectEmitter;
	data.targetTransform = targetTransform;
	data.offset = offset;
	data.rotOffset = rot;

	m_followEffects[id] = data;

	return id;
}

// <summary>
// エフェクトの停止。
// </summary>
void EffectManager::StopEffect(uint32_t id)
{
	auto it = m_effects.find(id);
	if (it == m_effects.end()){
		return;
	}

	if (it->second){
		it->second->Stop();
		DeleteGO(it->second);
	}

	m_followEffects.erase(id);
	m_effects.erase(it);
}