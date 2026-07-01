#pragma once

///このクラス関数の呼び出し方
/// MasterData::DebugLogFloat("変数名", 変数);
/// MasterData::DebugLogVector3("変数名", 変数);
/// MasterData::DebugLogMatrix("変数名", 変数);
class MasterData :public IGameObject
{
	public:
	MasterData() {}
	virtual ~MasterData() {}

	/// <summary>
	/// float用のデバック関数です。
	/// </summary>
	/// <param name="type"> デバックする値の名前を指定します。
	/// <para>name ：デバックする値の名前</para>
	/// <para>value：デバックするfloatの値</para>
	/// </param>
	static void DebugLogFloat(const char* name, float value)
	{
		char buf[256];
		sprintf_s(buf, "%s = %f\n", name, value);
		OutputDebugStringA(buf);
	}

	/// <summary>
	/// Vector3用のデバック関数です。
	/// </summary>
	/// <param name="type"> デバックする値の名前を指定します。
	/// <para>name ：デバックする値の名前</para>
	/// <para>value：デバックするVector3の値</para>
	/// </param>
	static void DebugLogVector3(const char* name, const Vector3& value)
	{
		char buf[256];
		sprintf_s(buf, "%s = (%f, %f, %f)\n", name, value.x, value.y, value.z);
		OutputDebugStringA(buf);
	}

	/// <summary>
	/// Matrixを出力のデバック関数です。
	/// </summary>
	/// <param name="type"> デバックする値の名前を指定します。
	/// <para>name ：デバックする値の名前</para>
	/// <para>value：デバックするMatrixの値</para>
	/// </param>
	static void DebugLogMatrix(const char* name, const Matrix& m)
	{
		char buf[1024];

		sprintf_s(buf,
			"%s\n"
			"[%8.3f %8.3f %8.3f %8.3f]\n"
			"[%8.3f %8.3f %8.3f %8.3f]\n"
			"[%8.3f %8.3f %8.3f %8.3f]\n"
			"[%8.3f %8.3f %8.3f %8.3f]\n",
			name,
			m.m[0][0], m.m[0][1], m.m[0][2], m.m[0][3],
			m.m[1][0], m.m[1][1], m.m[1][2], m.m[1][3],
			m.m[2][0], m.m[2][1], m.m[2][2], m.m[2][3],
			m.m[3][0], m.m[3][1], m.m[3][2], m.m[3][3]);

		OutputDebugStringA(buf);
	}
private:
};

