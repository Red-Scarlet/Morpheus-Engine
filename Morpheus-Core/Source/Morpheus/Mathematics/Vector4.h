#pragma once

#include "Morpheus/Core/Common.h"

namespace Morpheus {

	struct Vector4
	{
	public:
		floatm x, y, z, w;

		Vector4();
		Vector4(floatm X, floatm Y, floatm Z, floatm W);

		Vector4& Add(const Vector4& Other);
		Vector4& Subtract(const Vector4& Other);
		Vector4& Multiply(const Vector4& Other);
		Vector4& Divide(const Vector4& Other);

		friend Vector4& operator+(Vector4 Left, const Vector4& Right);
		friend Vector4& operator-(Vector4 Left, const Vector4& Right);
		friend Vector4& operator/(Vector4 Left, const Vector4& Right);
		friend Vector4& operator*(Vector4 Left, const Vector4& Right);

		bool operator==(const Vector4& Other);
		bool operator!=(const Vector4& Other);

		Vector4& operator+=(const Vector4& Other);
		Vector4& operator-=(const Vector4& Other);
		Vector4& operator*=(const Vector4& Other);
		Vector4& operator/=(const Vector4& Other);

		friend std::ostream& operator<<(std::ostream& Stream, const Vector4& Vector);
	};
}