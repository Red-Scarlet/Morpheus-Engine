#include "Morppch.h"
#include "Vector2.h"

namespace Morpheus {

	Vector2::Vector2()
		: x(0.00f), y(0.00f)
	{
	}

	Vector2::Vector2(const FLOAT& X, const FLOAT& Y)
		: x(X), y(Y)
	{
	}

	Vector2& Vector2::Add(const Vector2& Other)
	{
		x += Other.x;
		y += Other.y;

		return *this;
	}

	Vector2& Vector2::Subtract(const Vector2& Other)
	{
		x -= Other.x;
		y -= Other.y;

		return *this;
	}

	Vector2& Vector2::Multiply(const Vector2& Other)
	{
		x *= Other.x;
		y *= Other.y;

		return *this;
	}

	Vector2& Vector2::Divide(const Vector2& Other)
	{
		x /= Other.x;
		y /= Other.y;

		return *this;
	}

	Vector2& operator+(Vector2 Left, const Vector2& Right)
	{
		return Left.Add(Right);
	}
	Vector2& operator-(Vector2 Left, const Vector2& Right)
	{
		return Left.Subtract(Right);
	}
	Vector2& operator*(Vector2 Left, const Vector2& Right)
	{
		return Left.Multiply(Right);
	}
	Vector2& operator/(Vector2 Left, const Vector2& Right)
	{
		return Left.Divide(Right);
	}

	Vector2& Vector2::operator+=(const Vector2& Other)
	{
		return Add(Other);
	}
	Vector2& Vector2::operator-=(const Vector2& Other)
	{
		return Subtract(Other);
	}
	Vector2& Vector2::operator*=(const Vector2& Other)
	{
		return Multiply(Other);
	}
	Vector2& Vector2::operator/=(const Vector2& Other)
	{
		return Divide(Other);
	}

	bool Vector2::operator==(const Vector2& Other)
	{
		return x == Other.x && y == Other.y;
	}

	bool Vector2::operator!=(const Vector2& Other)
	{
		return !(*this == Other);
	}

	std::ostream& operator<<(std::ostream& Stream, const Vector2& Vector)
	{
		Stream << "Vector2: (" << Vector.x << "," << Vector.y << ")";
		return Stream;
	}

}