#pragma once

#include <iostream>
#include "Morpheus/Core/Common.h"

namespace Morpheus {

	struct Boolean2
	{
	public:
		Boolean x, y;

		Boolean2();
		Boolean2(const Boolean& X, const Boolean& Y);

		Boolean& Add(const Boolean2& other);
		Boolean2& Subtract(const Boolean2& other);
		Boolean2& Multiply(const Boolean2& other);
		Boolean2& Divide(const Boolean2& other);

		friend Boolean2& operator+(Boolean2 Left, const Boolean2& Right);
		friend Boolean2& operator-(Boolean2 Left, const Boolean2& Right);
		friend Boolean2& operator/(Boolean2 Left, const Boolean2& Right);
		friend Boolean2& operator*(Boolean2 Left, const Boolean2& Right);

		bool operator==(const Boolean2& Other);
		bool operator!=(const Boolean2& Other);

		Boolean2& operator+=(const Boolean2& Other);
		Boolean2& operator-=(const Boolean2& Other);
		Boolean2& operator*=(const Boolean2& Other);
		Boolean2& operator/=(const Boolean2& Other);

		friend std::ostream& operator<<(std::ostream& Stream, const Boolean2& Vector);
	};

}