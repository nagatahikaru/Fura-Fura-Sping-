#pragma once
#include "Source/Source.h"

class Actor:public Source
{
	public:
		Actor();
		virtual ~Actor();

public:
	Transform m_transform;

protected:
	ModelRender m_modelRender;
};

