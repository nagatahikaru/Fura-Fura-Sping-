#pragma once

namespace nsK2Engine {
	enum EnSkyCubeType {
		enSkyCubeType_Day,		// 昼間
		enSkyCubeType_Night,	// 夜間
		enSkyCubeType_Snow,		// 雪山
		enSkyCubeType_Snow_2,	// 雪山_2
		enSkyCubeType_Wild,		// 荒野
		enSkyCubeType_Wild_2,	// 荒野２
		enSkyCubeType_Wild_Night,	// 荒野(夜間)
		enSkyCubeType_Grass,	// 芝生
		enSkyCubeType_Euro,		// 欧州
		enSkyCubeType_DayToon,	// 昼間(トゥーン調)
		enSkyCubeType_DayToon_2,// 昼間(トゥーン調)２
		enSkyCubeType_DayToon_3,// 昼間(トゥーン調)３
		enSkyCubeType_DayToon_4,// 昼間(トゥーン調)４
		enSkyCubeType_NightToon, // 夜間(トゥーン調)
		enSkyCubeType_NightToon_2, // 夜間(トゥーン調)２
		enSkyCubeType_SunriseToon,	// 明け方(トゥーン調)
		//enSkyCubeType_SpaceToon,	// 大気圏(トゥーン調) 正常にでないのでコメントアウト。
		enSkyCubeType_SpaceToon_2,	// 大気圏(トゥーン調)２
		enSkyCubeType_Num,
	};
	/// <summary>
	/// ?
	/// </summary>
	class SkyCube : public IGameObject
	{
	public:
		SkyCube();
		~SkyCube();
		bool Start()override final;
		void Update()override final;
		void Render(RenderContext& rc)override final;
	public:
		void SetPosition(const Vector3& pos)
		{
			m_position = pos;
			m_isDirty = true;
		}

		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
			m_isDirty = true;
		}
		void SetScale(const float scale)
		{
			m_scale = g_vec3One;
			m_scale.Scale(scale);
			m_isDirty = true;
		}
		/// <summary>
		/// スカイキューブのタイプを設定。
		/// </summary>
		/// <param name="type"></param>
		void SetType(EnSkyCubeType type) 
		{
			m_type = type;
		}
		/// <summary>
		/// 明るさを設定。
		/// </summary>
		/// <param name="lum"></param>
		void SetLuminance(float lum)
		{
			m_luminance = lum;
		}
		const wchar_t* GetTextureFilePath()
		{
			return m_textureFilePaths[m_type];
		}
	private:
		ModelRender m_modelRender;
		
		Texture m_texture[enSkyCubeType_Num];
		const wchar_t* m_textureFilePaths[enSkyCubeType_Num];
		Vector3 m_position = g_vec3Zero;
		Vector3 m_scale = g_vec3One * 5000.0f;
		float m_luminance = 1.0f;
		bool m_isDirty = false;
		EnSkyCubeType m_type = enSkyCubeType_Day;
	};
}

