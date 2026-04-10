#pragma once

enum EffectType {
	enEffect_Explosion,
	enEffect_DownArrow,
	enEffect_Num
};

class EffectManager : public IGameObject
{
public:
	EffectManager();
	~EffectManager() {};

	/**
	* number: 再生するエフェクトの種類を指定します。EffectType列挙体の値を使用してください。
	* pos: エフェクトの再生位置を指定します。Vector3型で、x、y、zの座標を設定してください。
	* rot: エフェクトの回転を指定します。Quaternion型で、x、y、z、wの値を設定してください。
	* scale: エフェクトのスケールを指定します。Vector3型で、x、y、zのスケールを設定してください。
	*/
	EffectEmitter* PlayEffect(
		EffectType number,
		Vector3 pos,
		Quaternion rot,
		Vector3 scale
	);
};

extern EffectManager* g_effectManager;