#include "UINumberWidget.h"
#include "../Resource/Texture.h"

CUINumberWidget::CUINumberWidget() :
	m_Number(0),
	m_RenderEnable(true)
{
}

CUINumberWidget::CUINumberWidget(const CUINumberWidget& Widget) : CUIImage(Widget)
{
	m_Number = Widget.m_Number;
	m_RenderEnable = Widget.m_RenderEnable;
}

CUINumberWidget::~CUINumberWidget()
{
}

bool CUINumberWidget::Init()
{
	std::vector<std::wstring> vecNumberFileName;
	for (int i = 0; i < 10; i++)
	{
		TCHAR FileName[MAX_PATH] = {};
		wsprintf(FileName, TEXT("Number/%d.bmp"), i);
		vecNumberFileName.push_back(FileName);
	}

	SetTexture("Number", vecNumberFileName);

	return true;
}

void CUINumberWidget::Update(float DeltaTime)
{
	CUIImage::Update(DeltaTime);



	int TempNum = m_Number;
	while (TempNum > 0)
	{
		m_TempNumber.push(TempNum % 10);
		TempNum /= 10;
	}

	m_vecNumber.clear();

	while (!m_TempNumber.empty())
	{
		m_vecNumber.push_back(m_TempNumber.top());
		m_TempNumber.pop();
	}
}


void CUINumberWidget::Render(HDC hDC)
{
	if (!m_RenderEnable)
		return;
	if (m_vecNumber.empty())
	{
		// ��������� 0�� ���
		m_FrameIndex = 0;
		CUIImage::Render(hDC);
	}
	else
	{
		// ���� �ִٸ�
		size_t Size = m_vecNumber.size();

		for (size_t i = 0; i < Size; i++)
		{
			if (m_Texture->GetTextureType() == ETexture_Type::Frame)
			{
				m_FrameIndex = m_vecNumber[i];
			}
			else
			{
				m_FrameIndex = 0;
			}

			// ó������ Offset�� 0 --> �̷� Render �������� �Ѿ �� �ְ� ���ֱ�
			m_Offset.x = i * m_Size.x;
			CUIImage::Render(hDC);
		}
	}
}

void CUINumberWidget::Render(const Vector2& Pos, HDC hDC)
{
}

CUINumberWidget* CUINumberWidget::Clone()
{
	return nullptr;
}
