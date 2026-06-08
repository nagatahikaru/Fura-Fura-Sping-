#pragma once
#include "graphics/effect/EffectEmitter.h"
#include <map>
#include <string>

//エフェクトの種類。
//エフェクトの種類を追加する場合は、enEffect_Numの前に追加してください。
enum EffectType {
	enEffect_DownArrow1,
	enEffect_DownArrow2,
	enEffect_DownArrow3,
	enEffect_DownArrow4,
	enEffect_DownArrow5,
	enEffect_DownArrow6,
	enEffect_DownArrow7,
	enEffect_DownArrow8,
	enEffect_DownArrow9,
	enEffect_DownArrow10,
	enEffect_HitBat,
	enEffect_kemuri,
	enEffect_Num
};

class EffectManager : public IGameObject
{
public:
	EffectManager();
	~EffectManager();

	void Update() override;

	/// 再生関数 ///

	/// <summary>
	/// エフェクトの再生。
	/// エフェクトの種類と座標、スケール、回転を指定してエフェクトを再生します。
	/// </summary>
	/// <param name="type">再生したいエフェクトの種類。</param>
	/// <param name="pos">エフェクトの位置。</param>
	/// <param name="scale">エフェクトのスケール。</param>
	/// <param name="rot">エフェクトの回転。</param>
	/// <returns></returns>
	uint32_t PlayEffect(
		EffectType type= enEffect_DownArrow1,
		const Vector3& pos=Vector3::Zero,
		const Vector3& scale=Vector3::One,
		const Quaternion& rot=Quaternion::Identity);

	/// 移動関数 ///

	/// <summary>
	/// エフェクトの移動。
	/// エフェクトのIDと座標、スケール、回転を指定してエフェクトを移動します。
	/// </summary>
	/// <param name="id">移動させたいエフェクトのID。</param>
	/// <param name="pos">エフェクトの新しい位置。</param>
	/// <param name="scale">エフェクトの新しいスケール。</param>
	/// <param name="rot">エフェクトの新しい回転。</param>
	/// <returns></returns>
	bool MoveEffect(
		uint32_t id=0,
		const Vector3& pos=Vector3::Zero,
		const Vector3& scale = Vector3::One,
		const Quaternion& rot = Quaternion::Identity);

	/// <summary>
	/// エフェクトの移動。
	/// エフェクトのタイプと座標、スケール、回転を指定してエフェクトを移動します。
	/// </summary>
	/// <param name="type">移動させたいエフェクトのタイプ。</param>
	/// <param name="targetTransform">追従させたいターゲットのTransform。</param>
	/// <param name="offset">ターゲットからのオフセット。</param>
	/// <returns></returns>
	uint32_t PlayFollowEffect(
		EffectType type,
		Transform* targetTransform,
		const Vector3& offset = Vector3::Zero,
		const Vector3& scale = Vector3::One,
		const Quaternion& rot = Quaternion::Identity);

	/// 停止関数 ///

	// 停止
	void StopEffect(uint32_t id);

	//全てのエフェクトを停止。
	void AllStopEffect()
	{
		for (auto& effect : m_effects)
		{
			if (effect.second)
			{
				effect.second->Stop();
				DeleteGO(effect.second);
			}
		}
		m_effects.clear();
		m_followEffects.clear();
	}

	//追従停止
	void RemoveFollow(uint32_t id)
	{
		m_followEffects.erase(id);
	}

	/// 判定取得関数 ///

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
	const char16_t* m_filePath = u"Assets/effect/efk/";
	const char16_t* m_ext = u".efk";

	//エフェクトのファイル名。エフェクトの種類と同じ順番で追加してください。
	//例）enEffect_DownArrowのファイル名はm_files[enEffect_DownArrow]に格納されている。
	const char16_t* m_files[enEffect_Num] = {
		u"DownArrow1",
		u"DownArrow2",
		u"DownArrow3",
		u"DownArrow4",
		u"DownArrow5",
		u"DownArrow6",
		u"DownArrow7",
		u"DownArrow8",
		u"DownArrow9",
		u"DownArrow10",
		u"HitBat",
		u"kemuri"
	};

	std::unordered_map<uint32_t, EffectEmitter*> m_effects;					//effectへの参照。
	uint32_t m_nextId = 0;													//エフェクトのID。エフェクトを識別するために使用します。エフェクトを追加するたびにインクリメントされます。
	struct FollowData
	{
		EffectEmitter* emitter = nullptr;
		Transform* targetTransform = nullptr;
		Vector3 offset = Vector3::Zero;
		Quaternion rotOffset = Quaternion::Identity;
	};

	std::unordered_map<uint32_t, FollowData> m_followEffects;
};

//EffectManagerのグローバルインスタンス。
//EffectManagerはゲーム全体で1つあれば十分なので、グローバルインスタンスを用意しています。
//EffectManagerを使用する際は、g_effectManager->SetEffect()のようにしてください。
extern EffectManager* g_effectManager;