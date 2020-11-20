#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Renderer/Camera.h"

namespace Morpheus {

	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(const float32& _Fov, const float32& _AspectRatio);
		virtual ~PerspectiveCamera() = default;

		void SetView(const Vector3& _Position, const Vector3& _Front, const Vector3& _Up);
		void SetProjection(const float32& _Fov, const float32& _AspectRatio);

		virtual const Vector3& GetPosition() override
		{ return m_Position; }
		virtual const Vector3& GetRotation() override
		{ return m_Rotation; }

		virtual void SetPosition(const Vector3& _Position) override
		{ m_Position = _Position; }
		virtual void SetRotation(const Vector3& _Rotation) override
		{ m_Rotation = _Rotation; }

	private:
		Vector3 m_Position = { 0.0f, 0.0f, 0.0f };
		Vector3 m_Rotation = { 0.0f, 0.0f, 0.0f };

		float32 m_FOV;
		float32 m_AspectRatio;
	};

}