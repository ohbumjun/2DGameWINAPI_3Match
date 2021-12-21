#include "Board.h"
#include "Cell.h"
#include "../GameManager.h"

CCell::CCell() :
	m_CellType(CellType::BASIC),
	m_BlockType(BlockType::BASIC),
	m_AnimalType(AnimalType::PANDA),
	m_MoveSpeed(10.f),
	m_Dir(Vector2(0.f, 1.f)),
	m_TimeScale(1.f),
	m_Board(nullptr),
	m_AnimalNames{"Panda","Bear","Parrot","Elephant","Penguin","Duck"},
	m_IsMoving(false),
	m_NewPos{},
	m_IsSwapping(false)
{
}

CCell::CCell(const CCell& Cell)
{
	m_CellType = Cell.m_CellType;
	m_AnimalType = Cell.m_AnimalType;
	m_Dir = Cell.m_Dir;;
	m_TimeScale = Cell.m_TimeScale;
	m_MoveSpeed = Cell.m_MoveSpeed;	
	if (Cell.m_Board)
		m_Board = Cell.m_Board;
}

CCell::~CCell()
{
}

void CCell::Start()
{
	CGameObject::Start();

	AddAnimation(m_AnimalNames[m_AnimalType], true, 1.f);
	// SetCurrentAnimation(m_AnimalNames[m_AnimalType]);
}

bool CCell::Init(AnimalType Type)
{
	if (!CGameObject::Init())
		return false;

	m_AnimalType = Type;

	CreateAnimation();
	return true;
}

void CCell::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	// Check Block State
	m_BlockType = m_Board->GetBlock(m_RowIndex, m_ColIndex)->GetBlockType();

	if (m_BlockType == BlockType::EMPTY)
	{
		Destroy();
	}

	if (m_IsSwapping) // �ٲ�� ���̶��, 
		return;
	else // �ٲ�� ���� �ƴ϶��, 
		m_IsMoving = false;
	
	// ���߰� �� ��������� �Ѵ�. 
	// �б�ó���� ��� ����� �ϴ°� 
	// ���� �߿��� ����, ���콺 Ŭ������ �̵��ϴ� �ֵ�, �׳� �Ʒ��� ��
	// �������� �ֵ��� �и��ؼ� �۾��ؾ� �Ѵ�.
	// ���� , Ŭ���ϸ� �����༭, �ֵ��� ��������. 
	// ����, ���� Dir�� 0,0 �̸� ���� ���� 
	
	// �Ʒ� �̵� 
	if (m_Pos.y < m_NewPos.y)
	{
		Move(Vector2(0.f, 1.f));
		m_IsMoving = true;
	}
	else
	{
		// �ڱ� ���� �̵� ���� üũ --> �̵� �Ұ����ϴٸ�, �ڱⰡ ���� Block �� ���¸� Basic���� �ٲ��ش�.
		m_Board->GetBlock(m_RowIndex, m_ColIndex)->SetBlockType(BlockType::BASIC);
		m_IsMoving = false;
		// m_Dir = Vector2(0.f, 0.f);
	}
}

void CCell::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);

}

void CCell::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

CCell* CCell::Clone()
{
	return new CCell(*this);
}

bool CCell::Move(const Vector2& Dir)
{
	// if (Dir.x == 0.f && Dir.y == 0.f)
	//		return false;
		
	Vector2 CurrentMove = Dir * m_MoveSpeed * CGameManager::GetInst()->GetDeltaTime() * m_TimeScale;
	m_Pos += CurrentMove;
	return true;
}

bool CCell::Move(const Vector2& Dir, float Speed)
{
	// if (Dir.x == 0.f && Dir.y == 0.f)
	//		return false;
	Vector2 CurrentMove = Dir * Speed * CGameManager::GetInst()->GetDeltaTime() * m_TimeScale;
	m_Pos += CurrentMove;
	return true;
}
