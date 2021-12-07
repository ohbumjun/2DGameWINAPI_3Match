#pragma once

#include "GameInfo.h"

class CRef
{
public:
	CRef();
	CRef(const CRef& ref);
	virtual ~CRef();

protected:
	std::string	m_Name;
	bool	m_Active;
	int		m_RefCount;

public:
	void Destroy()
	{
		m_Active = false;
	}

	void AddRef()
	{
		++m_RefCount;
	}

	void MinusRef()
	{
		--m_RefCount;
	}

	int GetRefCount()	const
	{
		return m_RefCount;
	}

	int Release()
	{
		--m_RefCount;
		if (m_RefCount <= 0)
		{
			delete	this;
			return 0;
		}
		return m_RefCount;
	}

public:
	bool IsActive()	const
	{
		return m_Active;
	}

	std::string GetName()	const
	{
		return m_Name;
	}

	void SetName(const std::string& Name)
	{
		m_Name = Name;
	}
};

