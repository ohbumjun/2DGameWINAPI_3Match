#include "Texture.h"
#include "../Resource/ResourceManager.h"
#include "../Path/PathManager.h"
#include "../GameManager.h"

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
	const PathInfo* FolderPath = CPathManager::GetInst()->FindPath(PathName);
	TCHAR FullPath[MAX_PATH] = {};

	if (FolderPath)
		lstrcpy(FullPath, FolderPath->Path);

	lstrcat(FullPath, FileName);
	
	return LoadTextureFullPath(Name, FullPath);
}

bool CTexture::LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	SetName(Name);

	TCHAR FileName[MAX_PATH];
	char FullPathMultibyte[MAX_PATH] = {};

#ifdef UNICODE
	int ConvertLength = WideCharToMultiByte(CP_ACP, 0, FullPath, -1, nullptr, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, FullPath, -1, FullPathMultibyte, ConvertLength, 0, 0);
#else
	strcpy_s(FullPathMultibyte, FullPath);
#endif // UNICODE

	// �빮�ڷ� �����
	_strupr_s(FullPathMultibyte);

	int PathLength = (int)strlen(FullPathMultibyte);

	// ���������� ó������ ���鼭 Ž��
	// ex)
	// D:/Project/Bin/Texture/Test.bmp
	// D:/Project/Bin/Texture/Player/Test.bmp
	for (int i = PathLength - 1; i >= 0; i--)
	{
		if (FullPathMultibyte[i] == '\\')
		{
			// ���� �̸� : /Texture --> // �ڿ� null���ڱ��� �ؼ� �� 9��
			char FolderName[9] = {};
			strcpy_s(FolderName, "ERUTXET\\");
			bool Find = true;

			for (int j = 1; j <= 8; j++)
			{
				if (FullPathMultibyte[i - j] != FolderName[j - 1])
				{
					Find = false;
					break;
				}
			}
			if (Find)
			{
				// Texture  //  ���ĺ��� �����ؼ� FileName�� ���� ���̴� 
				lstrcpy(FileName, &FullPath[i + 1]);
			}
		}
	}

	TextureInfo* Info = new TextureInfo;
	lstrcpy(Info->FileName, FileName);

	// �޸� DC�� �����
	Info->hDC = CreateCompatibleDC(CGameManager::GetInst()->GetWindowDC());

	// ��Ʈ�� �Ӽ����� ������ �ҷ����� HBTMAP���� ����ȯ�ؼ� hImage�� �����Ѵ�.
	Info->hBmp = (HBITMAP)LoadImage(
		CGameManager::GetInst()->GetWindowInstance(), // ���� �̹����� ������ �ִ� ���� ���� ����� �ڵ� (���μ����� �ڵ�)
		FullPath,  // �ҷ��� �̹��� ��� 
		IMAGE_BITMAP,  // �ҷ��� �̹��� ���� 
		0,  // ������ Ŀ�� �ʺ�
		0,  // ������ Ŀ�� ���� 
		LR_LOADFROMFILE); // ���� ��θ� ����Ͽ� �ҷ����� ( flag )

	// ������ ������ �޸� DC�� �ҷ��� ��Ʈ���� �������ش�
	// SelectObject�� �Ʒ�ó�� HGDIOBJ�� �����ϸ� ���� ������ �ִ� HGDIOBJ�� ��ȯ���ش�
	// �׷��� �� DC�� ������ ������ �ݵ�� ���� HGDIOBJ�� ������ �صΰ�
	// ������� �Ѵ�.
	Info->hPrev = (HBITMAP)SelectObject(Info->hDC, Info->hBmp);

	// ��Ʈ�� ������ ���ͼ� �������ش�
	GetObject(
		Info->hBmp, // �����ϰ��� �ϴ� GDI ������Ʈ�� �ڵ�, ��Ʈ��, �귯��, ��, ��Ʈ ���� �ڵ��� �ش�. 
		sizeof(BITMAP), // ���ۿ� ����� ������ ũ�� 
		&Info->BmpInfo); // ������Ʈ�� ������ ���Ϲ��� ����ü�� ������ , �ڵ��� Ÿ�Կ� ���� ���Ǵ� ����ü�� �޶�����.

	m_vecTextureInfo.push_back(Info);

	if (m_vecTextureInfo.size() == 1)
		m_TextureType = ETexture_Type::Atlas;
	else
		m_TextureType = ETexture_Type::Frame;

	return true;
}

bool CTexture::LoadTexture(const std::string& Name, const std::vector<std::wstring>& vecFileName, const std::string& PathName)
{
	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);
	size_t Size = vecFileName.size();

	for (size_t i = 0; i < Size; i++)
	{
		TCHAR FullPath[MAX_PATH] = {};
		if (Info)
			lstrcpy(FullPath, Info->Path);
		lstrcat(FullPath, vecFileName[i].c_str());

		if (!LoadTextureFullPath(Name, FullPath))
			return false;
	}
	if (m_vecTextureInfo.size() == 1)
		m_TextureType = ETexture_Type::Atlas;
	else
		m_TextureType = ETexture_Type::Frame;

	return true;
}

void CTexture::Render(HDC hDC, const Vector2& WindowPos, const Vector2& ImagePos, const Vector2& Size, int Index)
{
	if (m_vecTextureInfo[Index]->ColorKeyEnable)
	{
		// 1������ : ����� DC�̴�.
		// 2������ : ����� ������ �󿡼��� x��ǥ
		// 3������ : ����� ������ �󿡼��� y��ǥ
		// 4������ : ����� ������ �󿡼��� xũ��
		// 5������ : ����� ������ �󿡼��� yũ��
		// 6������ : ����� �̹����� DC�̴�.
		// 7������ : ����� �̹��� �󿡼��� ���� ��ǥ x
		// 8������ : ����� �̹��� �󿡼��� ���� ��ǥ y
		// 9������ : ����� �̹��� �󿡼��� ����ũ��
		// 10������ : ����� �̹��� �󿡼��� ����ũ��
		// 11������ : ColorKey ����
		TransparentBlt(hDC, (int)WindowPos.x, (int)WindowPos.y, (int)Size.x, (int)Size.y,
			m_vecTextureInfo[Index]->hDC, (int)ImagePos.x, (int)ImagePos.y,
			(int)Size.x, (int)Size.y, m_vecTextureInfo[Index]->ColorKey);
	}

	else
	{
		// 1������ : ����� DC�̴�.
		// 2������ : ����� ������ �󿡼��� x��ǥ
		// 3������ : ����� ������ �󿡼��� y��ǥ
		// 4������ : ����� ������ �󿡼��� xũ��
		// 5������ : ����� ������ �󿡼��� yũ��
		// 6������ : ����� �̹����� DC�̴�.
		// 7������ : ����� �̹��� �󿡼��� ���� ��ǥ x
		// 8������ : ����� �̹��� �󿡼��� ���� ��ǥ y
		BitBlt(hDC, (int)WindowPos.x, (int)WindowPos.y, (int)Size.x, (int)Size.y,
			m_vecTextureInfo[Index]->hDC, (int)ImagePos.x, (int)ImagePos.y, SRCCOPY);
	}
}
