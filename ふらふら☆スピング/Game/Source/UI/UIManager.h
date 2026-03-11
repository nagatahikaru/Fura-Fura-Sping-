#pragma once
#include "Source/Source.h"
#include <vector>
#include <memory>
#include <utility>
#include <algorithm>

////UIマネージャークラス
//class UIManager :public Source
//{
//public:
//	UIManager() = default;
//	virtual ~UIManager() = default;
//
//	virtual void Update();
//
//private:	
//	UIAnimationManager animManager;
//};
//
////UIアニメーションの基本的な処理
//class UIAnimation
//{
//public:
//	UIAnimation() = default;
//	virtual ~UIAnimation() = default;
//
//	void Update(float deltaTime)
//	{
//		if (IsFinished()) return;
//
//		if (!m_transform) return;
//
//		if (m_delayTimer < m_delay)
//		{
//			m_delayTimer += deltaTime;
//			return;
//		}
//
//		m_elapsedTime += deltaTime;
//		Animation(deltaTime);
//	}
//
//	bool IsFinished() const
//	{
//		return GetProgress() >= 1.0f;
//	}
//
//	/**
//	* UIアニメーションの基本的な処理
//	* 入力された値の範囲を制限する関数
//	* value: 制限したい値
//	* min: 制限する最小値
//	* max: 制限する最大値
//	* 返り値: 制限された値
//	*/
//	static float Clamp(float value, float min, float max)
//	{
//		if (value < min) return min; if (value > max)
//			return max; return value;
//	}	
//
//	Transform* GetTransform() const { return m_transform; }
//	void SetTransform(Transform* transform) { m_transform = transform; }
//	void SetDuration(float duration) { m_duration = std::max<float>(duration, 0.0001f); }
//	float GetDuration() const { return m_duration; }
//	float GetProgress() const {
//		if(m_duration <= 0.0f) {
//			return 1.0f; // durationが0以下の場合はアニメーションが完了しているとみなす
//		}
//		return Clamp(m_elapsedTime / m_duration, 0.0f, 1.0f);
//	}
//
//	virtual void Animation(float progress) = 0;
//
//private:
//	Transform*	m_transform = nullptr;	//アニメーション対象のTransform
//	float		m_duration = 0.0f;		//アニメーションの総時間
//	float		m_elapsedTime = 0.0f;	//アニメーションの経過時間
//	float		m_delay = 0.0f;			//アニメーション開始前の遅延時間
//	float		m_delayTimer = 0.0f;	//遅延時間の経過時間
//};
//
////UIの移動アニメーション処理
//class MoveAnimation :public UIAnimation
//{
//	public:
//	MoveAnimation() = default;
//	~MoveAnimation() = default;
//	
//	MoveAnimation(
//		Transform* transform,
//		float duration,
//		const Vector2& start,
//		const Vector2& end)
//	{
//		SetTransform(transform);
//		SetDuration(duration);
//		m_startPosition = start;
//		m_endPosition = end;
//	}
//
//	void Animation(float deltaTime) override
//	{
//		Transform* t = GetTransform();		
//
//		float p = GetProgress();
//		t->m_2Dposition.Lerp(p, m_startPosition, m_endPosition);
//	}
//
//private:
//	Vector2		m_startPosition;
//	Vector2		m_endPosition;
//};
//
////UIの拡大縮小アニメーション処理
//class ScaleAnimation : public UIAnimation
//{
//public:
//
//	ScaleAnimation(
//		Transform* transform,
//		float duration,
//		const Vector2& start,
//		const Vector2& end)
//	{
//		SetTransform(transform);
//		SetDuration(duration);
//		m_startScale = start;
//		m_endScale = end;
//	}
//
//	void Animation(float deltaTime) override
//	{
//		Transform* t = GetTransform();	
//
//		float p = GetProgress();
//		t->m_2Dscale.Lerp(p, m_startScale, m_endScale);
//	}
//
//private:
//
//	Vector2 m_startScale;
//	Vector2 m_endScale;
//};
//
////UIの回転アニメーション処理
//class RotateAnimation :public UIAnimation
//{
//	public:
//		RotateAnimation() = default;
//		~RotateAnimation() = default;
//
//	RotateAnimation(
//		Transform* transform,
//		float duration,
//		const Quaternion& start,
//		const Quaternion& end)
//	{
//		SetTransform(transform);
//		SetDuration(duration);
//		m_startRotation = start;
//		m_endRotation = end;
//	}
//
//	void Animation(float deltaTime) override
//	{
//		Transform* t = GetTransform();	
//
//		float p = GetProgress();
//		t->m_rotation.Slerp(p, m_startRotation, m_endRotation);
//	}
//
//private:
//	Quaternion 	m_startRotation;
//	Quaternion 	m_endRotation;
//};
//
//// UIアニメーションマネージャー
//class UIAnimationManager
//{
//public:
//
//	UIAnimationManager() = default;
//	~UIAnimationManager() = default;
//
//	// アニメーション更新
//	void Update(float deltaTime)
//	{
//		for (auto& anim : m_animations)
//		{
//			anim->Update(deltaTime);
//		}
//
//		// 終了したアニメーション削除
//		m_animations.erase(
//			std::remove_if(
//				m_animations.begin(),
//				m_animations.end(),
//				[](const std::unique_ptr<UIAnimation>& anim)
//				{
//					return anim->IsFinished();
//				}),
//			m_animations.end());
//	}
//
//	/**
//	 * @brief アニメーションを追加
//	 * @tparam T アニメーションクラス
//	 * @param args コンストラクタ引数
//	 */
//	template<class T, class... Args>
//	void AddAnimation(Args&&... args)
//	{
//		m_animations.push_back(
//			std::make_unique<T>(
//				std::forward<Args>(args)...
//			)
//		);
//	}
//
//private:
//
//	std::vector<std::unique_ptr<UIAnimation>> m_animations;
//};
