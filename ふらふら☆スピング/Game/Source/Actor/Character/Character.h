#pragma once
#include "Source/Actor/Actor.h"

class Character :public Actor
{
public:
	Character();
	virtual ~Character();
	virtual bool Start()
	{

		return true;
	}

private:

protected:
	CharacterController m_characterController;

};

