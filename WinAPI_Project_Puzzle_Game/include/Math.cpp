#include "Math.h"
#include "GameInfo.h"


float RadianToDegree(float Radian)
{
	return Radian * 180.f / PI;
}

float DegreeToRadian(float Degree)
{
	return Degree * PI / 180.f;
}

float Distance(const Vector2& Src, const Vector2& Dest)
{
	Vector2	result = Src - Dest;

	return result.Length();
}

float GetAngle(const Vector2& Src, const Vector2& Dest)
{
	// 빗변을 구한다.
	float	c = Distance(Src, Dest);
	float	Width = Dest.x - Src.x;

	// cos0 값을 구한것이다. 그런데 여기서는 각도를 구해야 하므로
	// 이것을 cos의 역함수를 이용하여 각도로 만들어준다.
	float	Angle = Width / c;

	// acos 에 cos0 값을 넣어주면 0 를 얻어온다.
	// Radian 으로 나온다.
	Angle = RadianToDegree(acosf(Angle));

	// 만약 대상이 더 위에 있다면 360에서 각도를 제거해준다.
	if (Dest.y < Src.y)
		Angle = 360.f - Angle;

	return Angle;
}

Vector2::Vector2() :
	x(0.f),
	y(0.f)
{
}

Vector2::Vector2(float _x, float _y) :
	x(_x),
	y(_y)
{
}

Vector2::Vector2(const Vector2& v) :
	x(v.x),
	y(v.y)
{
}
