#include "stdafx.h"
#include "Source/Actor/Character/CharacterModel.h"

namespace nsApp
{
	CharacterModel::CharacterModel()
	{
		/* モデルのファイルパスを格納。*/
		/* キャラモデルの格納。*/
		/* プレイヤーモデル。*/
		m_filePathList[CharacterModelType::BatterUniformNumber_1] = GetCharacterModelFilePath("Batter/UniformNumber/1"); /* 1Pモデル。*/

		/* ピッチャーモデル。*/
		m_filePathList[CharacterModelType::PicherUniformNumber_1] = GetCharacterModelFilePath("Picher/UniformNumber/1");

		/* キャッチャーモデル。*/
		m_filePathList[CharacterModelType::CatcherUniformNumber_0] = GetCharacterModelFilePath("Catcher/UniformNumber/0"); /* ボスモデル。*/

		/* 装備モデル。*/
		/* バット。*/
		m_filePathList[CharacterModelType::Bat] = GetWeaponModelFilePath("Bat/bat");
		
	}


	bool CharacterModel::LoadCharacterModel(CharacterModelType characterType, AnimationClip* animationClip, int numClips)
	{
		/* 読み込むモデルがリストの中にあるか確認。*/
		if (m_filePathList.count(characterType) == 0)
			return false; /* 登録されていないならロード失敗。*/

		/* IDに対応するモデルパスを取り出す。*/
		m_modelFilePath = m_filePathList[characterType];

		/* モデルロードクラスを生成する。*/
		m_characterModelRender = std::make_unique<ModelRender>();

		/* モデルをロードする。*/
		m_characterModelRender->Init
		(
			m_modelFilePath.c_str(), /* モデルのファイルパス。*/
			animationClip,           /* アニメーション。*/
			numClips,                /* アニメーションの補完率。*/
			enModelUpAxisZ           /* モデルの上方向となる軸を設定。 今回はZ軸。*/
		);

		return true;
	}


	bool CharacterModel::LoadWeaponModel(CharacterModelType weaponType)
	{
		/* 読み込む武器がリストにあるのかを確認。*/
		if (m_filePathList.count(weaponType) == 0)
			return false;
		
		/* 武器モデルを生成してロードする。*/
		m_weaponModelRender = std::make_unique<ModelRender>();
		m_weaponModelRender->Init(
			m_filePathList[weaponType].c_str(),
			nullptr,
			0,
			enModelUpAxisZ
		);

		return true;
	}


	void CharacterModel::PlayAnimation(int animationNumber, float interpolateTime)
	{
		if (m_characterModelRender != nullptr)
		{
			m_characterModelRender->PlayAnimation(animationNumber, interpolateTime);
		}
	}


	void CharacterModel::DrawCharacterModel(RenderContext& rc)
	{
		/* 中身がある場合、描画する。*/
		if(m_characterModelRender != nullptr)
			m_characterModelRender->Draw(rc);

		/* 武器も描画する。*/
		if (m_weaponModelRender != nullptr)
			m_weaponModelRender->Draw(rc);
	}


	void CharacterModel::Update()
	{
		/* 更新処理。*/
		if(m_characterModelRender != nullptr)
			m_characterModelRender->Update();

		/* 武器を装備させる。*/
		if (m_characterModelRender != nullptr && m_weaponModelRender != nullptr)
		{
			/* 右手のボーンの位置と傾きのデータを取得。*/
			m_handMatrix = GetWorldMatrix(L"mixamorig:RightHand");

			/* 行列から座標を抽出。*/
			m_matrixPosition.x = m_handMatrix.m[3][0];
			m_matrixPosition.y = m_handMatrix.m[3][1];
			m_matrixPosition.z = m_handMatrix.m[3][2];
			
			m_xAxis = Vector3(m_handMatrix.m[0][0], m_handMatrix.m[0][1], m_handMatrix.m[0][2]);
			m_yAxis = Vector3(m_handMatrix.m[1][0], m_handMatrix.m[1][1], m_handMatrix.m[1][2]);
			m_zAxis = Vector3(m_handMatrix.m[2][0], m_handMatrix.m[2][1], m_handMatrix.m[2][2]);

			/* 正規化。*/
			m_xAxis.Normalize();
			m_yAxis.Normalize();
			m_zAxis.Normalize();

			m_offsetPosition = m_matrixPosition + (m_xAxis * m_weaponOffset.x), (m_yAxis * m_weaponOffset.y), (m_zAxis * m_weaponOffset.z);
			m_weaponModelRender->SetPosition(m_offsetPosition);
				                 

		    m_rotationMatrix = m_handMatrix;
			m_rotationMatrix.m[0][0] = m_xAxis.x; m_rotationMatrix.m[0][1] = m_xAxis.y; m_rotationMatrix.m[0][2] = m_xAxis.z;
			m_rotationMatrix.m[1][0] = m_yAxis.x; m_rotationMatrix.m[1][1] = m_yAxis.y; m_rotationMatrix.m[1][2] = m_yAxis.z;
			m_rotationMatrix.m[2][0] = m_zAxis.x; m_rotationMatrix.m[2][1] = m_zAxis.y; m_rotationMatrix.m[2][2] = m_zAxis.z;

			/* 正規化した行列から回転を抽出。*/
			m_matrixRotation.SetRotation(m_rotationMatrix);
			m_weaponModelRender->SetRotation(m_matrixRotation);

			/* 武器を更新。*/
			m_weaponModelRender->Update();
		}
	}


	Matrix CharacterModel::GetWorldMatrix(const wchar_t* boneName)
	{
		if (m_characterModelRender)
		{
			/* ボーンIDを取得。*/
			boneID = m_characterModelRender->FindBoneID(boneName);

			if (boneID != -1)
				return m_characterModelRender->GetBone(boneID)->GetWorldMatrix();
		}
		/* ボーンが見つからない場合。*/
		return Matrix::Identity;
	}
}