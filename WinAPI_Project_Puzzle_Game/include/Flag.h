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
	EMPTY = 0, // 빈 블럭, 블럭이 존재하지 않는 위치에 설정
	BASIC = 1  // 기본형, 세부종류로 6가지의 동물 캐릭터 스프라이트가 출력된다
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