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

	// 대문자로 만들기
	_strupr_s(FullPathMultibyte);

	int PathLength = (int)strlen(FullPathMultibyte);

	// 끝에서부터 처음으로 가면서 탐색
	// ex)
	// D:/Project/Bin/Texture/Test.bmp
	// D:/Project/Bin/Texture/Player/Test.bmp
	for (int i = PathLength - 1; i >= 0; i--)
	{
		if (FullPathMultibyte[i] == '\\')
		{
			// 폴더 이름 : /Texture --> // 뒤에 null문자까지 해서 총 9개
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
				// Texture  //  이후부터 복사해서 FileName에 넣을 것이다 
				lstrcpy(FileName, &FullPath[i + 1]);
			}
		}
	}

	TextureInfo* Info = new TextureInfo;
	lstrcpy(Info->FileName, FileName);

	// 메모리 DC를 만든다
	Info->hDC = CreateCompatibleDC(CGameManager::GetInst()->GetWindowDC());

	// 비트맵 속성으로 파일을 불러오고 HBTMAP으로 형변환해서 hImage에 저장한다.
	Info->hBmp = (HBITMAP)LoadImage(
		CGameManager::GetInst()->GetWindowInstance(), // 블러올 이미지를 가지고 있는 실행 파일 모듈의 핸들 (프로세스의 핸들)
		FullPath,  // 불러올 이미지 경로 
		IMAGE_BITMAP,  // 불러올 이미지 종류 
		0,  // 아이콘 커서 너비
		0,  // 아이콘 커너 높이 
		LR_LOADFROMFILE); // 파일 경로를 사용하여 불러오기 ( flag )

	// 위에서 생성한 메모리 DC에 불러온 비트맵을 지정해준다
	// SelectObject는 아래처럼 HGDIOBJ를 지정하면 원래 가지고 있던 HGDIOBJ를 반환해준다
	// 그래서 이 DC를 지워줄 때에는 반드시 원본 HGDIOBJ로 지정을 해두고
	// 지워줘야 한다.
	Info->hPrev = (HBITMAP)SelectObject(Info->hDC, Info->hBmp);

	// 비트맵 정보를 얻어와서 저장해준다
	GetObject(
		Info->hBmp, // 조사하고자 하는 GDI 오브젝트의 핸들, 비트맵, 브러시, 팬, 폰트 등의 핸들을 준다. 
		sizeof(BITMAP), // 버퍼에 기록할 정보의 크기 
		&Info->BmpInfo); // 오브젝트의 정보를 리턴받을 구조체의 포인터 , 핸들의 타입에 따라 사용되는 구조체가 달라진다.

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
		// 1번인자 : 출력할 DC이다.
		// 2번인자 : 출력할 윈도우 상에서의 x좌표
		// 3번인자 : 출력할 윈도우 상에서의 y좌표
		// 4번인자 : 출력할 윈도우 상에서의 x크기
		// 5번인자 : 출력할 윈도우 상에서의 y크기
		// 6번인자 : 출력할 이미지의 DC이다.
		// 7번인자 : 출력할 이미지 상에서의 시작 좌표 x
		// 8번인자 : 출력할 이미지 상에서의 시작 좌표 y
		// 9번인자 : 출력할 이미지 상에서의 가로크기
		// 10번인자 : 출력할 이미지 상에서의 세로크기
		// 11번인자 : ColorKey 지정
		TransparentBlt(hDC, (int)WindowPos.x, (int)WindowPos.y, (int)Size.x, (int)Size.y,
			m_vecTextureInfo[Index]->hDC, (int)ImagePos.x, (int)ImagePos.y,
			(int)Size.x, (int)Size.y, m_vecTextureInfo[Index]->ColorKey);
	}

	else
	{
		// 1번인자 : 출력할 DC이다.
		// 2번인자 : 출력할 윈도우 상에서의 x좌표
		// 3번인자 : 출력할 윈도우 상에서의 y좌표
		// 4번인자 : 출력할 윈도우 상에서의 x크기
		// 5번인자 : 출력할 윈도우 상에서의 y크기
		// 6번인자 : 출력할 이미지의 DC이다.
		// 7번인자 : 출력할 이미지 상에서의 시작 좌표 x
		// 8번인자 : 출력할 이미지 상에서의 시작 좌표 y
		BitBlt(hDC, (int)WindowPos.x, (int)WindowPos.y, (int)Size.x, (int)Size.y,
			m_vecTextureInfo[Index]->hDC, (int)ImagePos.x, (int)ImagePos.y, SRCCOPY);
	}
}
