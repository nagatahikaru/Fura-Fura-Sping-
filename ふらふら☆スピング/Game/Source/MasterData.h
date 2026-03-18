#pragma once

class MasterData :public IGameObject
{
	public:
	MasterData() {}
	virtual ~MasterData() {}

	void SetRandom(Vector3& seed)
	{
		m_randomSeed = seed;
	}

	void SetNumberofRevolutions(int number)
	{
		m_numberOfRevolutions = number;
	}

	Vector3 GetRandom() const
	{
		return m_randomSeed;
	}

	int GetNumberOfRevolutions() const
	{
		return m_numberOfRevolutions;
	}

private:
	int m_numberOfRevolutions = 0;
	Vector3 m_randomSeed;
};

