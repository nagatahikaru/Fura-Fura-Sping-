#pragma once
#include "Source/Actor/Actor.h"
#include "Source/Actor/Character/CharacterModel.h"

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
	// CharacterModel ‚ğƒƒ“ƒo•Ï”‚Æ‚µ‚Ä•Û
	std::unique_ptr<nsApp::CharacterModel> m_characterModel;

};

