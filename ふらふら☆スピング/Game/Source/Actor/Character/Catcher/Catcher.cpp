#include "stdafx.h"
#include "Catcher.h"
#include "Source/Actor/Character/Ball/Ball.h"

namespace CATCHER {
	std::string FILE_PATH_CATCHER_AIM = ("Assets/animData/catcher/");
	std::string FILE_PATH_DDS = (".tka");
	std::string FILE_PATH_ANIMATION[1] = {
		"idle"
	};


	inline std::string GetAnimationFilePath(int number)
	{
		return FILE_PATH_CATCHER_AIM + FILE_PATH_ANIMATION[number] + FILE_PATH_DDS;
	}

	void LoadAnimationClips(AnimationClip* m_animationClips, int enAnimation, int enAnimationClip_Num)
	{
		for (int j = enAnimation; j < enAnimationClip_Num; j++)
		{
			m_animationClips[j].Load(GetAnimationFilePath(j).c_str());
			m_animationClips[j].SetLoopFlag(true);
		}
	}
};

Catcher::~Catcher()
{

}

bool Catcher::Start()
{
	CATCHER::LoadAnimationClips(m_animationClips, enAnimationClip_Idle, enAnimationClip_Num);


	// インスタンスの生成
	m_characterModel = std::make_unique<nsApp::CharacterModel>();

	// ※ animationClip と numClips は環境に合わせて適切な値を渡してください
	m_characterModel->LoadCharacterModel(
		nsApp::CharacterModelType::CatcherUniformNumber_0,
		m_animationClips,
		enAnimationClip_Num);

	// 2. 武器（グローブ）の読み込み
	m_characterModel->LoadWeaponModel(nsApp::CharacterModelType::Glove);

	// 3. アタッチするボーンの設定とオフセットの調整
	m_characterModel->SetWeaponAttackBone(L"mixamorig:RightHand"); // 実際のボーン名に合わせる
	m_characterModel->SetWeaponOffset(CATCHER::GLOVE::OFFSET_GLOVE);
	

	m_transform.m_position = CATCHER::CatcherBasicSettings::INITIAL_COORDINATE;
	Quaternion rot = m_transform.m_rotation;
	rot.AddRotationY(Math::DegToRad(CATCHER::CatcherBasicSettings::INITIAL_ROTATION_Y));
	m_transform.m_rotation = rot;

	Quaternion rotWeapon = m_rotWeapon;
	rotWeapon.AddRotationX(Math::DegToRad(30.0f));
	rotWeapon.AddRotationZ(Math::DegToRad(75.0f));
	m_rotWeapon = rotWeapon;


	// 4. 初期位置やスケールの設定
	m_characterModel->SetPosition(m_transform.m_position);
	m_characterModel->SetCharacterScale(CATCHER::CatcherBasicSettings::INITIAL_SCALE);
	m_characterModel->SettRotation(m_transform.m_rotation);
	m_characterModel->SetWeaponScale(CATCHER::GLOVE::INITIAL_SCALE);
	m_characterModel->SetWeaponRotation(m_rotWeapon);

	m_characterModel->Update();

	m_ball = FindGO<Ball>("ball");
	m_game = FindGO<Game>("game");

	return true;
}

void Catcher::Update()
{
	if (m_isPaused) return;

	if (m_game == nullptr)
	{
		m_game = FindGO<Game>("game");
		return;
	}

	if (m_game && m_game->GetIsPaused()) return;
	if (!m_game->IsGameStarted())return;

	Catch();
}

void Catcher::Catch()
{
	if (m_collisionObject->IsHit(m_ball->GetCollisionObject()))
	{
		m_game->OnBallLanded();
	}
}

void Catcher::PlayeAnimation(int animationNo)
{
	if (m_characterModel)
		m_characterModel->PlayAnimation(animationNo, 0.2f);
}

void Catcher::Render(RenderContext& rc)
{
	if (m_game == nullptr)
	{
		m_game = FindGO<Game>("game");
		return;
	}
	if (m_game->GetCameraType() == 0)
	{
		return;
	}
	//モデルの描画

	m_characterModel->DrawCharacterModel(rc);
}