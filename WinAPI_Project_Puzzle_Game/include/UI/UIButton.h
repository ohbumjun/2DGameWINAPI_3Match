#include "UIWidget.h"

class CUIButton : public CUIWidget {
public:
	CUIButton();
	CUIButton(const CUIButton& Button);
	~CUIButton();
private:
	EButton_State m_ButtonState;
	AnimationFrameData m_FrameData[(int)EButton_State::End];
	std::function<void()> m_ButtonClickFunction;
	std::function<void()> m_ButtonMouseOnFunction;
	CSharedPtr<class CTexture> m_Texture;
public:
	void SetTexture(class CTexture* Texture);
	void SetTexture(const std::string& Name);
	void SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
	void SetTextureFullPath(const std::string& Name, const TCHAR* FullPath);
	void SetTextureColorKey(unsigned char r, unsigned char g, unsigned char b, int Index = 0);
	void Enable(bool Enable)
	{
		m_ButtonState = Enable ? EButton_State::Normal : EButton_State::Disable;
	}
public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CUIButton* Clone();
	virtual void Render(HDC hDC);
	virtual void Render(const Vector2& Pos, HDC hDC);
public :
	virtual void CollisionMouseHoveredCallback(float DeltaTime);
	virtual void CallMouseReleaseCallback(float DeltaTime);
public:
	template<typename T>
	void SetButtonClickFunction(T* Obj, void(T::* Func)())
	{
		m_ButtonClickFunction = std::bind(Func, Obj);
	}
	template<typename T>
	void SetButtonOnFunction(T* Obj, void(T::* Func)())
	{
		m_ButtonMouseOnFunction = std::bind(Func, Obj);
	}
};

