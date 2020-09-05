#pragma once

#include "Morpheus/Core/Common.h"

#include "Vector3.h"
#include "Vector4.h"

namespace Morpheus {

	struct Matrix4
	{
	public:
		union
		{
			FLOAT Elements[4 * 4];
			Vector4 Columns[4];
		};

		Matrix4();
		Matrix4(FLOAT Diagonal);
		Vector4 GetColumn(UINT32 Index);

		Matrix4 Orthographic(FLOAT Left, FLOAT Right, FLOAT Bottom, FLOAT Top, FLOAT Near, FLOAT Far);
		Matrix4 Perspective(FLOAT Fov, FLOAT AspectRatio, FLOAT Near, FLOAT Far);

		Matrix4 Translation(const Vector3& Translation);
		Matrix4 Rotation(FLOAT Angle, const Vector3& Axis);
		Matrix4 Scale(const Vector3& Scale);
		//Matrix4 Inverse(const Matrix4& Matrix);

		Matrix4& Multiply(const Matrix4& Other);
		Matrix4& Divide(const Matrix4& Other);

		Vector3 Multiply(const Vector3& Other) const;
		Vector4 Multiply(const Vector4& Other) const;

		friend Matrix4 operator*(Matrix4 Left, const Matrix4& Right);
		friend Matrix4 operator/(Matrix4 Left, const Matrix4& Right);

		friend Vector3 operator*(const Matrix4& Left, const Vector3& Right);
		friend Vector4 operator*(const Matrix4& Left, const Vector4& Right);

		Matrix4& operator*=(const Matrix4& Other);
		Matrix4& operator/=(const Matrix4& Other);
	};

}