#pragma once

#include "Morpheus/Core/Common.h"
#include "RendererCore/RenderCommand.h"
#include "Morpheus/Mathematics/Mathematics.h"

#include "RendererResources/VertexBuffer.h"
#include "RendererResources/IndexBuffer.h"
#include "RendererResources/UniformBuffer.h"

#include "RendererUnknown/FrameBuffer.h"
#include "RendererUnknown/Renderpass.h"

#include "RendererBindables/VertexArray.h"
#include "RendererBindables/Shader.h"

namespace Morpheus {

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		//static void DrawVertexBuffer(const Ref<VertexArray>& _VertexArray);
		//static void DrawQuad();
		//static void DrawTexturedQuad(const Ref<Texture2D> _Texture);
		//static void DrawTexturedQuad(const Ref<Texture2D> _FirstTexture, const Ref<Texture2D> _SecondTexture);

		static void OnWindowResize(uint32 _Width, uint32 _Height);

	private:
		//struct SceneData { glm::mat4 ProjectionMatrix; glm::mat4 ViewMatrix; Ref<VertexArray> m_VertexQuad; Ref<Shader> m_Shader; };
		//static SceneData* s_SceneData;
	};

	//class Renderer
	//{
	//public:
	//	static void CreateGeomerty()
	//	{
	//		VertexData Data[4] =
	//		{
	//			{ { 0.25f,  0.25f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
	//			{ { -0.25f,  0.25f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
	//			{ { -0.25f, -0.25f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
	//			{ { 0.25f, -0.25f, 0.0f }, { 1.0f, 0.0f, 1.0f } }
	//		};
	//
	//		//VertexData Data[3] =
	//		//{
	//		//	{ { 0.25f,  0.25f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
	//		//	{ { -0.25f,  0.25f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
	//		//	{ { 0.0f, -0.25f, 0.0f }, { 0.0f, 0.0f, 1.0f } }
	//		//};		
	//
	//		//uint32 IndexBufferData[3] = { 0, 1, 2 };
	//		uint32 IndexBufferData[6] = { 0, 1, 2, 0, 2, 3 };
	//
	//		BufferLayout LayoutData1 = {
	//			{ UniformDataType::Mat4, "ProjectionMatrix",  s_RenderData->ProjectionMatrix },
	//			{ UniformDataType::Mat4, "ViewMatrix",  s_RenderData->ViewMatrix },
	//			{ UniformDataType::Mat4, "TransformMatrix",  s_RenderData->TransformMatrix }
	//		};
	//
	//		Matrix4 Broken = Matrix4(1.00f);
	//		Broken.Translate(Vector3(0.0f, 0.0f, 1.0f));
	//
	//		s_RenderData->VertexArray1 = VertexArray::Create();
	//		s_RenderData->VertexArray1->SetVertexBuffer(VertexBuffer::Create(Data, sizeof(Data) / sizeof(VertexData)));
	//		s_RenderData->VertexArray1->SetIndexBuffer(IndexBuffer::Create(IndexBufferData, sizeof(IndexBufferData) / sizeof(uint32)));
	//	
	//		s_RenderData->UniformBuffer1 = UniformBuffer::Create(LayoutData1);
	//		s_RenderData->VertexArray1->SetUniformBuffer(s_RenderData->UniformBuffer1);
	//
	//
	//		// 2
	//		s_RenderData->VertexArray2 = VertexArray::Create();
	//		s_RenderData->VertexArray2->SetVertexBuffer(VertexBuffer::Create(Data, sizeof(Data) / sizeof(VertexData)));
	//		s_RenderData->VertexArray2->SetIndexBuffer(IndexBuffer::Create(IndexBufferData, sizeof(IndexBufferData) / sizeof(uint32)));
	//
	//		BufferLayout LayoutData2 = {
	//			{ UniformDataType::Mat4, "ProjectionMatrix",  s_RenderData->ProjectionMatrix },
	//			{ UniformDataType::Mat4, "ViewMatrix",  s_RenderData->ViewMatrix },
	//			{ UniformDataType::Mat4, "TransformMatrix",  s_RenderData->TransformMatrix2 }
	//		};
	//
	//		s_RenderData->UniformBuffer2 = UniformBuffer::Create(LayoutData2);
	//		s_RenderData->VertexArray2->SetUniformBuffer(s_RenderData->UniformBuffer2);
	//
	//		BufferLayout LayoutData3 = {
	//			{ UniformDataType::Mat4, "ProjectionMatrix",  s_RenderData->ProjectionMatrix },
	//			{ UniformDataType::Mat4, "ViewMatrix",  s_RenderData->ViewMatrix },
	//			{ UniformDataType::Mat4, "TransformMatrix",  s_RenderData->TransformMatrix3 }
	//		};
	//
	//		s_RenderData->VertexArray3 = VertexArray::Create();
	//		s_RenderData->VertexArray3->SetVertexBuffer(VertexBuffer::Create(Data, sizeof(Data) / sizeof(VertexData)));
	//		s_RenderData->VertexArray3->SetIndexBuffer(IndexBuffer::Create(IndexBufferData, sizeof(IndexBufferData) / sizeof(uint32)));
	//
	//		s_RenderData->UniformBuffer3 = UniformBuffer::Create(LayoutData3);
	//		s_RenderData->VertexArray3->SetUniformBuffer(s_RenderData->UniformBuffer3);
	//	}
	//
	//	static void UpdateBasicCamera()
	//	{
	//		Matrix4 T1 = Matrix4(1.00f);
	//		T1 *= Matrix4::Translate(Vector3(0.50f, 0.00f, -0.50f));
	//		T1 *= Matrix4::Rotation(s_RenderData->RotationValue, Vector3(0.00f, 0.00f, 1.00f));
	//
	//		s_RenderData->TransformMatrix = T1;
	//
	//		Matrix4 T2 = Matrix4(1.00f);
	//		T2 *= Matrix4::Translate(Vector3(-0.50f, 0.00f, -0.50f));
	//		T2 *= Matrix4::Rotation(s_RenderData->RotationValue, Vector3(0.00f, 0.00f, 1.00f));
	//
	//		s_RenderData->TransformMatrix2 = T2;
	//	}
	//	
	//	static void* GetFrameImage(uint32 id)
	//	{
	//		return s_RenderData->FrameBuffer->GetImage(id);
	//	}
	//
	//	//LaDELA
	//	static void Init()
	//	{
	//		RenderCommand::Init();
	//		s_RenderData = new RenderData;
	//
	//		RenderpassLayout RPLayout = {
	//			{ RenderpassTypes::ATTACHMENT_COLOR, RenderpassAttachment::ATTACHMENT_CLEAR, RenderpassAttachment::ATTACHMENT_STORE },
	//			{ RenderpassTypes::ATTACHMENT_DEPTH, RenderpassAttachment::ATTACHMENT_CLEAR, RenderpassAttachment::ATTACHMENT_DONTCARE }
	//		};
	//
	//		s_RenderData->Renderpass = Renderpass::Create(RPLayout);
	//		s_RenderData->FrameBuffer = FrameBuffer::Create(s_RenderData->Renderpass);
	//
	//		float32 zoom = -2.5f;
	//		s_RenderData->ProjectionMatrix = Matrix4::Perspective(45.0f, (float32)1280.0f / (float32)720.0f, 0.01f, 1024.0f);;
	//		s_RenderData->ViewMatrix = Matrix4::Translate(Vector3(0.0f, 0.0f, zoom));
	//		s_RenderData->TransformMatrix = Matrix4::Translate(Vector3(0.5f, 0.0f, 0.00f));
	//		s_RenderData->TransformMatrix2 = Matrix4::Translate(Vector3(-0.5f, 0.0f, 0.00f));
	//
	//		UpdateBasicCamera();
	//		s_RenderData->Shader = Shader::Create("Assets/uniform.vert.spv", "Assets/uniform.frag.spv");
	//
	//		CreateGeomerty();
	//		{
	//			s_RenderData->Shader->Bind();
	//			s_RenderData->VertexArray1->Bind();
	//			s_RenderData->VertexArray2->Bind();
	//			s_RenderData->VertexArray3->Bind();
	//		}
	//
	//		RenderCommand::SetClearColor({ 0.15f, 0.15f, 0.15f, 1.0f});
	//	}
	//
	//	static void Shutdown()
	//	{
	//		RenderCommand::Shutdown();
	//	}
	//
	//	static void BeginScene()
	//	{
	//		// Loops
	//		s_RenderData->BreakValue += 0.00005f;
	//		s_RenderData->RotationValue += 0.05;
	//
	//		UpdateBasicCamera();
	//
	//		// Update Uniform Buffer (Update Shader)!
	//		BufferLayout Layout = {
	//			{ UniformDataType::Mat4, "ProjectionMatrix",  s_RenderData->ProjectionMatrix },
	//			{ UniformDataType::Mat4, "ViewMatrix",  s_RenderData->ViewMatrix },
	//			{ UniformDataType::Mat4, "TransformMatrix",  s_RenderData->TransformMatrix }
	//		};
	//
	//		BufferLayout Layout2 = {
	//			{ UniformDataType::Mat4, "ProjectionMatrix",  s_RenderData->ProjectionMatrix },
	//			{ UniformDataType::Mat4, "ViewMatrix",  s_RenderData->ViewMatrix },
	//			{ UniformDataType::Mat4, "TransformMatrix",  s_RenderData->TransformMatrix2 }
	//		};
	//
	//		s_RenderData->UniformBuffer1->SetLayout(Layout);
	//		s_RenderData->UniformBuffer2->SetLayout(Layout2);
	//
	//	}
	//
	//	static void EndScene()
	//	{
	//		// Loops
	//		RenderCommand::Flush();
	//	}
	//
	//private:
	//	struct RenderData 
	//	{
	//	public:
	//		Ref<VertexArray> VertexArray1;
	//		Ref<VertexArray> VertexArray2;
	//		Ref<VertexArray> VertexArray3;
	//
	//		Ref<UniformBuffer> UniformBuffer1;
	//		Ref<UniformBuffer> UniformBuffer2;
	//		Ref<UniformBuffer> UniformBuffer3;
	//
	//		Ref<FrameBuffer> FrameBuffer;
	//		Ref<Renderpass> Renderpass;
	//		Ref<Shader> Shader;
	//
	//		Matrix4 ProjectionMatrix = Matrix4(1.00f);
	//		Matrix4 ViewMatrix = Matrix4(1.00f);
	//		Matrix4 TransformMatrix = Matrix4(1.00f);
	//		Matrix4 TransformMatrix2 = Matrix4(1.00f);
	//		Matrix4 TransformMatrix3 = Matrix4(1.00f);
	//
	//		float32 BreakValue = 1.25f;
	//		float32 RotationValue = 0.00f;
	//
	//		bool NToggle = true;
	//	};
	//
	//	static RenderData* s_RenderData;
	//
	//};

}