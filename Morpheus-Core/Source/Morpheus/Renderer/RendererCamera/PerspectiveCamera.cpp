#include "Morppch.h"
#include "PerspectiveCamera.h"

namespace Morpheus {

	PerspectiveCamera::PerspectiveCamera(const float32& _Fov, const float32& _AspectRatio)
		: m_FOV(_Fov), m_AspectRatio(_AspectRatio)
	{
		SetProjectionMatrix(glm::perspective(glm::radians(m_FOV), _AspectRatio, 0.01f, 1000.00f));
		SetViewMatrix(glm::translate(Vector3(0.00f, 0.00f, 1.00f)));
	}

	void PerspectiveCamera::SetView(const Vector3& _Position, const Vector3& _Front, const Vector3& _Up)
	{
		SetViewMatrix(glm::lookAt(_Position, _Position + _Front, _Up));
	}

	void PerspectiveCamera::SetProjection(const float32& _Fov, const float32& _AspectRatio)
	{
		SetProjectionMatrix(glm::perspective(glm::radians(m_FOV), _AspectRatio, 0.01f, 1000.00f));
	}

}