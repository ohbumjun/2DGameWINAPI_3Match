#include "Ref.h"

CRef::CRef() :
	m_Active(true),
	m_RefCount(0)
{
}

CRef::CRef(const CRef& ref) :
	m_Active(true),
	m_RefCount(0),
	m_Name(ref.m_Name)
{
}

CRef::~CRef()
{
}
