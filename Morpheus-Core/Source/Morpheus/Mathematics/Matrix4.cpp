#include "Morppch.h"
#include "Matrix4.h"

#include "MathematicsFunc.h"

namespace Morpheus {

	Matrix4::Matrix4()
	{
		for (uint32 i = 0; i < (4 * 4); i++)
			Elements[i] = 0.00f;
	}

	Matrix4::Matrix4(floatm Diagonal)
	{
		for (uint32 i = 0; i < (4 * 4); i++)
			Elements[i] = 0.0f;

		Elements[0 + 0 * 4] = Diagonal;
		Elements[1 + 1 * 4] = Diagonal;
		Elements[2 + 2 * 4] = Diagonal;
		Elements[3 + 3 * 4] = Diagonal;
	}

	Vector4 Matrix4::GetColumn(uint32 Index)
	{
		Index *= 4;
		return Vector4(Elements[Index], Elements[Index + 1], Elements[Index + 2], Elements[Index + 3]);
	}

	Matrix4 Matrix4::Orthographic(floatm Left, floatm Right, floatm Bottom, floatm Top, floatm Near, floatm Far)
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

	Matrix4 Matrix4::Perspective(floatm Fov, floatm AspectRatio, floatm Near, floatm Far)
	{
		Matrix4 result(1.00f);

		floatm y_scale = (1.00f / tan(ToRadians(Fov / 2.00f))) * AspectRatio;
		floatm x_scale = (y_scale / AspectRatio);
		floatm frustum_length = (Far / Near);
		
		floatm b = -((Far + Near) / frustum_length);
		floatm c = -((2.0f * Near * Far) / frustum_length);
		
		result.Elements[0 + 0 * 4] = x_scale;
		result.Elements[1 + 1 * 4] = y_scale;
		result.Elements[2 + 2 * 4] = b;
		result.Elements[3 + 2 * 4] = -1.0f;
		result.Elements[2 + 3 * 4] = c;
		result.Elements[3 + 3 * 4] = 0;

		return result;
	}

	Matrix4 Matrix4::LookAt(const Vector3& Camera, const Vector3& Object, const Vector3& Up)
	{
		Matrix4 result = Matrix4(1.00f);

		Vector3 f = Vector3::Normalize(Object - Camera);
		Vector3 s = Vector3::Cross(f, Vector3::Normalize(Up));
		Vector3 u = Vector3::Cross(s, f);
		
		//Vector3 n = Vector3::Normalize(Up);
		//Vector3 s = Vector3::Cross(f, n);
		//Vector3 u = Vector3::Cross(s, f);

		result.Elements[0 + 0 * 4] = s.x;
		result.Elements[0 + 1 * 4] = s.y;
		result.Elements[0 + 2 * 4] = s.z;

		result.Elements[1 + 0 * 4] = u.x;
		result.Elements[1 + 1 * 4] = u.y;
		result.Elements[1 + 2 * 4] = u.z;

		result.Elements[2 + 0 * 4] = -f.x;
		result.Elements[2 + 1 * 4] = -f.y;
		result.Elements[2 + 2 * 4] = -f.z;

		return result * Translate(Vector3(-Camera.x, -Camera.y, -Camera.z));
	}

	Matrix4 Matrix4::Translate(const Vector3& Translation)
	{
		Matrix4 result(1.0f);

		result.Elements[0 + 3 * 4] = Translation.x;
		result.Elements[1 + 3 * 4] = Translation.y;
		result.Elements[2 + 3 * 4] = Translation.z;

		return result;
	}

	Matrix4 Matrix4::Rotation(const floatm& Angle, const Vector3& Axis)
	{
		Matrix4 result(1.0f);

		floatm r = ToRadians(Angle);
		floatm c = cos(r);
		floatm s = sin(r);
		floatm omc = 1.0f - c;
			 
		floatm x = Axis.x;
		floatm y = Axis.y;
		floatm z = Axis.z;

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

	Matrix4 Matrix4::Rotation(const Quaternion& Quat)
	{
		Matrix4 result(1.0f);

		floatm qx, qy, qz, qw, qx2, qy2, qz2, qxqx2, qyqy2, qzqz2, qxqy2, qyqz2, qzqw2, qxqz2, qyqw2, qxqw2;
		qx = Quat.Scalar;
		qy = Quat.Vector.x;
		qz = Quat.Vector.y;
		qw = Quat.Vector.z; 

		//qx = Quat.Vector.x;
		//qy = Quat.Vector.y;
		//qz = Quat.Vector.z;
		//qw = Quat.Scalar;

		qx2 = (qx + qx);
		qy2 = (qy + qy);
		qz2 = (qz + qz);
		qxqx2 = (qx * qx2);
		qxqy2 = (qx * qy2);
		qxqz2 = (qx * qz2);
		qxqw2 = (qw * qx2);
		qyqy2 = (qy * qy2);
		qyqz2 = (qy * qz2);
		qyqw2 = (qw * qy2);
		qzqz2 = (qz * qz2);
		qzqw2 = (qw * qz2);

		result.Columns[0] = Vector4(((1.0f - qyqy2) - qzqz2), (qxqy2 - qzqw2), (qxqz2 + qyqw2), 0.0f);
		result.Columns[1] = Vector4((qxqy2 + qzqw2), ((1.0f - qxqx2) - qzqz2), (qyqz2 - qxqw2), 0.0f);
		result.Columns[2] = Vector4((qxqz2 - qyqw2), (qyqz2 + qxqw2), ((1.0f - qxqx2) - qyqy2), 0.0f);
		return result;
	}

	Matrix4 Matrix4::Scale(const Vector3& Scale)
	{
		Matrix4 result(1.0f);

		result.Elements[0 + 0 * 4] = Scale.x;
		result.Elements[1 + 1 * 4] = Scale.y;
		result.Elements[2 + 2 * 4] = Scale.z;

		return result;
	}

	Matrix4 Matrix4::Inverse(const Matrix4& Matrix)
	{
		Matrix4 result = Matrix;
		return result.Invert();
	}

	Matrix4& Matrix4::Multiply(const Matrix4& Other)
	{
		floatm data[16];
		for (uint32 y = 0; y < 4; y++)
			for (uint32 x = 0; x < 4; x++) {
				floatm sum = 0.0f;
				for (uint32 e = 0; e < 4; e++)
					sum += Elements[x + e * 4] * Other.Elements[e + y * 4];
				data[x + y * 4] = sum;
			}
		memcpy(Elements, data, 4 * 4 * sizeof(floatm));

		return *this;
	}

	Matrix4& Matrix4::Divide(const Matrix4& Other)
	{
		floatm data[16];
		for (uint32 y = 0; y < 4; y++)
			for (uint32 x = 0; x < 4; x++) {
				floatm sum = 0.0f;
				for (uint32 e = 0; e < 4; e++)
					sum += Elements[x + e * 4] / Other.Elements[e + y * 4];
				data[x + y * 4] = sum;
			}
		memcpy(Elements, data, 4 * 4 * sizeof(floatm));

		return *this;
	}

	Matrix4& Matrix4::Invert()
	{
		float temp[16];

		temp[0] = Elements[5] * Elements[10] * Elements[15] -
			Elements[5] * Elements[11] * Elements[14] -
			Elements[9] * Elements[6] * Elements[15] +
			Elements[9] * Elements[7] * Elements[14] +
			Elements[13] * Elements[6] * Elements[11] -
			Elements[13] * Elements[7] * Elements[10];

		temp[4] = -Elements[4] * Elements[10] * Elements[15] +
			Elements[4] * Elements[11] * Elements[14] +
			Elements[8] * Elements[6] * Elements[15] -
			Elements[8] * Elements[7] * Elements[14] -
			Elements[12] * Elements[6] * Elements[11] +
			Elements[12] * Elements[7] * Elements[10];

		temp[8] = Elements[4] * Elements[9] * Elements[15] -
			Elements[4] * Elements[11] * Elements[13] -
			Elements[8] * Elements[5] * Elements[15] +
			Elements[8] * Elements[7] * Elements[13] +
			Elements[12] * Elements[5] * Elements[11] -
			Elements[12] * Elements[7] * Elements[9];

		temp[12] = -Elements[4] * Elements[9] * Elements[14] +
			Elements[4] * Elements[10] * Elements[13] +
			Elements[8] * Elements[5] * Elements[14] -
			Elements[8] * Elements[6] * Elements[13] -
			Elements[12] * Elements[5] * Elements[10] +
			Elements[12] * Elements[6] * Elements[9];

		temp[1] = -Elements[1] * Elements[10] * Elements[15] +
			Elements[1] * Elements[11] * Elements[14] +
			Elements[9] * Elements[2] * Elements[15] -
			Elements[9] * Elements[3] * Elements[14] -
			Elements[13] * Elements[2] * Elements[11] +
			Elements[13] * Elements[3] * Elements[10];

		temp[5] = Elements[0] * Elements[10] * Elements[15] -
			Elements[0] * Elements[11] * Elements[14] -
			Elements[8] * Elements[2] * Elements[15] +
			Elements[8] * Elements[3] * Elements[14] +
			Elements[12] * Elements[2] * Elements[11] -
			Elements[12] * Elements[3] * Elements[10];

		temp[9] = -Elements[0] * Elements[9] * Elements[15] +
			Elements[0] * Elements[11] * Elements[13] +
			Elements[8] * Elements[1] * Elements[15] -
			Elements[8] * Elements[3] * Elements[13] -
			Elements[12] * Elements[1] * Elements[11] +
			Elements[12] * Elements[3] * Elements[9];

		temp[13] = Elements[0] * Elements[9] * Elements[14] -
			Elements[0] * Elements[10] * Elements[13] -
			Elements[8] * Elements[1] * Elements[14] +
			Elements[8] * Elements[2] * Elements[13] +
			Elements[12] * Elements[1] * Elements[10] -
			Elements[12] * Elements[2] * Elements[9];

		temp[2] = Elements[1] * Elements[6] * Elements[15] -
			Elements[1] * Elements[7] * Elements[14] -
			Elements[5] * Elements[2] * Elements[15] +
			Elements[5] * Elements[3] * Elements[14] +
			Elements[13] * Elements[2] * Elements[7] -
			Elements[13] * Elements[3] * Elements[6];

		temp[6] = -Elements[0] * Elements[6] * Elements[15] +
			Elements[0] * Elements[7] * Elements[14] +
			Elements[4] * Elements[2] * Elements[15] -
			Elements[4] * Elements[3] * Elements[14] -
			Elements[12] * Elements[2] * Elements[7] +
			Elements[12] * Elements[3] * Elements[6];

		temp[10] = Elements[0] * Elements[5] * Elements[15] -
			Elements[0] * Elements[7] * Elements[13] -
			Elements[4] * Elements[1] * Elements[15] +
			Elements[4] * Elements[3] * Elements[13] +
			Elements[12] * Elements[1] * Elements[7] -
			Elements[12] * Elements[3] * Elements[5];

		temp[14] = -Elements[0] * Elements[5] * Elements[14] +
			Elements[0] * Elements[6] * Elements[13] +
			Elements[4] * Elements[1] * Elements[14] -
			Elements[4] * Elements[2] * Elements[13] -
			Elements[12] * Elements[1] * Elements[6] +
			Elements[12] * Elements[2] * Elements[5];

		temp[3] = -Elements[1] * Elements[6] * Elements[11] +
			Elements[1] * Elements[7] * Elements[10] +
			Elements[5] * Elements[2] * Elements[11] -
			Elements[5] * Elements[3] * Elements[10] -
			Elements[9] * Elements[2] * Elements[7] +
			Elements[9] * Elements[3] * Elements[6];

		temp[7] = Elements[0] * Elements[6] * Elements[11] -
			Elements[0] * Elements[7] * Elements[10] -
			Elements[4] * Elements[2] * Elements[11] +
			Elements[4] * Elements[3] * Elements[10] +
			Elements[8] * Elements[2] * Elements[7] -
			Elements[8] * Elements[3] * Elements[6];

		temp[11] = -Elements[0] * Elements[5] * Elements[11] +
			Elements[0] * Elements[7] * Elements[9] +
			Elements[4] * Elements[1] * Elements[11] -
			Elements[4] * Elements[3] * Elements[9] -
			Elements[8] * Elements[1] * Elements[7] +
			Elements[8] * Elements[3] * Elements[5];

		temp[15] = Elements[0] * Elements[5] * Elements[10] -
			Elements[0] * Elements[6] * Elements[9] -
			Elements[4] * Elements[1] * Elements[10] +
			Elements[4] * Elements[2] * Elements[9] +
			Elements[8] * Elements[1] * Elements[6] -
			Elements[8] * Elements[2] * Elements[5];

		floatm determinant = Elements[0] * temp[0] + Elements[1] * temp[4] + Elements[2] * temp[8] + Elements[3] * temp[12];
		determinant = 1.0f / determinant;
		
		for (uint32 i = 0; i < 4 * 4; i++)
			Elements[i] = temp[i] * determinant;

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

	String Matrix4::ToString() const
	{
		std::stringstream result;
		result << "Matrix4: (" << Columns[0].x << ", " << Columns[1].x << ", " << Columns[2].x << ", " << Columns[3].x << "), ";
		result << "(" << Columns[0].y << ", " << Columns[1].y << ", " << Columns[2].y << ", " << Columns[3].y << "), ";
		result << "(" << Columns[0].z << ", " << Columns[1].z << ", " << Columns[2].z << ", " << Columns[3].z << "), ";
		result << "(" << Columns[0].w << ", " << Columns[1].w << ", " << Columns[2].w << ", " << Columns[3].w << ")";
		return result.str();
	}

}