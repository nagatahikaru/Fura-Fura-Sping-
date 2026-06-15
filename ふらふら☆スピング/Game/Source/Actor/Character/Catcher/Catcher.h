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
	ModelRender m_modelRender;
	AnimationClip m_animationClips[enAnimationClip_Num];
	CharacterController m_characterController;
	CollisionObject* m_collisionObject = nullptr; // 衝突オブジェクト


public:
	Catcher() {};
	virtual ~Catcher();
	virtual bool Start();
	virtual void Update();
	void PlayeAnimation(int animationNo);
	void Catch();
	void AnimationUpdate()
	{
		
		m_modelRender.Update();
	}
	ModelRender* GetModelRender() 
	{
		return &m_modelRender;
	}
	virtual void Render(RenderContext& rc);
	bool m_isPaused = false; // アニメーションの一時停止フラグ
};

