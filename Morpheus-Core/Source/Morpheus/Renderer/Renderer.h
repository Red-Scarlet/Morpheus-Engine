#pragma once

#include "Morpheus/Core/Common.h"
#include "RendererCore/RenderCommand.h"
#include "Morpheus/Mathematics/Mathematics.h"

#include "RendererResources/VertexBuffer.h"
#include "RendererResources/IndexBuffer.h"
#include "RendererResources/TextureBuffer.h"

#include "RendererBindables/FrameBuffer.h"
#include "RendererBindables/VertexArray.h"
#include "RendererBindables/Shader.h"


#include "Morpheus/Renderer/Camera.h"

// TODO: Fix spelling mistake (Destroy)

namespace Morpheus {

	class Renderer
	{
	public:	
		static void Init()
		{
			RenderCommand::Init();
			s_RenderData = new RenderData;
	
			RenderpassLayout RenderpassLayout = {
				{ RenderpassTypes::ATTACHMENT_COLOR, RenderpassAttachment::ATTACHMENT_CLEAR, RenderpassAttachment::ATTACHMENT_STORE }
			};
			s_RenderData->FrameBuffer = FrameBuffer::Create(RenderpassLayout);

			ShaderAttributeLayout ShaderLayoutData = {
				{ ShaderAttributeType::Float2, "Position" },
				{ ShaderAttributeType::Float3, "Color" }
			};

			ShaderAttributeLayout UniformLayoutData = {
				{ ShaderAttributeType::Mat4, "ProjectionMatrix" },
				{ ShaderAttributeType::Mat4, "ViewMatrix" },
				{ ShaderAttributeType::Mat4, "TransformMatrix" }
			};

			s_RenderData->Shader = Shader::Create(ShaderLayoutData, "Assets/uniform.vert.spv", "Assets/uniform.frag.spv");
			s_RenderData->Shader->SetUniformDescription(UniformLayoutData);

			QuadVertex Data[4] =
			{
				{ { 0.25f, 0.25f }, { 1.0f, 0.0f, 0.0f } },
				{ { -0.25f, 0.25f }, { 0.0f, 1.0f, 0.0f } },
				{ { -0.25f, -0.25f }, { 0.0f, 0.0f, 1.0f } },
				{ { 0.25f, -0.25f }, { 1.0f, 0.0f, 1.0f } }
			};

			uint32 IndexBufferData[6] = { 0, 1, 2, 0, 2, 3 };

			uint32 CraftSize = 1;

			s_RenderData->VertexArrays.resize(CraftSize);
			for (uint32 i = 0; i < s_RenderData->VertexArrays.size(); i++)
			{
				s_RenderData->VertexArrays[i] = VertexArray::Create();
				s_RenderData->VertexArrays[i]->SetVertexBuffer(VertexBuffer::Create(Data, sizeof(Data) / sizeof(QuadVertex)));
				s_RenderData->VertexArrays[i]->SetIndexBuffer(IndexBuffer::Create(IndexBufferData, sizeof(IndexBufferData) / sizeof(uint32)));
			}

			s_RenderData->TextureBuffer = TextureBuffer::Create("Assets/Textures/texture.jpg");

			s_RenderData->PositionMatrixs.resize(CraftSize);
			for (uint32 i = 0; i < s_RenderData->PositionMatrixs.size(); i++)
				s_RenderData->PositionMatrixs[i] = Matrix4::Translate(Vector3(i * 0.5f, 0.00f, 0.00f));
		}
	
		static void Shutdown()
		{
			RenderCommand::Shutdown();
		}
	
		static void BeginScene(Camera& Camera)
		{
			RenderCommand::SetClearColor({ 0.15f, 0.15f, 0.15f, 1.0f });

			s_RenderData->RotationValue += 0.05;

			s_RenderData->ProjectionMatrix = Camera.GetProjectionMatrix();
			s_RenderData->ViewMatrix = Camera.GetViewMatrix();
		}
	
		static void EndScene()
		{
			s_RenderData->FrameBuffer->Bind();

			for (uint32 i = 0; i < s_RenderData->VertexArrays.size(); i++)
			{
				s_RenderData->Shader->Bind();
				s_RenderData->Shader->SetMat4("ProjectionMatrix", s_RenderData->ProjectionMatrix);
				s_RenderData->Shader->SetMat4("ViewMatrix", s_RenderData->ViewMatrix);
				s_RenderData->Shader->SetMat4("TransformMatrix", s_RenderData->PositionMatrixs[i]);
				s_RenderData->VertexArrays[i]->Bind();
				RenderCommand::DrawIndexed(s_RenderData->VertexArrays[i]);
			}

			RenderCommand::Flush();
		}
	
	private:
		struct RenderData 
		{
		public:
			Vector<Ref<VertexArray>> VertexArrays;
			Vector<Matrix4> PositionMatrixs;

			Ref<TextureBuffer> TextureBuffer;

			Ref<FrameBuffer> FrameBuffer;
			Ref<Shader> Shader;
	
			Matrix4 ProjectionMatrix = Matrix4(1.00f);
			Matrix4 ViewMatrix = Matrix4(1.00f);
			Matrix4 TransformMatrix = Matrix4(1.00f);

			float32 RotationValue = 0.00f;
	
			bool NToggle = true;
		};
	
		static RenderData* s_RenderData;
	
	};

}