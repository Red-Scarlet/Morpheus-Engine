#include "Morppch.h"
#include "Quaternion.h"

namespace Morpheus {

	Quaternion::Quaternion()
		: Scalar(0.00f), Vector(0.00f, 0.00f, 0.00f)
	{
	}

	Quaternion::Quaternion(floatm SCALAR, Vector3 VECTOR)
		: Scalar(SCALAR), Vector(VECTOR)
	{
	}

	Quaternion& Quaternion::Add(const Quaternion& Other)
	{
		floatm scalar = Scalar + Other.Scalar;
		Vector3 imaginary = Vector + Other.Vector;

		return Quaternion(scalar, imaginary);
	}

	Quaternion& Quaternion::Subtract(const Quaternion& Other)
	{
		floatm scalar = Scalar - Other.Scalar;
		Vector3 imaginary = Vector - Other.Vector;

		return Quaternion(scalar, imaginary);
	}

	Quaternion& Quaternion::Multiply(const Quaternion& Other)
	{
		floatm scalar = Scalar * Other.Scalar - Vector.Dot(Other.Vector);

		Vector3 A = Other.Vector;
		A.Multiply({ Scalar, Scalar, Scalar });
		
		Vector3 B = Vector;
		B.Multiply({ Other.Scalar, Other.Scalar, Other.Scalar });

		Vector3 imaginary = A + B + Vector.Cross(Other.Vector);

		return Quaternion(scalar, imaginary);
	}

	Quaternion& operator+(Quaternion Left, const Quaternion& Right)
	{
		return Left.Add(Right);
	}
	Quaternion& operator-(Quaternion Left, const Quaternion& Right)
	{
		return Left.Subtract(Right);
	}
	Quaternion& operator*(Quaternion Left, const Quaternion& Right)
	{
		return Left.Multiply(Right);
	}

	Quaternion& Quaternion::operator+=(const Quaternion& Other)
	{
		return Add(Other);
	}

	Quaternion& Quaternion::operator-=(const Quaternion& Other)
	{
		return Subtract(Other);
	}

	Quaternion& Quaternion::operator*=(const Quaternion& Other)
	{
		return Multiply(Other);
	}

	bool Quaternion::operator==(const Quaternion& Other)
	{
		return Scalar == Other.Scalar && Vector == Other.Vector;
	}

	bool Quaternion::operator!=(const Quaternion& Other)
	{
		return !(*this == Other);
	}

}