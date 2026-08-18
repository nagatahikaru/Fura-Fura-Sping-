#pragma once
#include "Source/Actor/Character/Character.h"
#include "Source/Scene/InGame/Game.h"

namespace CATCHER {
	namespace CatcherBasicSettings
	{
		const Vector3 INITIAL_COORDINATE = Vector3(0.0f, -50.0f, 6000.0f);//初期座標
		const Vector3 VECTOR_UP = Vector3(0.0f, 1.0f, 0.0f); //上方向ベクトル
		const Vector3 INITIAL_SCALE = Vector3(10.0f, 10.0f, 10.0f); //初期スケール
		const Vector3 COLLISION_SCALE = Vector3(50.0f, 35.0f, 50.0f); //当たり判定スケール
		constexpr float INITIAL_ROTATION_Y = 180.0f; //初期回転角度
	}

	namespace GLOVE {
		const Vector3 OFFSET_GLOVE = Vector3(60.0f, -340.0f, -280.0); //グローブのオフセット
		const Vector3 COLLISION_SCALE_GLOVE = Vector3(40.0f, 40.0f, 40.0f);
		const Vector3 INITIAL_SCALE = Vector3(10.0f, 10.0f, 10.0f); //初期スケール
		const Vector3 ROTATION_ANGLE = Vector3(0.0f, 0.0f, 1.0f); //グローブの回転角度と回転速度
		constexpr float SPIN_TIME_LIMIT = 5.0f;             //回転の時間制限
		constexpr float HALF_ROTATION_ANGLE = 180.0f;   //回転の半分の角度
		constexpr float FULL_ROTATION_ANGLE = 360.0f;   //回転の全体の角度
	}

	extern std::string FILE_PATH_CATCHER_AIM;
	extern std::string FILE_PATH_CATCHER;
	extern std::string FILE_PATH_TKM;
	extern std::string FILE_PATH_DDS;
	extern std::string FILE_PATH;
	extern std::string FILE_PATH_ANIMATION[1];
};

class Catcher :public Character
{
private:
	enum EnAnimationClip {
		enAnimationClip_Idle,
		enAnimationClip_Num
	};

	Ball* m_ball = nullptr;
	Game* m_game = nullptr;
	AnimationClip m_animationClips[enAnimationClip_Num];
	CharacterController m_characterController;
	CollisionObject* m_collisionObject = nullptr; // 衝突オブジェクト
	Vector3 m_facingDir;
	float m_y;
	float m_x;
	FontRender m_xy;
	Quaternion m_rotWeapon;
	bool m_isPaused = false; // アニメーションの一時停止フラグ

public:
	Catcher() {};
	virtual ~Catcher();
	virtual bool Start();
	virtual void Update();
	void PlayeAnimation(int animationNo);
	void Catch();

	// アニメーション更新（Batterと同様、m_characterModel経由で実体のModelRenderを更新する）
	void AnimationUpdate()
	{
		if (m_characterModel)
			m_characterModel->Update();
	}

	// CharacterModel取得（Batterと同様の公開インターフェース）
	nsApp::CharacterModel* GetCharacterModel() const
	{
		return m_characterModel.get();
	}

	// 実体のModelRenderが必要な場合はCharacterModel経由で取得する
	ModelRender* GetModelRender()
	{
		return m_characterModel ? m_characterModel->GetModelRender() : nullptr;
	}
	virtual void Render(RenderContext& rc);
	void SetIsPaused(bool isPaused)
	{
		m_isPaused = isPaused;
	}
	bool GetIsPaused() const
	{
		return m_isPaused;
	}

};

