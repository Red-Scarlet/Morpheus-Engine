#include "Morppch.h"
#include "Vector4.h"

namespace Morpheus {

	Vector4::Vector4()
		: x(0.00f), y(0.00f), z(0.00f), w(0.00f)
	{
	}

	Vector4::Vector4(FLOAT X, FLOAT Y, FLOAT Z, FLOAT W)
		: x(X), y(Y), z(Z), w(W)
	{
	}

	Vector4& Vector4::Add(const Vector4& Other)
	{
		x += Other.x;
		y += Other.y;
		z += Other.z;
		w += Other.w;

		return *this;
	}

	Vector4& Vector4::Subtract(const Vector4& Other)
	{
		x -= Other.x;
		y -= Other.y;
		z -= Other.z;
		w -= Other.w;

		return *this;
	}

	Vector4& Vector4::Multiply(const Vector4& Other)
	{
		x *= Other.x;
		y *= Other.y;
		z *= Other.z;
		w *= Other.w;

		return *this;
	}

	Vector4& Vector4::Divide(const Vector4& Other)
	{
		x /= Other.x;
		y /= Other.y;
		z /= Other.z;
		w /= Other.w;

		return *this;
	}

	bool Vector4::operator==(const Vector4& Other)
	{
		return x == Other.x && y == Other.y && z == Other.z && w == Other.w;
	}

	bool Vector4::operator!=(const Vector4& Other)
	{
		return !(*this == Other);
	}

	Vector4& Vector4::operator+=(const Vector4& Other)
	{
		return Add(Other);
	}

	Vector4& Vector4::operator-=(const Vector4& Other)
	{
		return Subtract(Other);
	}

	Vector4& Vector4::operator*=(const Vector4& Other)
	{
		return Multiply(Other);
	}

	Vector4& Vector4::operator/=(const Vector4& Other)
	{
		return Divide(Other);
	}

	Vector4& operator+(Vector4 Left, const Vector4& Right)
	{
		return Left.Add(Right);
	}

	Vector4& operator-(Vector4 Left, const Vector4& Right)
	{
		return Left.Subtract(Right);
	}

	Vector4& operator*(Vector4 Left, const Vector4& Right)
	{
		return Left.Multiply(Right);
	}

	Vector4& operator/(Vector4 Left, const Vector4& Right)
	{
		return Left.Divide(Right);
	}

	std::ostream& operator<<(std::ostream& Stream, const Vector4& Vector)
	{
		Stream << "Vector4: (" << Vector.x << ", " << Vector.y << ", " << Vector.z << ", " << Vector.w << ")";
		return Stream;
	}

}