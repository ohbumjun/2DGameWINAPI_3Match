#pragma once

#include <Windows.h>
#include <math.h>

#define	PI	3.141592f

float RadianToDegree(float Radian);
float DegreeToRadian(float Degree);

struct Vector2
{
	float	x;
	float	y;

public:
	Vector2();
	Vector2(float _x, float _y);
	Vector2(const Vector2& v);

public:
	const Vector2& operator = (const Vector2& v)
	{
		x = v.x;
		y = v.y;

		return *this;
	}

	const Vector2& operator = (const POINT& v)
	{
		x = (float)v.x;
		y = (float)v.y;

		return *this;
	}

	// +
	Vector2 operator + (const Vector2& v)	const
	{
		Vector2	result;
		result.x = x + v.x;
		result.y = y + v.y;
		return result;
	}

	Vector2 operator + (const POINT& v)	const
	{
		Vector2	result;
		result.x = x + v.x;
		result.y = y + v.y;
		return result;
	}

	Vector2 operator + (float f)	const
	{
		Vector2	result;
		result.x = x + f;
		result.y = y + f;
		return result;
	}

	Vector2 operator + (int i)	const
	{
		Vector2	result;
		result.x = x + i;
		result.y = y + i;
		return result;
	}

	// +=
	const Vector2& operator += (const Vector2& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	const Vector2& operator += (const POINT& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	const Vector2& operator += (float f)
	{
		x += f;
		y += f;
		return *this;
	}

	const Vector2& operator += (int i)
	{
		x += i;
		y += i;
		return *this;
	}

	// -
	Vector2 operator - (const Vector2& v)	const
	{
		Vector2	result;
		result.x = x - v.x;
		result.y = y - v.y;
		return result;
	}

	Vector2 operator - (const POINT& v)	const
	{
		Vector2	result;
		result.x = x - v.x;
		result.y = y - v.y;
		return result;
	}

	Vector2 operator - (float f)	const
	{
		Vector2	result;
		result.x = x - f;
		result.y = y - f;
		return result;
	}

	Vector2 operator - (int i)	const
	{
		Vector2	result;
		result.x = x - i;
		result.y = y - i;
		return result;
	}

	// -=
	const Vector2& operator -= (const Vector2& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	const Vector2& operator -= (const POINT& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	const Vector2& operator -= (float f)
	{
		x -= f;
		y -= f;
		return *this;
	}

	const Vector2& operator -= (int i)
	{
		x -= i;
		y -= i;
		return *this;
	}

	// *
	Vector2 operator * (const Vector2& v)	const
	{
		Vector2	result;
		result.x = x * v.x;
		result.y = y * v.y;
		return result;
	}

	Vector2 operator * (const POINT& v)	const
	{
		Vector2	result;
		result.x = x * v.x;
		result.y = y * v.y;
		return result;
	}

	Vector2 operator * (float f)	const
	{
		Vector2	result;
		result.x = x * f;
		result.y = y * f;
		return result;
	}

	Vector2 operator * (int i)	const
	{
		Vector2	result;
		result.x = x * i;
		result.y = y * i;
		return result;
	}

	// *=
	const Vector2& operator *= (const Vector2& v)
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}

	const Vector2& operator *= (const POINT& v)
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}

	const Vector2& operator *= (float f)
	{
		x *= f;
		y *= f;
		return *this;
	}

	const Vector2& operator *= (int i)
	{
		x *= i;
		y *= i;
		return *this;
	}

	// /
	Vector2 operator / (const Vector2& v)	const
	{
		Vector2	result;
		result.x = x / v.x;
		result.y = y / v.y;
		return result;
	}

	Vector2 operator / (const POINT& v)	const
	{
		Vector2	result;
		result.x = x / v.x;
		result.y = y / v.y;
		return result;
	}

	Vector2 operator / (float f)	const
	{
		Vector2	result;
		result.x = x / f;
		result.y = y / f;
		return result;
	}

	Vector2 operator / (int i)	const
	{
		Vector2	result;
		result.x = x / i;
		result.y = y / i;
		return result;
	}

	// /=
	const Vector2& operator /= (const Vector2& v)
	{
		x /= v.x;
		y /= v.y;
		return *this;
	}

	const Vector2& operator /= (const POINT& v)
	{
		x /= v.x;
		y /= v.y;
		return *this;
	}

	const Vector2& operator /= (float f)
	{
		x /= f;
		y /= f;
		return *this;
	}

	const Vector2& operator /= (int i)
	{
		x /= i;
		y /= i;
		return *this;
	}

	float Length()	const
	{
		return sqrtf(x * x + y * y);
	}

	// 길이를 1로 만들어준다.
	void Normalize()
	{
		float	SelfLength = Length();
		x /= SelfLength;
		y /= SelfLength;
	}
};

float Distance(const Vector2& Src, const Vector2& Dest);
float GetAngle(const Vector2& Src, const Vector2& Dest);