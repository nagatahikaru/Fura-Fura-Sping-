#include "stdafx.h"
#include "Transform.h"

Transform::Transform()
	:m_position(Vector3::Zero)
	, m_localPosition(Vector3::Zero)
	, m_scale(Vector3::One)
	, m_localScale(Vector3::One)
	, m_rotation(Quaternion::Identity)
	, m_localRotation(Quaternion::Identity)
	, m_rotationMatrix(Matrix::Identity)
	, m_worldMatrix(Matrix::Identity)
	, m_parent(nullptr)
{
	m_children.clear();
}

Transform::~Transform()
{
	if (m_parent) {
		m_parent->RemoveChild(this);
	}
	Release();
}

void Transform::UpdateTransform()
{

	if (m_parent) {
		//座標計算
		Matrix localPos;
		localPos.MakeTranslation(m_localPosition);

		Matrix pos;
		pos.Multiply(localPos, m_parent->m_worldMatrix);

		//多分平行移動の部分を取ってるだけ
		m_position.x = pos.m[3][0];
		m_position.y = pos.m[3][1];
		m_position.z = pos.m[3][2];

		//スケール
		m_scale.x = m_localScale.x * m_parent->m_scale.x;
		m_scale.y = m_localScale.y * m_parent->m_scale.y;
		m_scale.z = m_localScale.z * m_parent->m_scale.z;

		//回転
		m_rotation = m_parent->m_rotation * m_localRotation;

	}
	else
	{
		//ローカルの値をそのままコピー
		m_position = m_localPosition;
		m_scale = m_localScale;
		m_rotation = m_localRotation;
	}

	//回転行列
	m_rotationMatrix.MakeRotationFromQuaternion(m_rotation);
	//ワールド行列更新
	UpdateWorldMatrix();
}

void Transform::UpdateWorldMatrix()
{
	Matrix scal, pos, world;
	scal.MakeScaling(m_scale);
	pos.MakeTranslation(m_position);

	world.Multiply(scal, m_rotationMatrix);
	m_worldMatrix.Multiply(world, pos);

	//子も更新
	for (Transform* child : m_children)
	{
		child->UpdateTransform();
	}
}


void Transform::Release()
{
	//イテレータ生成
	std::vector<Transform*>::iterator it = m_children.begin();
	//vectorの終わりまで回す
	while (it != m_children.end())
	{
		//子トランスフォームからの紐づけを外す
		(*it)->m_parent = nullptr;
		//子トランスフォームへの紐づけを外す
		m_children.erase(it);
		// 対象がいないなら抜ける
		if (m_children.size() == 0) {
			break;
		}
		//イテレータを進める
		++it;

	}
	//念のため？vectorの要素を全削除
	m_children.clear();
}

void Transform::RemoveChild(Transform* t)
{
	//イテレータ生成
	std::vector<Transform*>::iterator it = m_children.begin();
	//vectorを回す
	while (it != m_children.end())
	{
		//イテレータから子トランスフォームのポインタを受け取る
		Transform* child = (*it);
		//受け取った子トランスフォームがしてされた物なら
		if (child == t)
		{
			//紐づけを外す
			child->m_parent = nullptr;
			m_children.erase(it);
			//処理を終了
			return;
		}

		//イテレータを進める
		++it;
	}
}


void Transform::ClearChild()
{
	m_children.clear();
}