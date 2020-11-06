#include "Morppch.h"
#include "Vector3.h"

namespace Morpheus {

	Vector3::Vector3()
		: x(0.00f), y(0.00f), z(0.00f)
	{
	}

	Vector3::Vector3(const floatm& X, const floatm& Y, const floatm& Z)
		: x(X), y(Y), z(Z)
	{
	}

	Vector3& Vector3::Add(const Vector3& Other)
	{
		x += Other.x;
		y += Other.y;
		z += Other.z;

		return *this;
	}

	Vector3& Vector3::Subtract(const Vector3& Other)
	{
		x -= Other.x;
		y -= Other.y;
		z -= Other.z;

		return *this;
	}

	Vector3& Vector3::Multiply(const Vector3& Other)
	{
		x *= Other.x;
		y *= Other.y;
		z *= Other.z;

		return *this;
	}

	Vector3& Vector3::Divide(const Vector3& Other)
	{
		x /= Other.x;
		y /= Other.y;
		z /= Other.z;

		return *this;
	}

	Vector3& Vector3::Normalize()
	{
		floatm magnitude = std::sqrt(x * x + y * y + z * z);

		if (magnitude > 0.0f) {

			floatm normal = 1.0f / magnitude;

			x = x * normal;
			y = y * normal;
			z = z * normal;
		}

		return *this;
	}

	Vector3& Vector3::Cross(const Vector3& Other)
	{
		Vector3 cross;

		cross.x = y * Other.z - z * Other.y;
		cross.y = z * Other.x - x * Other.z;
		cross.z = x * Other.y - y * Other.x;

		return cross;
	}

	floatm& Vector3::Dot(const Vector3& Other)
	{
		floatm dot;
		dot = x * Other.x + y * Other.y + z * Other.z;
		return dot;
	}

	floatm& Vector3::Magnitude()
	{
		floatm magnitude;
		magnitude = std::sqrt(x * x + y * y + z * z);
		return magnitude;
	}

	bool Vector3::operator==(const Vector3& Other)
	{
		return x == Other.x && y == Other.y && z == Other.z;
	}

	bool Vector3::operator!=(const Vector3& Other)
	{
		return !(*this == Other);
	}

	Vector3& Vector3::operator+=(const Vector3& Other)
	{
		return Add(Other);
	}

	Vector3& Vector3::operator-=(const Vector3& Other)
	{
		return Subtract(Other);
	}

	Vector3& Vector3::operator*=(const Vector3& Other)
	{
		return Multiply(Other);
	}

	Vector3& Vector3::operator/=(const Vector3& Other)
	{
		return Divide(Other);
	}

	Vector3& operator+(Vector3 Left, const Vector3& Right)
	{
		return Left.Add(Right);
	}

	Vector3& operator-(Vector3 Left, const Vector3& Right)
	{
		return Left.Subtract(Right);
	}

	Vector3& operator*(Vector3 Left, const Vector3& Right)
	{
		return Left.Multiply(Right);
	}

	Vector3& operator/(Vector3 Left, const Vector3& Right)
	{
		return Left.Divide(Right);
	}

	std::ostream& operator<<(std::ostream& Stream, const Vector3& Vector)
	{
		Stream << "Vector3: (" << Vector.x << ", " << Vector.y << ", " << Vector.z << ")";
		return Stream;
	}

}