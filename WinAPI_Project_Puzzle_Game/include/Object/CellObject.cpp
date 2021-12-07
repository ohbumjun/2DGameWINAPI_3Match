#include "CellObject.h"
#include "CellBehaviour.h"

CCellObject::CCellObject()
{
}

CCellObject::~CCellObject()
{
}

bool CCellObject::Init()
{
	m_CellBehaviour = new CCellBehaviour;

	if(!m_CellBehaviour->Init());
		return false;

	m_CellBehaviour->SetOwner(this);

	return true;
}
