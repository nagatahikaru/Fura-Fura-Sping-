#pragma once

class Source:public IGameObject
{
	public:
	Source() {}
	virtual ~Source() {}

	/// <summary>
	/// float用のデバック関数です。
	/// </summary>
	/// <param name="type"> デバックする値の名前を指定します。
	/// <para>name ：デバックする値の名前</para>
	/// <para>value：デバックするfloatの値</para>
	/// </param>
	void DebugLogFloat(const char* name, float value)
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
	void DebugLogVector3(const char* name, const Vector3& value)
	{
		char buf[256];
		sprintf_s(buf, "%s = (%f, %f, %f)\n", name, value.x, value.y, value.z);
		OutputDebugStringA(buf);
	}
};

