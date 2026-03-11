#pragma once
#include "Source/Actor/Character/Character.h"

class Ball :public Character
{
public:
	Ball();
	virtual ~Ball();
	virtual bool Start();
	virtual void Update();
	virtual void Throw();
	virtual void Render(RenderContext& rc);

	//ボールを投げる関数
	void Throw(Vector3 targetPos);

private:
	Vector3 m_position;  //ボールの位置
	Vector3 m_velocity;  //ボールの速度
	Vector3 m_targetPos; //目標位置

	bool m_isMove = false; 

};

