#include "stdafx.h"
#include "UIManager.h"

//UIマネージャーの更新処理
void UIManager::Update()
{		
	m_uianimationManager.Update(g_gameTime->GetFrameDeltaTime());
}

//UIの移動アニメーションの設定
void MoveAnimation::SetAnimation(Transform* transform, float duration, const Vector2& startPosition, const Vector2& endPosition)
{
	//UIの移動アニメーションの設定
	SetTransform(transform);
	SetDuration(duration);
	m_startPosition = startPosition;
	m_endPosition = endPosition;
}

//UIの拡大縮小アニメーションの設定
void ScaleAnimation::SetAnimation(Transform* transform, float duration, const Vector2& startScale, const Vector2& endScale)
{
	//UIの拡大縮小アニメーションの設定
	SetTransform(transform);
	SetDuration(duration);
	m_startScale = startScale;
	m_endScale = endScale;
}

//UIの回転アニメーションの設定
void RotateAnimation::SetAnimation(Transform* transform, float duration, const Quaternion& startRotation, const Quaternion& endRotation)
{
	//UIの回転アニメーションの設定
	SetTransform(transform);
	SetDuration(duration);
	m_startRotation = startRotation;
	m_endRotation = endRotation;
}

//UIアニメーションの更新処理
void UIAnimationManager::Update(float deltaTime)
{
	//UIアニメーションの更新処理
	for (auto& animation : m_animations) {
		animation->Animation(deltaTime);
	}

}

//UIの移動アニメーション処理
void MoveAnimation::Animation(float deltaTime)
{
	//UIの移動アニメーション処理
	SetElapsedTime(deltaTime);
	float t = GetProgress();
	GetTransform()->m_2Dposition.Lerp(t, m_startPosition,m_endPosition);

}

//UIの拡大縮小アニメーション処理
void ScaleAnimation::Animation(float deltaTime)
{
	//UIの拡大縮小アニメーション処理
	SetElapsedTime(deltaTime);
	float t = GetProgress();
	GetTransform()->m_2Dscale.Lerp(t, m_startScale, m_endScale);
}

//UIの回転アニメーション処理
void RotateAnimation::Animation(float deltaTime)
{
	//UIの回転アニメーション処理
	SetElapsedTime(deltaTime);
	float t = GetProgress();
	GetTransform()->m_rotation.Slerp(t, m_startRotation, m_endRotation);
}