#pragma once

#include <iostream>
#include "Morpheus/Core/Common.h"

namespace Morpheus {

	struct Vector3
	{
	public:
		floatm x, y, z;

		Vector3();
		Vector3(const floatm& X, const floatm& Y, const floatm& Z);

		Vector3& Add(const Vector3& other);
		Vector3& Subtract(const Vector3& other);
		Vector3& Multiply(const Vector3& other);
		Vector3& Divide(const Vector3& other);

		static Vector3& Normalize(Vector3 _Left);
		static Vector3& Cross(Vector3 _Left, const Vector3& _Right);
		floatm& Dot(const Vector3& Right);
		floatm& Magnitude();

		friend Vector3& operator+(Vector3 Left, const Vector3& Right);
		friend Vector3& operator-(Vector3 Left, const Vector3& Right);
		friend Vector3& operator/(Vector3 Left, const Vector3& Right);
		friend Vector3& operator*(Vector3 Left, const Vector3& Right);
		friend Vector3& operator*(Vector3 Left, const floatm& Right);

		bool operator==(const Vector3& Other);
		bool operator!=(const Vector3& Other);

		Vector3& operator+=(const Vector3& Other);
		Vector3& operator-=(const Vector3& Other);
		Vector3& operator*=(const Vector3& Other);
		Vector3& operator/=(const Vector3& Other);

		friend std::ostream& operator<<(std::ostream& Stream, const Vector3& Vector);
	};
}