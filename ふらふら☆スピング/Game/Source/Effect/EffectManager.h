#pragma once
#include "graphics/effect/EffectEmitter.h"
#include <map>
#include <string>

//エフェクトの種類。
//エフェクトの種類を追加する場合は、enEffect_Numの前に追加してください。
enum EffectType {
	enEffect_DownArrow,
	enEffect_HitBat,
	enEffect_Num
};

class EffectManager : public IGameObject
{
public:
	EffectManager();
	~EffectManager();

	void Update() override;

	uint32_t SetEffect(
		EffectType type=enEffect_DownArrow,
		const Vector3& pos=Vector3::Zero,
		const Vector3& scale=Vector3::One,
		const Quaternion& rot=Quaternion::Identity);

	uint32_t MoveEffect(
		uint32_t id=0,
		const Vector3& pos=Vector3::Zero,
		const Vector3& scale = Vector3::One,
		const Quaternion& rot = Quaternion::Identity);

	// 停止
	void StopEffect(uint32_t id);

	//全てのエフェクトを停止。
	void OrllStopEffect()
	{
		for (auto& effect : m_effects) {
			if (effect.second != nullptr) {
				effect.second->Stop();
			}
		}
	}

	//再生中かどうかを取得。
	bool GetIsPlayeEffect(uint32_t id)
	{
		auto it = m_effects.find(id);
		if (it == m_effects.end() || it->second == nullptr) return false;
		return it->second->IsPlay();
	}

	//エフェクトの参照を取得。
	//エフェクトの参照を取得します。エフェクトの参照がない場合はnullptrが返ります。
	EffectEmitter* GetEffectEmitter(uint32_t id)
	{
		auto it = m_effects.find(id);
		if (it == m_effects.end()) return nullptr;
		return it->second;
	}



private:
	const char16_t* m_filePath = u"Assets/effect/";
	const char16_t* m_ext = u".efk";

	//エフェクトのファイル名。エフェクトの種類と同じ順番で追加してください。
	//例）enEffect_DownArrowのファイル名はm_files[enEffect_DownArrow]に格納されている。
	const char16_t* m_files[enEffect_Num] = {
		u"DownArrow",
		u"HitBat"
	};

	std::unordered_map<uint32_t, EffectEmitter*> m_effects;					//effectへの参照。
	uint32_t m_nextId = 0;													//エフェクトのID。エフェクトを識別するために使用します。エフェクトを追加するたびにインクリメントされます。
};

//EffectManagerのグローバルインスタンス。
//EffectManagerはゲーム全体で1つあれば十分なので、グローバルインスタンスを用意しています。
//EffectManagerを使用する際は、g_effectManager->SetEffect()のようにしてください。
extern EffectManager* g_effectManager;