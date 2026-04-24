#pragma once
#include "graphics/effect/EffectEmitter.h"
#include <map>
#include <string>

//エフェクトの種類。
//エフェクトの種類を追加する場合は、enEffect_Numの前に追加してください。
enum EffectType {
	enEffect_DownArrow,
	enEffect_Num
};

class EffectManager : public IGameObject
{
public:
	EffectManager();
	~EffectManager() = default;

	void SetEffect(
		EffectType type,
		const Vector3& pos,
		const Quaternion& rot,
		const Vector3& scale);

	// 停止
	void StopEffect();

	//再生中かどうかを取得。
	bool GetIsPlayeEffect()
	{
		if (!GetEffectEmitter())return false;
		return m_effectEmitter->IsPlay(); 
	}

	//エフェクトの参照を取得。
	//エフェクトの参照を取得します。エフェクトの参照がない場合はnullptrが返ります。
	EffectEmitter* GetEffectEmitter()
	{
		return m_effectEmitter;
	}



private:
	const char16_t* m_filePath = u"Assets/effect/";
	const char16_t* m_ext = u".efk";

	//エフェクトのファイル名。エフェクトの種類と同じ順番で追加してください。
	//例）enEffect_DownArrowのファイル名はm_files[enEffect_DownArrow]に格納されている。
	const char16_t* m_files[enEffect_Num] = {
		u"DownArrow"
	};

	EffectEmitter* m_effectEmitter = nullptr;					//effectへの参照。
};

//EffectManagerのグローバルインスタンス。
//EffectManagerはゲーム全体で1つあれば十分なので、グローバルインスタンスを用意しています。
//EffectManagerを使用する際は、g_effectManager->SetEffect()のようにしてください。
extern EffectManager* g_effectManager;