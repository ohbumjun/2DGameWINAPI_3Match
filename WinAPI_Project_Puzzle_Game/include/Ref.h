#pragma once

#include "GameInfo.h"

class CRef
{
public :
	CRef();
	CRef(const CRef& ref);
	virtual ~CRef();
protected :
	int m_RefCount;
	std::string m_Name;
	bool m_Active;
	bool m_RenderEnable;
public :
	void Destroy()
	{
		m_Active = false;
	}
	void SetName(const std::string& Name)
	{
		m_Name = Name;
	}
	void AddRef()
	{
		++m_RefCount;
	}
	int Release()
	{
		--m_RefCount;
		if (m_RefCount <= 0)
		{
			delete this;
			return 0;
		}
		return m_RefCount;
	}
	void SetEnable(bool Enable)
{
		m_RenderEnable = Enable;
}
public :
	bool GetEnable() const
	{
		return m_RenderEnable;
	}
	int GetRefCount() const  
	{
		return m_RefCount;
	}
	bool IsActive() const
	{
		return m_Active;
	}
	std::string GetName() const
	{
		return m_Name;
	}


};