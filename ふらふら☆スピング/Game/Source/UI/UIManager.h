#pragma once
#include "Source/Source.h"
#include <algorithm>

//UIマネージャークラス
class UIManager :public Source
{
public:
	UIManager() {}
	virtual ~UIManager() {}

	virtual void Update();
	


private:
	UIAnimationManager m_uianimationManager;
	MoveAnimation m_moveAnimation;
	ScaleAnimation m_scaleAnimation;
	RotateAnimation m_rotateAnimation;
};

//UIアニメーションの管理クラス
class UIAnimationManager
{
public:
	UIAnimationManager() {}
	~UIAnimationManager() {}

	void Update(float deltaTime);

	/**
	* *@brief UIアニメーションを追加する関数
	* *@param animation 追加するUIアニメーション
	* *@details
	* UIアニメーションを追加する関数。引数で渡されたUIアニメーションを管理クラスのコンテナに追加します。
	* UIアニメーションはstd::unique_ptrで管理されるため、所有権が移動します。追加されたUIアニメーションは、管理クラスのUpdate関数で更新されます。
	* UIアニメーションの追加は、UIの動きを制御するために使用されます。例えば、UIの移動、拡大縮小、回転などのアニメーションを追加することができます。
	*/
	void AddAnimation(std::unique_ptr<UIAnimation> animation)
	{
		m_animations.push_back(std::move(animation));
	}

private:
	
	//UIアニメーションクラスのコンテナ
	std::vector<std::unique_ptr<UIAnimation>> m_animations;
};

//UIアニメーションの基本的な処理
class UIAnimation
{
public:
	UIAnimation() {}
	~UIAnimation() {}

	//UIアニメーションの基本的な処理
	//入力された値の範囲を制限する関数
	//value: 制限したい値
	//min: 制限する最小値
	//max: 制限する最大値
	//返り値: 制限された値
	static float Clamp(float value, float min, float max)
	{
		if (value < min) return min; if (value > max)
			return max; return value;
	}	

	Transform* GetTransform() const { return m_transform; }
	void SetTransform(Transform* transform) { m_transform = transform; }
	void SetDuration(float duration) { m_duration = duration; }
	void SetElapsedTime(float time) { m_elapsedTime += time; }
	float GetDuration() const { return m_duration; }
	float GetProgress() const {
		return Clamp(m_elapsedTime / m_duration, 0.0f, 1.0f);
	}

	virtual void Animation(float deltaTime) = 0;	

private:
	Transform*		m_transform;	//アニメーションを適用するTransform
	float			m_duration;		//アニメーションの経過時間
	float			m_elapsedTime;	//アニメーションの経過時間
};

//UIの移動アニメーション処理
class MoveAnimation :public UIAnimation
{
	public:
	MoveAnimation() {}
	~MoveAnimation() {}
	
	void Animation(float deltaTime)override;
	void SetAnimation(Transform* transform, float duration, const Vector2& startPosition, const Vector2& endPosition);

private:
	Vector2		m_startPosition;
	Vector2		m_endPosition;
};

//UIの拡大縮小アニメーション処理
class ScaleAnimation :public UIAnimation
{
public:
	ScaleAnimation() {}
	~ScaleAnimation() {}

	void Animation(float deltaTime)override;
	void SetAnimation(Transform* transform, float duration, const Vector2& startScale, const Vector2& endScale);

private:
	Vector2		m_startScale;
	Vector2		m_endScale;
};

//UIの回転アニメーション処理
class RotateAnimation :public UIAnimation
{
	public:
	RotateAnimation() {}
	~RotateAnimation() {}

	void Animation(float deltaTime)override;
	void SetAnimation(Transform* transform, float duration, const Quaternion& startRotation, const Quaternion& endRotation);

private:
	Quaternion 	m_startRotation;
	Quaternion 	m_endRotation;
};


