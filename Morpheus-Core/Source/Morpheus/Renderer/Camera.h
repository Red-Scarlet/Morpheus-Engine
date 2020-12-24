#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Mathematics/Mathematics.h"

namespace Morpheus {

	class Camera
	{
	public:
		virtual ~Camera() = default;

	public:
		const Matrix4& GetProjectionMatrix()
		{ return m_ProjectionMatrix; }
		const Matrix4& GetViewMatrix()
		{ return m_ViewMatrix; }

		virtual const Vector3& GetPosition() = 0;
		virtual const Vector3& GetRotation() = 0;

		virtual void SetPosition(const Vector3& _Position) = 0;
		virtual void SetRotation(const Vector3& _Rotation) = 0;

	protected:
		void SetProjectionMatrix(const Matrix4& _Matrix) { m_ProjectionMatrix = _Matrix; }
		void SetViewMatrix(const Matrix4& _Matrix) { m_ViewMatrix = _Matrix; }

	private:
		Matrix4 m_ProjectionMatrix = Matrix4(0.0f);
		Matrix4 m_ViewMatrix = Matrix4(0.0f);
	};

}