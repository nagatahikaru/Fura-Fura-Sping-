#pragma once

class Transform : public Noncopyable
{
	/** public変数 */
public:
	//自身のパラメータ
	Vector3 m_localPosition;
	Quaternion m_localRotation;
	Vector3 m_localScale;

	/** 親トランスフォームを考慮したパラメータ */
	Vector3 m_position;
	Quaternion m_rotation;
	Vector3 m_scale;
	Vector2 m_2Drotation;//2D用の回転角度
	Vector2 m_2Dposition;//2D用の位置
	Vector2 m_2Dscale;//2D用のスケール
	float m_speed = 0.0f;//移動速度
	Vector3 m_moveSpeed;//移動速度

	/** private変数 */
	/** 基本的に関数以外から弄る必要なし */
private:

	Matrix m_rotationMatrix;
	Matrix m_worldMatrix;

	Transform* m_parent;
	std::vector<Transform*> m_children;


	/** public関数 */
public:
	Transform();
	~Transform();

	/** 更新処理 */
	void UpdateTransform();
	/** ワールド行列更新、UpdateTransformの方で呼ばれるので呼び出す必要なし */
	void UpdateWorldMatrix();

	/** 全ての子トランスフォームとの紐づけを外す */
	void Release();

	/** 特定の子トランスフォームとの紐づけを外す */
	void RemoveChild(Transform* t);
	/** すべてのトランスフォームとの紐づけを外す */
	void ClearChild();
	void AddChild(Transform* t)
	{
		// 同じのは追加しない
		if (FindChild(t)) {
			return;
		}
		m_children.push_back(t);
	}
	bool FindChild(Transform* t)
	{
		for (auto* child : m_children)
		{
			if (child == t) {
				return true;
			}
		}
		return false;
	}

	/** 親トランスフォームを設定 */
	void SetParent(Transform* p)
	{
		if (m_parent) {
			return;
		}
		m_parent = p;
		m_parent->AddChild(this);
	}

	/** 親トランスフォームがあるか */
	bool HasParent() const
	{
		return m_parent != nullptr;
	}
	/** 親を解除 */
	void ClearParent()
	{
		m_parent = nullptr;
	}
};