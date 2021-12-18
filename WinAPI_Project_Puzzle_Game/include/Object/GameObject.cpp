#include "GameObject.h"
#include "../Animation/Animation.h"
#include "../Resource/ResourceManager.h"

CGameObject::CGameObject() :
	m_Pos(Vector2(0.f, 0.f)),
	m_Size(Vector2(0.f, 0.f)),
	m_RowIndex(-1),
	m_ColIndex(-1),
	m_Index(-1),
	m_Texture(nullptr),
	m_Start(false),
	m_RenderPos(Vector2(0.f, 0.f)),
	m_Offset(Vector2(0.f, 0.f)),
	m_Animation(nullptr)
{
}

CGameObject::CGameObject(const CGameObject& Obj)
{
	m_Pos = Obj.m_Pos;
	m_Size = Obj.m_Size;
	m_RowIndex = Obj.m_RowIndex;
	m_ColIndex = Obj.m_ColIndex;
	m_Index = Obj.m_Index;
	m_Pivot = Obj.m_Pivot;
	m_Start = false;
	m_Offset = Obj.m_Offset;
	m_Texture = Obj.m_Texture;
	m_ImageStart = Obj.m_ImageStart;

	if (Obj.m_Animation)
		m_Animation = Obj.m_Animation->Clone();
	else
		m_Animation = nullptr;
	if (m_Animation)
		m_Animation->m_Owner = this;
}

CGameObject::~CGameObject()
{
	SAFE_DELETE(m_Animation);
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

void CGameObject::SetTexture(CTexture* Texture)
{
	m_Texture = Texture;
	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
	}
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

void CGameObject::Start()
{
	m_Start = true;
}

bool CGameObject::Init()
{
	return true;
}

void CGameObject::Update(float DeltaTime)
{
	if (!m_Start)
	{
		Start();
		m_Start = true;
	}
	
	// Animation
	if (m_Animation)
		m_Animation->Update(DeltaTime);
}

void CGameObject::PostUpdate(float DeltaTime)
{
	if (m_Animation && m_Animation->m_CurrentAnimation)
	{
		AnimationInfo* AnimInfo = m_Animation->m_CurrentAnimation;
		const AnimationFrameData FrameData = AnimInfo->Sequence->GetFrameData(AnimInfo->Frame);
		m_Size = FrameData.Size;
	}
}

void CGameObject::PrevRender()
{
	m_RenderPos = m_Pos;
}

void CGameObject::Render(HDC hDC)
{
	// Animation이 있다면 Animation Render
	if (m_Animation && m_Animation->m_CurrentAnimation)
	{
		AnimationInfo* AnimInfo = m_Animation->m_CurrentAnimation;
		const AnimationFrameData FrameData = AnimInfo->Sequence->GetFrameData(AnimInfo->Frame);
		Vector2 LT = m_RenderPos - m_Pivot * FrameData.Size + m_Offset;
		if (AnimInfo->Sequence->GetTextureType() == ETexture_Type::Atlas) // 하나의 이미지 파일 --> 하나의 Texture 구성 
		{
			AnimInfo->Sequence->GetTexture()->Render(hDC, LT, FrameData.StartPos, FrameData.Size);
		}
		else // 여러개 이미지 파일 --> Texture 구성 
		{
			AnimInfo->Sequence->GetTexture()->Render(hDC, LT,
				FrameData.StartPos, FrameData.Size, AnimInfo->Frame);
		}
	}
	// 그게 아니라면 Texture Render
	else
	{
		Vector2 LT = m_RenderPos - m_Pivot * m_Size + m_Offset; // 여기서 m_Size는 Texture Size로 맞춰지게 세팅되어 있다
		if (m_Texture)
		{
			// 이미지를 이용해서 출력
			m_Texture->Render(hDC, LT, m_ImageStart, m_Size);
		}
		else
		{
			Rectangle(hDC, (int)LT.x, (int)LT.y,(int)(LT.x + m_Size.x), (int)(LT.y + m_Size.y));
		}
	}
}

CGameObject* CGameObject::Clone()
{
	return new  CGameObject(*this);
}
