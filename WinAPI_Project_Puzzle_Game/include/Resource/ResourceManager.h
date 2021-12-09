#pragma once

#include "Texture.h"
#include "AnimationSequence.h"
// #include "Sound.h"
// #include "Font.h"

class CResourceManager
{
private:
	CResourceManager();
	~CResourceManager();
private :
	std::unordered_map<std::string, CSharedPtr<CTexture>> m_mapTexture;

public :
	bool Init();
	void Update();
public :
	// Texture -------------------------------------------------------------------------------------------------------------------------------------------------
	bool LoadTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName =  TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath);
	bool LoadTexture(const std::string& Name, const std::vector<std::wstring>& vecFileName, const std::string& PathName =  TEXTURE_PATH);
	void SetTextureColorKey(const std::string& Name,
		const unsigned char r, const unsigned char g,
		const unsigned char b, int Index = 0);
	void SetTextureColorKey(const std::string& Name,
		unsigned int ColorKey, int Index = 0);

	void ReleaseTexture(const std::string& Name);

	CTexture* FindTexture(const std::string& Name);
private :
	static CResourceManager* m_Inst;
public :
	static CResourceManager* GetInst()
	{
		if (!m_Inst)
			m_Inst = new CResourceManager;
		return m_Inst;
	}
	static void Destroy()
	{
		SAFE_DELETE(m_Inst);
	}
};

