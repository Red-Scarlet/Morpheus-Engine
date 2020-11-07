#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Mathematics/Mathematics.h"

//#include "Camera.h"
//#include "RendererCore/Texture.h"

namespace Morpheus {

	struct Camera
	{
	public:
		Matrix4 ProjectionMatrix;
		Matrix4 ViewMatrix;
	};

	class Renderer2D 
	{
	public:
		static void Init();
		static void Shutdown();

		//static void BeginScene();
		static void BeginScene(Camera& _Camera);
		static void EndScene();
		static void Flush();

	public:
		static void DrawQuad(const Vector2& _Position, const Vector2& _Size, const Vector4& _Color);	//2D
		static void DrawQuad(const Vector3& _Position, const Vector2& _Size, const Vector4& _Color);	//3D

		//static void DrawQuad(const Mathematics::Vec2& _Position, const Mathematics::Vector2& _Size, const Ref<Texture2D>& _Texture);	//T2D
		//static void DrawQuad(const Mathematics::Vec3& _Position, const Mathematics::Vector2& _Size, const Ref<Texture2D>& _Texture);	//T3D

		//static void DrawQuad(const Mathematics::Vec2& _Position, const Mathematics::Vector2& _Size, const Ref<Texture2D>& _Texture, const Mathematics::Vector4& _Color);	//T2D
		//static void DrawQuad(const Mathematics::Vec3& _Position, const Mathematics::Vector2& _Size, const Ref<Texture2D>& _Texture, const Mathematics::Vector4& _Color);	//T3D

		//static void DrawRotatingQuad(const Mathematics::Vector2& _Position, const float32& _Rotate, const Mathematics::Vector2& _Size, const Ref<Texture2D>& _Texture);	//T2D
		//static void DrawRotatingQuad(const Mathematics::Vec3& _Position, const float32& _Rotate, const Mathematics::Vector2& _Size, const Ref<Texture2D>& _Texture);	//T3D

		//static void DrawRotatingQuad(const Mathematics::Vector2& _Position, const float32& _Rotate, const Mathematics::Vector2& _Size, const Ref<Texture2D>& _Texture, const Mathematics::Vector4& _Color);	//T2D
		//static void DrawRotatingQuad(const Mathematics::Vec3& _Position, const float32& _Rotate, const Mathematics::Vector2& _Size, const Ref<Texture2D>& _Texture, const Mathematics::Vector4& _Color);	//T3D

		//static void DrawRotatingQuad(const Mathematics::Vector2& _Position, const float32& _Rotate, const Mathematics::Vector2& _Size, const Mathematics::Vec4& _Color);	//T2D
		//static void DrawRotatingQuad(const Mathematics::Vec3& _Position, const float32& _Rotate, const Mathematics::Vector2& _Size, const Mathematics::Vec4& _Color);	//T3D

	public:
		struct Statistics
		{
			uint32 DrawCalls = 0; uint32 QuadCount = 0;
			uint32 GetTotalVertexCount() { return QuadCount * 4; }
			uint32 GetTotalIndexCount() { return QuadCount * 6; }
		};

		static void ResetStats();
		static Statistics GetStats();

	private:
		static void FlushAndReset();
		 
	};

}