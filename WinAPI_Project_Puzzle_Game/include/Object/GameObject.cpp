#include "GameObject.h"
#include "../Animation/Animation.h"
#include "../Resource/ResourceManager.h"

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

void CGameObject::CreateAnimation()
{
	if (!m_Animation)
	{
		m_Animation = new CAnimation;

		m_Animation->m_Owner = this;
		m_Animation->m_CurrentAnimation = nullptr;
	}
}

void CGameObject::AddAnimation(const std::string& SequenceName, bool Loop, float PlayTime, float PlayScale, bool Reverse)
{
	if (!m_Animation)
	{
		m_Animation = new CAnimation;

		m_Animation->m_Owner = this;
		m_Animation->m_CurrentAnimation = nullptr;
	}
	m_Animation->AddAnimation(SequenceName, Loop, PlayTime, PlayScale, Reverse);
}

void CGameObject::SetAnimationPlayTime(const std::string& Name, float PlayTime)
{
	if (!m_Animation)
		return;
	m_Animation->SetPlayTime(Name, PlayTime);
}

void CGameObject::SetAnimationPlayScale(const std::string& Name, float PlayScale)
{
	if (!m_Animation)
		return;
	m_Animation->SetPlayScale(Name, PlayScale);
}

void CGameObject::SetCurrentAnimation(const std::string& Name)
{
	if (!m_Animation)
		return;
	m_Animation->SetCurrentAnimation(Name);
}

void CGameObject::ChangeAnimation(const std::string& Name)
{
	if (!m_Animation)
		return;
	return m_Animation->ChangeAnimation(Name);
}

bool CGameObject::CheckCurrentAnimation(const std::string& Name)
{
	if (!m_Animation)
		return false;
	return m_Animation->CheckCurrentAnimation(Name);
}

void CGameObject::SetAnimationReverse(const std::string& Name, bool Reverse)
{
	if (!m_Animation)
		return;

	m_Animation->SetReverse(Name, Reverse);
}

void CGameObject::SetAnimationLoop(const std::string& Name, bool Loop)
{
	if (!m_Animation)
		return;

	m_Animation->SetLoop(Name, Loop);
}

void CGameObject::SetTexture(const std::string& Name)
{
	m_Texture = CResourceManager::GetInst()->FindTexture(Name);
	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
	}
}

void CGameObject::SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName);
	m_Texture = CResourceManager::GetInst()->FindTexture(Name);
	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
	}
}

void CGameObject::SetTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	CResourceManager::GetInst()->LoadTextureFullPath(Name, FullPath);

	m_Texture = CResourceManager::GetInst()->FindTexture(Name);
	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
	}
}

void CGameObject::SetTexture(const std::string& Name, const std::vector<std::wstring>& vecFileName, const std::string& PathName)
{
	CResourceManager::GetInst()->LoadTexture(Name, vecFileName, PathName);

	m_Texture = CResourceManager::GetInst()->FindTexture(Name);
	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
	}
}

void CGameObject::SetTextureColorKey(unsigned char r, unsigned char g, unsigned char b, int Index)
{
	if (m_Texture)
		m_Texture->SetColorKey(r, g, b, Index);
}
