#pragma once

template <typename T>
class CSharedPtr
{
public:
	CSharedPtr() :
		m_Ptr(nullptr)
	{
	}

	// 너는 복사생성자가 아니야. 사용자 정의 생성자야 ! 
	CSharedPtr(T* Ptr)
	{
		m_Ptr = Ptr;
		
		if (m_Ptr)
			m_Ptr->AddRef();
	}

	//내가 복사 생성자 
	CSharedPtr(const CSharedPtr<T>& Ptr)
	{
		m_Ptr = Ptr.m_Ptr;

		if (m_Ptr)
			m_Ptr->AddRef();
	}

	~CSharedPtr()
	{
		if (m_Ptr)
			m_Ptr->Release();
	}

private:
	T* m_Ptr;

public:
	void operator = (T* Ptr)
	{
		// 기존에 가지고 있는 레퍼런스 객체가 있을 경우 카운트를 1 감소시켜준다.
		if (m_Ptr)
			m_Ptr->Release();

		m_Ptr = Ptr;

		// 새로 지정된 레퍼런스 객체가 있을 경우 카운트를 1 증가시켜준다.
		if (m_Ptr)
			m_Ptr->AddRef();
	}

	void operator = (const CSharedPtr<T>& Ptr)
	{
		// 기존에 가지고 있는 레퍼런스 객체가 있을 경우 카운트를 1 감소시켜준다.
		if (m_Ptr)
			m_Ptr->Release();

		m_Ptr = Ptr.m_Ptr;

		// 새로 지정된 레퍼런스 객체가 있을 경우 카운트를 1 증가시켜준다.
		if (m_Ptr)
			m_Ptr->AddRef();
	}

	bool operator == (T* Ptr)	const
	{
		return m_Ptr == Ptr;
	}
	bool operator == (const CSharedPtr<T>& Ptr)	const
	{
		return m_Ptr == Ptr.m_Ptr;
	}
	bool operator != (T* Ptr)	const
	{
		return m_Ptr != Ptr;
	}
	bool operator != (const CSharedPtr<T>& Ptr)	const
	{
		return m_Ptr != Ptr.m_Ptr;
	}

	// 변환연산자(operator conversion)
	operator T* ()	const
	{
		return m_Ptr;
	}
	T* operator -> ()	const
	{
		return m_Ptr;
	}
};

