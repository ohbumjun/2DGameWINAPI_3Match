#pragma once

#define _CRTDBG_MAP_ALLOC

#include <Windows.h>
#include <stdlib.h>
#include <crtdbg.h>
#include <list>	  
#include <vector> 
#include <stack>
#include <string>
#include <unordered_map>
#include <functional>
#include "resource.h"

// Header
#include "SharedPtr.h"
#include "Math.h"
#include "Flag.h"

// Sound 
// #include "fmod.hpp"
#pragma comment(lib, "msimg32.lib")
// #pragma comment(lib, "fmod64_vc.lib")

// Const 
#define GRAVITY 9.8f
#define M_PI 3.14f
#define KEYCOUNT_MAX 256

// Path
#define ROOT_PATH "RootPath"
#define TEXTURE_PATH "TexturePath"
#define SOUND_PATH "SoundPath"
#define FONT_PATH "FontPath"
#define MAP_PATH "MapPath"

// Delete
#define SAFE_DELETE(p) \
	if (p)             \
	{                  \
		delete p;      \
		p = nullptr;   \
	}

#define SAFE_DELETE_ARRAY(p) \
	if (p)                   \
	{                        \
		delete[] p;          \
		p = nullptr;         \
	}

#define SAFE_RELEASE(p) \
	if (p)              \
	{                   \
		p->Release();   \
		p = nullptr;    \
	}

// Classes
struct Resolution
{
	int Width;
	int Height;
};

struct AnimationFrameData
{
	Vector2 StartPos;
	Vector2 Size;
};