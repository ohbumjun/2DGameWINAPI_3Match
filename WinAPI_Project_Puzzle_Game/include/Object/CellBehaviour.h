#pragma once

class CCellBehaviour
{
public :
	CCellBehaviour();
	~CCellBehaviour();
private :
	class CCellObject* m_Owner;
public :
	bool Init();
	void SetOwner(CCellObject* CellObject)
	{
		m_Owner = CellObject;
	}
};

