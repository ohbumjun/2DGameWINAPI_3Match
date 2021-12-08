#include "Texture.h"

CTexture::CTexture()
{
}

CTexture::~CTexture()
{
	size_t Size = m_vecTextureInfo.size();

	for (size_t i = 0; i < Size; i++)
	{
		SAFE_DELETE(m_vecTextureInfo[i]);
	}
}

bool CTexture::LoadTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	const
	
	return false;
}

bool CTexture::LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	return false;
}

bool CTexture::LoadTexture(const std::string& Name, const std::vector<std::wstring>& vecFileName, const std::string& PathName)
{
	return false;
}

void CTexture::Render(HDC hDC, const Vector2& WindowPos, const Vector2& ImagePos, const Vector2& Size, int Index)
{
}
