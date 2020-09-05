#pragma once

#include <iostream>
#include "Morpheus/Core/Common.h"

namespace Morpheus {

	struct Vector2
	{
	public:
		FLOAT x, y;

		Vector2();
		Vector2(const FLOAT& X, const FLOAT& Y);

		Vector2& Add(const Vector2& other);
		Vector2& Subtract(const Vector2& other);
		Vector2& Multiply(const Vector2& other);
		Vector2& Divide(const Vector2& other);

		friend Vector2& operator+(Vector2 Left, const Vector2& Right);
		friend Vector2& operator-(Vector2 Left, const Vector2& Right);
		friend Vector2& operator/(Vector2 Left, const Vector2& Right);
		friend Vector2& operator*(Vector2 Left, const Vector2& Right);

		bool operator==(const Vector2& Other);
		bool operator!=(const Vector2& Other);

		Vector2& operator+=(const Vector2& Other);
		Vector2& operator-=(const Vector2& Other);
		Vector2& operator*=(const Vector2& Other);
		Vector2& operator/=(const Vector2& Other);

		friend std::ostream& operator<<(std::ostream& Stream, const Vector2& Vector);
	};

}