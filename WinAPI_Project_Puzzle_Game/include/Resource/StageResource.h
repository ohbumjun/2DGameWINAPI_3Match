#pragma once

#include "../GameInfo.h"

#include "../Resource/Texture.h"
// #include "../Animation/AnimationInfo.h"
#include "../Resource/AnimationSequence.h"

class CStageResource
{
	friend class CStage;
public :
	CStageResource();
	~CStageResource();

private :
	std::unordered_map<std::string, CSharedPtr<CTexture>>	m_mapTexture;
	std::unordered_map<std::string, CSharedPtr<CAnimationSequence>>	m_mapAnimationSequence;

public:
	bool LoadTexture(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath);
	bool LoadTexture(const std::string& Name, const std::vector<std::wstring>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	void SetTextureColorKey(const std::string& Name,
		const unsigned char r, const unsigned char g,
		const unsigned char b, int Index = 0);
	void SetTextureColorKey(const std::string& Name,
		unsigned int ColorKey, int Index = 0);

	CTexture* FindTexture(const std::string& Name);

public :
	bool CreateAnimationSequence(const std::string& SequenceName, const std::string& TextureName);
	bool CreateAnimationSequence(const std::string& SequenceName, const std::string& TextureName,
		const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
	bool CreateAnimationSequence(const std::string& SequenceName, const std::string& TextureName,
		const std::vector<std::wstring>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	void AddAnimationFrameData(const std::string& SequenceName, const Vector2& StartPos, const Vector2& Size);
	void AddAnimationFrameData(const std::string& SequenceName, float PosX, float PosY, float SizeX, float SizeY);

	CAnimationSequence* FindAnimationSequence(const std::string& Name);


};

