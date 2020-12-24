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
		Vector3 Rotation;
		Vector3 Scale;

		const Matrix4& GetMatrix()
		{
			m_Matrix = glm::translate(Position) * glm::scale(Scale);
			return m_Matrix;
		}

	private:
		Matrix4 m_Matrix;
	};
}