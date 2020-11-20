#pragma once

#include "Morpheus/Core/Common.h"
#include "Matrix4.h"
#include "Quaternion.h"
#include "Vector3.h"

namespace Morpheus {

	struct Spatial
	{
	public:
		Vector3 Position;
		Quaternion Rotation;
		Vector3 Scale;

		void Process() 
		{ 
			m_Matrix = Matrix4::Translate(Position) * Matrix4::Scale(Scale) * Matrix4::Rotation(Rotation);
			//m_Updated = false; 
		}

		const Matrix4& GetMatrix()	// Use operator= and check if that matrix differs from this one.
		{
			//if (m_Updated)
			Process();
			return m_Matrix;
		}

	private:
		Matrix4 m_Matrix;
		//Boolean m_Updated = false;
	};
}