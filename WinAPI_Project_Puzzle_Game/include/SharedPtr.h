#pragma once

template<typename T>
class CSharedPtr
{
private :
	T* m_Ptr;
public :
	CSharedPtr :
	m_Ptr(nullptr)
	{
	}

	CSharedPtr(T* ptr)
	{
		m_Ptr = ptr;
		if (m_Ptr)
			m_Ptr->AddRef();
	}
public :
	void operator = (T* Ptr)
	{
		if (m_Ptr)
			m_Ptr->Rel
	}
};