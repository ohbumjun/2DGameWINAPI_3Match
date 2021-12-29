#pragma once

enum EKeyState
{
	KeyState_Down,
	KeyState_Push,
	KeyState_Up,
	KeyState_Max
};

enum class ETexture_Type
{
	Atlas,
	Frame
};

enum class CellType
{
	EMPTY    =  0,
	BASIC    =  1,
	FIXTURE =  2,
	JELLY      = 3
};

enum class BlockType
{
	EMPTY = 0, // �� ��, ���� �������� �ʴ� ��ġ�� ����
	BASIC = 1  // �⺻��, ���������� 6������ ���� ĳ���� ��������Ʈ�� ��µȴ�
};

enum  AnimalType
{
	PANDA = 0,
	BEAR,
	PARROT , // red bird 
 	ELEPHANT,
	PENGUIN,
	DUCK, // green bird
	END
};

enum class EButton_State
{
	Normal,
	MouseOn,
	Click,
	Disable,
	End
};