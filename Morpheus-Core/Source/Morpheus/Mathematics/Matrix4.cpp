#include "Morppch.h"
#include "Matrix4.h"

namespace Morpheus {

	Matrix4::Matrix4()
	{
		for (UINT i = 0; i < (4 * 4); i++)
			Elements[i] = 0.00f;
	}

	Matrix4::Matrix4(FLOAT Diagonal)
	{
		for (uint32 i = 0; i < (4 * 4); i++)
			Elements[i] = 0.0f;

		Elements[0 + 0 * 4] = Diagonal;
		Elements[1 + 1 * 4] = Diagonal;
		Elements[2 + 2 * 4] = Diagonal;
		Elements[3 + 3 * 4] = Diagonal;
	}

	Vector4 Matrix4::GetColumn(UINT Index)
	{
		Index *= 4;
		return Vector4(Elements[Index], Elements[Index + 1], Elements[Index + 2], Elements[Index + 3]);
	}

	Matrix4& Matrix4::Orthographic(FLOAT Left, FLOAT Right, FLOAT Bottom, FLOAT Top, FLOAT Near, FLOAT Far)
	{
		Matrix4 result(1.0f);

		result.Elements[0 + 0 * 4] = 2.0f / (Right - Left);			   
		result.Elements[1 + 1 * 4] = 2.0f / (Top - Bottom);	   
		result.Elements[2 + 2 * 4] = 2.0f / (Near - Far);
			   
		result.Elements[0 + 3 * 4] = (Left + Right) / (Left - Right);
		result.Elements[1 + 3 * 4] = (Bottom + Top) / (Bottom - Top);
		result.Elements[2 + 3 * 4] = (Far + Near) / (Far - Near);

		return result;
	}

	Matrix4& Matrix4::Perspective(FLOAT Fov, FLOAT AspectRatio, FLOAT Near, FLOAT Far)
	{
		Matrix4 result(1.0f);

		float y_scale = (1.0f / tan((Fov / 2) * (3.142 / 180))) * AspectRatio;
		float x_scale = (y_scale / AspectRatio);
		float frustum_length = (Far / Near);

		float b = -((Far + Near) / frustum_length);
		float c = -((2.0f * Near * Far) / frustum_length);

		result.Elements[0 + 0 * 4] = x_scale;
		result.Elements[1 + 1 * 4] = y_scale;
		result.Elements[2 + 2 * 4] = b;
		result.Elements[3 + 2 * 4] = -1.0f;
		result.Elements[2 + 3 * 4] = c;
		result.Elements[3 + 3 * 4] = 0;

		return result;
	}

	Matrix4& Matrix4::Translation(const Vector3& Translation)
	{
		Matrix4 result(1.0f);

		result.Elements[0 + 3 * 4] = Translation.x;
		result.Elements[1 + 3 * 4] = Translation.y;
		result.Elements[2 + 3 * 4] = Translation.z;

		return result;
	}

	Matrix4& Matrix4::Rotation(FLOAT Angle, const Vector3& Axis)
	{
		Matrix4 result(1.0f);

		float r = (Angle * (3.142 / 180));
		float c = cos(r);
		float s = sin(r);
		float omc = 1.0f - c;

		float x = Axis.x;
		float y = Axis.y;
		float z = Axis.z;

		result.Elements[0 + 0 * 4] = x * x * omc + c;
		result.Elements[0 + 1 * 4] = y * x * omc + z * s;
		result.Elements[0 + 2 * 4] = x * z * omc - y * s;
			   
		result.Elements[1 + 0 * 4] = x * y * omc - z * s;
		result.Elements[1 + 1 * 4] = y * y * omc + c;
		result.Elements[1 + 2 * 4] = y * z * omc + x * s;
			   
		result.Elements[2 + 0 * 4] = x * z * omc + y * s;
		result.Elements[2 + 1 * 4] = y * z * omc - x * s;
		result.Elements[2 + 2 * 4] = z * z * omc + c;

		return result;
	}

	Matrix4& Matrix4::Scale(const Vector3& Scale)
	{
		Matrix4 result(1.0f);

		result.Elements[0 + 0 * 4] = Scale.x;
		result.Elements[1 + 1 * 4] = Scale.y;
		result.Elements[2 + 2 * 4] = Scale.z;

		return result;
	}

	Matrix4& Matrix4::Multiply(const Matrix4& Other)
	{
		FLOAT data[16];
		for (UINT y = 0; y < 4; y++)
			for (UINT x = 0; x < 4; x++) {
				FLOAT sum = 0.0f;
				for (UINT e = 0; e < 4; e++)
					sum += Elements[x + e * 4] * Other.Elements[e + y * 4];
				data[x + y * 4] = sum;
			}
		memcpy(Elements, data, 4 * 4 * sizeof(FLOAT));

		return *this;
	}

	Matrix4& Matrix4::Divide(const Matrix4& Other)
	{
		FLOAT data[16];
		for (UINT y = 0; y < 4; y++)
			for (UINT x = 0; x < 4; x++) {
				FLOAT sum = 0.0f;
				for (UINT e = 0; e < 4; e++)
					sum += Elements[x + e * 4] / Other.Elements[e + y * 4];
				data[x + y * 4] = sum;
			}
		memcpy(Elements, data, 4 * 4 * sizeof(FLOAT));

		return *this;
	}

	Vector3 Matrix4::Multiply(const Vector3& Other) const
	{
		return Vector3(
			Columns[0].x * Other.x + Columns[1].x * Other.y + Columns[2].x * Other.z + Columns[3].x,
			Columns[0].y * Other.x + Columns[1].y * Other.y + Columns[2].y * Other.z + Columns[3].y,
			Columns[0].z * Other.x + Columns[1].z * Other.y + Columns[2].z * Other.z + Columns[3].z
		);
	}

	Vector4 Matrix4::Multiply(const Vector4& Other) const
	{
		return Vector4(
			Columns[0].x * Other.x + Columns[1].x * Other.y + Columns[2].x * Other.z + Columns[3].x * Other.w,
			Columns[0].y * Other.x + Columns[1].y * Other.y + Columns[2].y * Other.z + Columns[3].y * Other.w,
			Columns[0].z * Other.x + Columns[1].z * Other.y + Columns[2].z * Other.z + Columns[3].z * Other.w,
			Columns[0].w * Other.x + Columns[1].w * Other.y + Columns[2].w * Other.z + Columns[3].w * Other.w
		);
	}

	Matrix4& Matrix4::operator*=(const Matrix4& Other)
	{
		return Multiply(Other);
	}

	Matrix4& Matrix4::operator/=(const Matrix4& Other)
	{
		return Divide(Other);
	}
	
	Matrix4 operator*(Matrix4 Left, const Matrix4& Right)
	{
		return Left.Multiply(Right);
	}

	Matrix4 operator/(Matrix4 Left, const Matrix4& Right)
	{
		return Left.Divide(Right);
	}

	Vector3 operator*(const Matrix4& Left, const Vector3& Right)
	{
		return Left.Multiply(Right);
	}

	Vector4 operator*(const Matrix4& Left, const Vector4& Right)
	{
		return Left.Multiply(Right);
	}


}