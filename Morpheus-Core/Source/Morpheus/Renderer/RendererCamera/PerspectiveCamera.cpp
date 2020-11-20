#include "Morppch.h"
#include "PerspectiveCamera.h"

namespace Morpheus {

	PerspectiveCamera::PerspectiveCamera(const float32& _Fov, const float32& _AspectRatio)
		: m_FOV(_Fov), m_AspectRatio(_AspectRatio)
	{
		SetProjectionMatrix(Matrix4::Perspective(m_FOV, _AspectRatio, 0.01f, 1000.00f));
		SetViewMatrix(Matrix4::Translate(Vector3(0.00f, 0.00f, -2.00f)));
	}

	void PerspectiveCamera::SetView(const Vector3& _Position, const Vector3& _Front, const Vector3& _Up)
	{
		SetViewMatrix(Matrix4::LookAt(_Position, _Position + _Front, _Up));
	}

	void PerspectiveCamera::SetProjection(const float32& _Fov, const float32& _AspectRatio)
	{
		SetProjectionMatrix(Matrix4::Perspective(m_FOV, _AspectRatio, 0.01f, 1000.00f));
	}

}