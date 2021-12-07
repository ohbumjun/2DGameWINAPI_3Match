
#include "Texture.h"
// #include "../Scene/Scene.h"
// #include "../Scene/SceneResource.h"
#include "../Path/PathManager.h"
#include "../GameManager.h"

CTexture::CTexture() :
	m_TextureType(ETexture_Type::Atlas)
{
}

CTexture::~CTexture()
{
	size_t	Size = m_vecTextureInfo.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_vecTextureInfo[i]);
	}
}

bool CTexture::LoadTexture(const std::string& Name, const TCHAR* FileName,
	const std::string& PathName)
{
	// 최종 경로를 만들어준다.
	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	TCHAR	FullPath[MAX_PATH] = {};

	if (Info)
		lstrcpy(FullPath, Info->Path);
	lstrcat(FullPath, FileName);

	return LoadTextureFullPath(Name, FullPath);
}

bool CTexture::LoadTextureFullPath(const std::string& Name,
	const TCHAR* FullPath)
{
	SetName(Name);

	// FullPath에서, Texture File Name과 Folder 경로까지 구분하는 코드
	// 즉,FileName 부분만을 얻어온다
	TCHAR FileName[MAX_PATH] = {};

	char FullPathMultibyte[MAX_PATH] = {};

#ifdef UNICODE
	// 유니코드라면, 멀티바이트로 변환해준다
	// 유니코드 문자열을 멀티바이트 문자열로 변환한다.
	int ConvertLength = WideCharToMultiByte(CP_ACP, 0,
		FullPath, -1, nullptr, 0, 0, 0);

	// FullPath을 FullPathMultibyte에 변환해서 넣어라 
	WideCharToMultiByte(CP_ACP, 0, FullPath, -1,
		FullPathMultibyte, ConvertLength, 0, 0);
#else
	strcpy_s(FullPathMultibyte, FullPath);
#endif 

	// 대소문자 구분없이, 전부 대문자로 만들어서 사용
	_strupr_s(FullPathMultibyte);

	int PathLength = (int)strlen(FullPathMultibyte);

	// 끝에서부터 처음으로 가면서 탐색 
	for (int i = PathLength - 1; i >= 0; i--)
	{
		// ex)
		// D:/Project/Bin/Texture/Test.bmp
		// D:/Project/Bin/Texture/Player/Test.bmp
		if (FullPathMultibyte[i] == '\\') // Texture 다음에 나오는 것
		{
			// 폴더 이름 : /Texture
			char FolderName[9] = {}; // 뒤에 null문자까지 해서 총 9개
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
				// Texture // 이후 부터 복사해서
				// FileName에 넣을 것이다 
				lstrcpy(FileName, &FullPath[i + 1]);
			}
		}
	}
	TextureInfo* Info = new TextureInfo;
	lstrcpy(Info->FileName, FileName);

	// 메모리DC를 만든다.
	Info->hDC = CreateCompatibleDC(CGameManager::GetInst()->GetWindowDC());
	Info->hBmp = (HBITMAP)LoadImage(CGameManager::GetInst()->GetWindowInstance(),
		FullPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	// 위에서 생성한 메모리 DC에 불러온 비트맵을 지정해준다.
	// SelectObject는 아래처럼 HGDIOBJ을 지정하면 원래 가지고 있던
	// HGDIOBJ를 반환해준다.
	// 그래서 이 DC를 지워줄때에는 반드시 원본 HGDIOBJ로 지정을 해두고
	// 지워줘야 한다.
	Info->hPrev = (HBITMAP)SelectObject(Info->hDC, Info->hBmp);

	// 비트맵 정보를 얻어와서 저장해둔다.
	GetObject(Info->hBmp, sizeof(BITMAP), &Info->BmpInfo);

	m_vecTextureInfo.push_back(Info);

	if (m_vecTextureInfo.size() == 1)
		m_TextureType = ETexture_Type::Atlas;

	else
		m_TextureType = ETexture_Type::Frame;

	return true;
}

bool CTexture::LoadTexture(const std::string& Name,
	const std::vector<std::wstring>& vecFileName,
	const std::string& PathName)
{
	// 최종 경로를 만들어준다.
	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	size_t	Size = vecFileName.size();

	for (size_t i = 0; i < Size; ++i)
	{
		TCHAR	FullPath[MAX_PATH] = {};

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

void CTexture::Render(HDC hDC, const Vector2& WindowPos, const Vector2& ImgPos, const Vector2& Size, int Index)
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
			m_vecTextureInfo[Index]->hDC, (int)ImgPos.x, (int)ImgPos.y,
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
			m_vecTextureInfo[Index]->hDC, (int)ImgPos.x, (int)ImgPos.y, SRCCOPY);
	}
}

/*
void CTexture::Save(FILE* pFile)
{
	int Length = (int)m_Name.length();

	fwrite(&Length, sizeof(int), 1, pFile);
	fwrite(m_Name.c_str(), sizeof(char), Length, pFile);

	fwrite(&m_TextureType, sizeof(int), 1, pFile);

	// Texture Info가 몇개 있느냐
	int Count = (int)m_vecTextureInfo.size();
	fwrite(&Count, sizeof(int), 1, pFile);

	for (int i = 0; i < Count; i++)
	{
		fwrite(&m_vecTextureInfo[i]->ColorKeyEnable, sizeof(bool), 1, pFile);
		fwrite(&m_vecTextureInfo[i]->ColorKey, sizeof(unsigned int), 1, pFile);

		int NameCount = lstrlen(m_vecTextureInfo[i]->FileName);
		fwrite(&NameCount, sizeof(int), 1, pFile);
		fwrite(&m_vecTextureInfo[i]->FileName, sizeof(TCHAR), NameCount, pFile);
	}
}

void CTexture::Load(FILE* pFile)
{
	int Length = 0;
	char Name[256] = {};

	fread(&Length, sizeof(int), 1, pFile);
	fread(Name, sizeof(char), Length, pFile);

	m_Name = Name;

	fread(&m_TextureType, sizeof(int), 1, pFile);

	// Texture Info가 몇개 있느냐
	int Count = 0;
	fread(&Count, sizeof(int), 1, pFile);

	if (Count == 1)
	{
		bool ColorKeyEnable = false;
		unsigned int ColorKey = 0;

		fread(&ColorKeyEnable, sizeof(bool), 1, pFile);
		fread(&ColorKey, sizeof(unsigned int), 1, pFile);

		int NameCount = 0;
		fread(&NameCount, sizeof(int), 1, pFile);

		TCHAR FileName[MAX_PATH] = {};
		fread(&FileName, sizeof(TCHAR), NameCount, pFile);

		// Texture Load
		LoadTexture(m_Name, FileName);
		if (ColorKeyEnable)
		{
			SetColorKey(ColorKey);
		}
	}
	else // vector<wstring> 형태로 가져와야 한다 
	{
		std::vector<std::wstring> vecFileName;
		std::vector<bool> vecColorkeyEnable;
		std::vector<unsigned int> vecColorkey;

		for (int i = 0; i < Count; i++)
		{
			bool ColorKeyEnable = false;
			unsigned int ColorKey = 0;

			fread(&ColorKeyEnable, sizeof(bool), 1, pFile);
			fread(&ColorKey, sizeof(unsigned int), 1, pFile);

			vecColorkeyEnable.push_back(ColorKeyEnable);
			vecColorkey.push_back(ColorKey);

			int NameCount = 0;
			fread(&NameCount, sizeof(int), 1, pFile);

			TCHAR FileName[MAX_PATH] = {};
			fread(&FileName, sizeof(TCHAR), NameCount, pFile);

			vecFileName.push_back(FileName);
		}

		// Texture Load
		LoadTexture(m_Name, vecFileName);

		// 전체 반복 돌면서 SetColorKey
		for (int i = 0; i < Count; i++)
		{
			if (!vecColorkeyEnable[i]) continue;
			SetColorKey(vecColorkey[i], i);
		}
	}
}
*/

/*
CTexture* CTexture::LoadStatic(FILE* pFile, CScene* Scene)
{
	CTexture* Result = nullptr;

	int Length = 0;
	char Name[256] = {};

	fread(&Length, sizeof(int), 1, pFile);
	fread(Name, sizeof(char), Length, pFile);

	ETexture_Type TextureType;
	fread(&TextureType, sizeof(ETexture_Type), 1, pFile);

	// Texture Info가 몇개 있느냐
	int Count = 0;
	fread(&Count, sizeof(int), 1, pFile);

	if (Count == 1)
	{
		bool ColorKeyEnable = false;
		unsigned int ColorKey = 0;

		fread(&ColorKeyEnable, sizeof(bool), 1, pFile);
		fread(&ColorKey, sizeof(unsigned int), 1, pFile);

		int NameCount = 0;
		fread(&NameCount, sizeof(int), 1, pFile);

		TCHAR FileName[MAX_PATH] = {};
		fread(&FileName, sizeof(TCHAR), NameCount, pFile);

		// Texture Load
		
		Stage->GetSceneResource()->LoadTexture(Name, FileName);
		if (ColorKeyEnable)
		{
			Stage->GetSceneResource()->SetTextureColorKey(Name, ColorKey);
		}
		Result = Stage->GetSceneResource()->FindTexture(Name);
		
	}
	else // vector<wstring> 형태로 가져와야 한다 
	{
		bool ColorKeyEnable = false;
		unsigned int ColorKey = 0;

		std::vector<std::wstring> vecFileName;
		std::vector<bool> vecColorkeyEnable;
		std::vector<unsigned int> vecColorkey;

		for (int i = 0; i < Count; i++)
		{
			bool ColorKeyEnable = false;
			unsigned int ColorKey = 0;

			fread(&ColorKeyEnable, sizeof(bool), 1, pFile);
			fread(&ColorKey, sizeof(unsigned int), 1, pFile);

			vecColorkeyEnable.push_back(ColorKeyEnable);
			vecColorkey.push_back(ColorKey);

			int NameCount = 0;
			fread(&NameCount, sizeof(int), 1, pFile);

			TCHAR FileName[MAX_PATH] = {};
			fread(&FileName, sizeof(TCHAR), NameCount, pFile);

			vecFileName.push_back(FileName);
		}

		// Texture Load
		
		Scene->GetSceneResource()->LoadTexture(Name, vecFileName);

		// 전체 반복 돌면서 SetColorKey
		for (int i = 0; i < Count; i++)
		{
			if (!vecColorkeyEnable[i]) continue;
			Scene->GetSceneResource()->SetTextureColorKey(Name, vecColorkey[i]);
		}
		Result = Scene->GetSceneResource()->FindTexture(Name);
		
	}
	return Result;
}

*/