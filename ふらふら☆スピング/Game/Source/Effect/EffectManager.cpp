#include "stdafx.h"
#include "EffectManager.h"
EffectManager* g_effectManager = nullptr;

namespace {
	//ファイル名のみ追加すれば、
	//コンストラクタにて自動でg_effectEngineに登録されます。
	//例："Assets/effect/"+"Title"+".wav"
	const char* EFFECT_FILEPATH = "Assets/effect/";
	const char* EFFECT_EXTENSION = ".efkefc";
	const char* effectFileNameList[enEffect_Num] =
	{
		//ここにファイル名を追加してください。
		//追加する際は、ヘッダのenum EffectTypeにも追加してください。
		"Explosion",
	};
}

EffectManager::EffectManager()
{
	// ★ エフェクトエンジンのインスタンスを作成
	nsK2EngineLow::EffectEngine::CreateInstance();

	// EffectEngineのインスタンス取得
	auto* effectEngine = nsK2EngineLow::EffectEngine::GetInstance();
	for (int i = 0; i < enEffect_Num; i++)
	{
		std::string file = std::string(EFFECT_FILEPATH) + effectFileNameList[i] + (EFFECT_EXTENSION);
		// std::string → std::u16string へ変換
		std::u16string u16file(file.begin(), file.end());
		const char16_t* filepath = u16file.c_str();
		effectEngine->ResistEffect(i, filepath);
	}
}

// エフェクトを再生する関数
// 引数でエフェクトの種類、位置、回転、スケールを指定して、エフェクトを再生します。
// エフェクトの再生にはEffectEngineを使用し、EffectEmitterのインスタンスを生成して返します。
// 返り値はEffectEmitterのインスタンスで、エフェクトの制御に使用できます。
// 例えば、エフェクトの再生後に位置や回転を変更したい場合は、返り値のEffectEmitterを使用して制御できます。
// 例:
// EffectEmitter* emitter = effectManager->PlayEffect(enEffect_Explosion, Vector3(0, 0, 0), Quaternion::Identity, Vector3(1, 1, 1));
// emitter->SetPosition(Vector3(1, 0, 0)); // エフェクトの位置を変更
// emitter->SetRotation(Quaternion::Identity); // エフェクトの回転を変更
// emitter->SetScale(Vector3(2, 2, 2)); // エフェクトのスケールを変更
// 注意: エフェクトの再生後は、返り値のEffectEmitterを適切に管理してください。不要になった場合は、ゲームオブジェクトマネージャーなどを使用して削除することを検討してください。
// 注意: エフェクトの再生には、EffectEngineのインスタンスが必要です。EffectManagerのコンストラクタでEffectEngineのインスタンスを作成しているため、EffectManagerのインスタンスを作成する前にEffectEngineのインスタンスが存在することを確認してください。
EffectEmitter* EffectManager::PlayEffect(EffectType number, Vector3 pos, Quaternion rot, Vector3 scale)
{
	auto* emitter = NewGO<EffectEmitter>(0);

	emitter->Init(number);
	emitter->SetPosition(pos);
	emitter->SetRotation(rot);
	emitter->SetScale(scale);
	emitter->Play();

	return emitter;
}