#pragma once

#include "Morpheus/Core/Common.h"
#include "Vector3.h"

namespace Morpheus {

	struct Quaternion
	{
	public:
		FLOAT Scalar; 
		Vector3 Vector;

		Quaternion();
		Quaternion(FLOAT SCALAR, Vector3 VECTOR);

		Quaternion& Add(const Quaternion& Other);
		Quaternion& Subtract(const Quaternion& Other);
		Quaternion& Multiply(const Quaternion& Other);

		friend Quaternion& operator+(Quaternion Left, const Quaternion& Right);
		friend Quaternion& operator-(Quaternion Left, const Quaternion& Right);
		friend Quaternion& operator*(Quaternion Left, const Quaternion& Right);

		bool operator==(const Quaternion& Other);
		bool operator!=(const Quaternion& Other);

		Quaternion& operator+=(const Quaternion& Other);
		Quaternion& operator-=(const Quaternion& Other);
		Quaternion& operator*=(const Quaternion& Other);

	};

}