#include "GameObject.h"

CGameObject::CGameObject() :
	m_Pos(Vector2(0.f, 0.f)),
	m_Size(Vector2(0.f, 0.f)),
	m_RowIndex(-1),
	m_ColIndex(-1),
	m_Index(-1),
	m_Texture(nullptr)
{
}

CGameObject::CGameObject(const CGameObject& Obj)
{
	m_Pos = Obj.m_Pos;
	m_Size = Obj.m_Size;
	m_RowIndex = Obj.m_RowIndex;
	m_ColIndex = Obj.m_ColIndex;
	m_Index = Obj.m_Index;

}

CGameObject::~CGameObject()
{
}
