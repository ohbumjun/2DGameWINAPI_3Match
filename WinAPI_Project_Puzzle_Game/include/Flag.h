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
	EMPTY = 0,
	BASIC = 1
};

