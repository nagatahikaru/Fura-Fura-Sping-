#pragma once
/**
* @file   CharacterModel.h
* @brief  キャラクターモデル/アニメーションを管理するクラス。
*         BossModel/Player1p～4pモデルを管理。
* @author Yamaguchi Hayato。
* @date   2026/03/10
*/

namespace nsApp
{
	enum class CharacterModelType
	{
		/* バッターモデルを定義。*/
		BatterUniformNumber_0,		

		///* ピッチャーモデルを定義。*/		
		//PicherUniformNumber_0,		

		///*キャッチャーモデルを定義。*/
		//CatcherUniformNumber_0,

		/* モデルを定義。*/
		Bat,
		Boll,
		Null
	};


	class CharacterModel 
	{
	public:
		/* コンストラクタとデストラクタ。*/
		CharacterModel();
		~CharacterModel() = default;


	public:
	    /* モデルを読み込む。
		* @param characterType モデルの種類をしているする。
		* @param animationClip アニメーションクリップの配列。
		* @param numClips      アニメーションクリップの数。
		*/
		bool LoadCharacterModel(CharacterModelType characterType, AnimationClip* animationClip, int numClips);

		/**
		* 武器を読み込む。
		*/
		bool LoadWeaponModel(CharacterModelType weaponType);

		/* アニメーションを再生する。
		* @param animationNumber 再生するアニメーションの配列の要素数を指定する。
		* @param interpolateTime アニメーションの補間時間を指定する。デフォルトは0.2秒。
		*/
		void PlayAnimation(int animationNumber, float interpolateTime);

		/* キャラクターを描画する。*/
		void DrawCharacterModel(RenderContext& rc);

		/* 更新処理。*/
		void Update();

		/* アニメーションが再生終了したか。*/
	    bool IsPlayAnimation()
		{
			/* nullチェック。*/
			if (m_characterModelRender != nullptr)
				/* アニメーションが再生終了しているかを返す。*/
				return m_characterModelRender->IsPlayingAnimation();

			return false;
		}

	/* セッター。*/
	public:
		/* 座標を設定。*/
		inline void SetPosition(const Vector3& position)
		{
			if (m_characterModelRender)
				m_characterModelRender->SetPosition(position);
		}

		/* 回転を設定。*/
		inline void SettRotation(const Quaternion& rotation)
		{
			if (m_characterModelRender)
				m_characterModelRender->SetRotation(rotation);
		}

		/* キャラモデルの大きさを設定。*/
	    inline void SetCharacterScale(const Vector3& scale)
		{
			if (m_characterModelRender)
				m_characterModelRender->SetScale(scale);
		}

		/* 武器モデルの大きさを設定。*/
		inline void SetWeaponScale(const Vector3& scale)
		{
			if (m_weaponModelRender)
				m_weaponModelRender->SetScale(scale);
		}

		/* 武器のずれを調整。*/
		inline void SetWeaponOffset(const Vector3& offset) 
		{
			m_weaponOffset = offset;
		}

		/* 武器を装備するボーンの名前を変更する。*/
		inline void SetWeaponAttackBone(const std::wstring& boneName)
		{
			m_attackBoneName = boneName;
		}

		inline void SetWeaponRotation(const Quaternion& rotation)
		{
			if (m_weaponModelRender)
				m_weaponModelRender->SetRotation(rotation);
		}
		inline void SetWeaponPosition(const Vector3& position)
		{
			if (m_weaponModelRender)
				m_weaponModelRender->SetPosition(position);
		}

		void SetWeaponRotation(bool isWeaponRotation)
		{
			m_isWeaponRotasition = isWeaponRotation;
		}
		void SetPlaySpeed(float speed) {
			m_playSpeed = speed;
		}
		void AimRightHand(const Vector3& targetPos);

	/* ゲッター。*/
	public:
		/* キャラモデルのファイルパスを格納。*/
		inline const std::string GetCharacterModelFilePath(std::string filePath)
		{
			 std::string modelPath = m_characterModelFilePath + filePath + m_modelExtension;
			 return modelPath;
		}

		/* 武器モデルのファイルパスを格納。*/
		inline const std::string GetWeaponModelFilePath(std::string filePath)
		{
			std::string weaponFilePath = m_weaponModelFilePath + filePath + m_modelExtension;
			return weaponFilePath;
		}

		Vector3 GetWeaponWorldPosition();

		Vector3 GetBatTip()
		{
			return m_offsetPosition + m_zAxis * 100.0f; // 長さは調整
		}

		Vector3 GetBatBase()
		{
			return m_offsetPosition;
		}

		/* 指定したボーンのワールド行列を取得する。
		* @param boneName 取得したいボーンの名前を指定する。
		*/
	    Matrix GetWorldMatrix(const wchar_t* boneName);

		float GetPlaySpeed() const {
			return m_playSpeed;
		}

		ModelRender* GetModelRender()
		{
			return m_characterModelRender.get();
		}

	private:	
		std::unordered_map<CharacterModelType, std::string> m_filePathList;		                                               /* モデルIDからファイルパスを文字列に変化。*/
		std::unique_ptr<ModelRender> m_characterModelRender;                                                                   /* モデルを管理。*/
		std::unique_ptr<ModelRender> m_weaponModelRender;                                                                      /* 武器モデルを管理。*/
		std::wstring m_attackBoneName;

		/* ファイルパスを定数化するための変数群。*/
		std::string m_modelFilePath;                                                                                           /* モデルのファイルパスを格納。*/
		std::string m_characterModelFilePath = "Assets/modelData/";															   /* プレイヤー/NPCモデルのファイルパスを格納。*/
		std::string m_modelExtension = ".tkm";                                                                                 /* プレイヤー/NPCモデルの拡張子を格納。*/
		std::string m_weaponModelFilePath = "Assets/modelData/";                                              /* 武器モデルのファイルパスを格納。*/

		//int boneID;                                                                                                            /* ボーンIDを格納。*/

		Quaternion m_matrixRotation;
		Quaternion m_weaponRotation;                                                                                             /* 武器の回転を管理する変数。*/

		Matrix m_handMatrix; 																					               /* 武器を装備させるときの右手のボーンの行列を管理する変数。*/
		Matrix m_rotationMatrix;                                                                                               /* 武器を装備させたときの武器の傾きを制御。*/

		Vector3 m_xAxis;                                                                                                       /* 武器を装備させるときの右手のボーンの行列から抽出したX軸を管理する変数。*/
		Vector3 m_yAxis;                                                                                                       /* 武器を装備させるときの右手のボーンの行列から抽出したY軸を管理する変数。*/
		Vector3 m_zAxis;                                                                                                       /* 武器を装備させるときの右手のボーンの行列から抽出したZ軸を管理する変数。*/
		Vector3 m_matrixPosition;
		Vector3 m_weaponOffset;                                                                                                /* 武器の位置のずれを管理する変数。*/
		Vector3 m_offsetPosition; 
		float m_playSpeed = 1.0f; /* 武器の位置のずれを管理する変数。*/
		bool m_isWeaponRotasition = false;                                                                                             /* 武器の回転を制御する変数。*/
		Quaternion m_weaponOffsetRot;
	};	
}
