#pragma once

#include "Morpheus/Core/Common.h"

#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"

namespace Morpheus {

	struct Matrix4
	{
	public:
		union
		{
			floatm Elements[4 * 4];
			Vector4 Columns[4];
		};

		Matrix4();
		Matrix4(floatm Diagonal);
		Vector4 GetColumn(uint32 Index);

		static Matrix4 Orthographic(floatm Left, floatm Right, floatm Bottom, floatm Top, floatm Near, floatm Far);
		static Matrix4 Perspective(floatm Fov, floatm AspectRatio, floatm Near, floatm Far);
		static Matrix4 LookAt(const Vector3& Camera, const Vector3& Object, const Vector3& Up);
		static Matrix4 Translate(const Vector3& Translation);
		static Matrix4 Rotation(const floatm& Angle, const Vector3& Axis);
		static Matrix4 Rotation(const Quaternion& Quad);
		static Matrix4 Scale(const Vector3& Scale);
		static Matrix4 Inverse(const Matrix4& Matrix);

		Matrix4& Multiply(const Matrix4& Other);
		Matrix4& Divide(const Matrix4& Other);
		Matrix4& Invert();

		Vector3 Multiply(const Vector3& Other) const;
		Vector4 Multiply(const Vector4& Other) const;

		friend Matrix4 operator*(Matrix4 Left, const Matrix4& Right);
		friend Matrix4 operator/(Matrix4 Left, const Matrix4& Right);

		friend Vector3 operator*(const Matrix4& Left, const Vector3& Right);
		friend Vector4 operator*(const Matrix4& Left, const Vector4& Right);

		Matrix4& operator*=(const Matrix4& Other);
		Matrix4& operator/=(const Matrix4& Other);

		String ToString() const;
	};

}