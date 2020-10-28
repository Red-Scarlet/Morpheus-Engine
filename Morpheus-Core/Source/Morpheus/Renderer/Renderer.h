#pragma once

#include "Morpheus/Core/Common.h"
#include "RendererCore/RenderCommand.h"
#include "Morpheus/Mathematics/Mathematics.h"

#include "RendererResources/VertexBuffer.h"
#include "RendererResources/IndexBuffer.h"
#include "RendererResources/UniformBuffer.h"

#include "RendererResources/GraphicsPipeline.h"

#include "RendererUnknown/FrameBuffer.h"
#include "RendererUnknown/Renderpass.h"

#include "RendererBindables/VertexArray.h"
#include "RendererBindables/Shader.h"

namespace Morpheus {

	class Renderer
	{
	public:
		static void CreateGeomerty()
		{
			float32 Offset_1 = 0.0f;
			VertexData Data[3] =
			{
				{ { 0.25f - Offset_1,  0.25f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
				{ { -0.25f - Offset_1,  0.25f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
				{ { 0.0f - Offset_1, -0.25f, 0.0f }, { 0.0f, 0.0f, 1.0f } }
			};		

			uint32 IndexBufferData[3] = { 0, 1, 2 };

			BufferLayout LayoutData = {
				{ UniformDataType::Mat4, "ProjectionMatrix",  s_RenderData->ProjectionMatrix },
				{ UniformDataType::Mat4, "ViewMatrix",  s_RenderData->ViewMatrix },
				{ UniformDataType::Mat4, "TransformMatrix",  s_RenderData->TransformMatrix },
				{ UniformDataType::Float, "BreakValue", s_RenderData->BreakValue }
			};

			s_RenderData->VertexArray = VertexArray::Create();
			s_RenderData->VertexArray->SetVertexBuffer(VertexBuffer::Create(Data, sizeof(Data) / sizeof(VertexData)));
			s_RenderData->VertexArray->SetIndexBuffer(IndexBuffer::Create(IndexBufferData, sizeof(IndexBufferData) / sizeof(uint32)));
			s_RenderData->VertexArray->SetUniformBuffer(UniformBuffer::Create(LayoutData));

			//s_RenderData->VertexArray->Bind();
		}

		static void UpdateBasicCamera()
		{
			s_RenderData->TransformMatrix = Matrix4::Rotation(s_RenderData->RotationValue, Vector3(0.0f, 1.0f, 0.0f));
		}

		static void Init()
		{
			RenderCommand::Init();
			s_RenderData = new RenderData;

			s_RenderData->Renderpass = Renderpass::Create();
			s_RenderData->FrameBuffer = FrameBuffer::Create();
			float32 zoom = -2.5f;
			s_RenderData->ProjectionMatrix = Matrix4::Perspective(45.0f, (float32)1280.0f / (float32)720.0f, 0.01f, 1024.0f);;
			s_RenderData->ViewMatrix = Matrix4::Translation(Vector3(0.0f, 0.0f, zoom));
			s_RenderData->TransformMatrix = Matrix4(1.00f);

			UpdateBasicCamera();	
			s_RenderData->Shader = Shader::Create("Assets/uniform.vert.spv", "Assets/uniform.frag.spv");
			s_RenderData->Pipeline = GraphicsPipeline::Create();

			CreateGeomerty();
			s_RenderData->Shader->Bind();

			RenderCommand::SetCompile(true);
			RenderCommand::SetReady(true);
		}

		static void Shutdown()
		{
			RenderCommand::Shutdown();
		}

		static void BeginScene()
		{
			// Loops
			s_RenderData->BreakValue += 0.00005f;
			s_RenderData->RotationValue += 0.05;

			UpdateBasicCamera();

			// Update Uniform Buffer (Update Shader)!
			BufferLayout Layout = {
				{ UniformDataType::Mat4, "ProjectionMatrix",  s_RenderData->ProjectionMatrix },
				{ UniformDataType::Mat4, "ViewMatrix",  s_RenderData->ViewMatrix },
				{ UniformDataType::Mat4, "TransformMatrix",  s_RenderData->TransformMatrix },
				{ UniformDataType::Float, "BreakValue", s_RenderData->BreakValue }
			};

			//s_RenderData->UniformBuffer1->SetLayout(Layout_1);
		}

		static void EndScene()
		{
			// Loops

			RenderCommand::Flush();
		}
	
	private:
		struct RenderData 
		{
		public:
			Ref<VertexArray> VertexArray;

			Ref<FrameBuffer> FrameBuffer;
			Ref<Renderpass> Renderpass;
			Ref<Shader> Shader;
			Ref<GraphicsPipeline> Pipeline;

			Matrix4 ProjectionMatrix;
			Matrix4 ViewMatrix;
			Matrix4 TransformMatrix;
			float32 BreakValue = 1.25f;
			float32 RotationValue = 0.00f;
		};

		static RenderData* s_RenderData;

	};

}