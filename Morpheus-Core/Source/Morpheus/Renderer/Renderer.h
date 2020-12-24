#pragma once

#include "Morpheus/Core/Common.h"
#include "RendererCore/RenderCommand.h"
#include "Morpheus/Mathematics/Mathematics.h"

#include "RendererResources/VertexBuffer.h"
#include "RendererResources/IndexBuffer.h"
#include "RendererResources/UniformBuffer.h"
#include "RendererResources/TextureBuffer.h"

#include "RendererBindables/FrameBuffer.h"
#include "RendererBindables/VertexArray.h"
#include "RendererBindables/Shader.h"

#include "Morpheus/Renderer/Camera.h"

// TODO: Fix spelling mistake (Destroy)
#define CraftSizeX 6
#define CraftSizeY 1

namespace Morpheus {

	class Renderer
	{
	public:	
		static void Init()
		{
			MORP_PROFILE_FUNCTION();

			RenderCommand::Init();
			s_RenderData = new RenderData;
	
			QuadVertex Data[4] =
			{
				{ { 0.25f, 0.25f }, { 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
				{ { -0.25f, 0.25f }, { 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
				{ { -0.25f, -0.25f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } },
				{ { 0.25f, -0.25f }, { 1.0f, 1.0f }, { 1.0f, 0.0f, 1.0f } }
			};
			Ref<VertexBuffer> VBO = VertexBuffer::Create(Data, sizeof(Data));

			uint32 IndexBufferData[6] = { 0, 1, 2, 0, 2, 3 };
			Ref<IndexBuffer> IBO = IndexBuffer::Create(IndexBufferData, sizeof(IndexBufferData));


			//RenderpassElement ColorElement;
			//ColorElement.Attachment = 0;
			//ColorElement.Layout = RenderpassAttachmentLayout::ATTACHMENT_COLOR_OPTIMAL;
			//ColorElement.Load = RenderpassLoad::ATTACHMENT_CLEAR;
			//ColorElement.Store = RenderpassStore::ATTACHMENT_STORE;
			//ColorElement.Initial = RenderpassImage::ATTACHMENT_UNDEFINED;
			//ColorElement.Final = RenderpassImage::ATTACHMENT_PRESENT;
			//
			//RenderpassLayout RenderpassLayout = { ColorElement };
			//s_RenderData->Renderpass = Renderpass::Create(RenderpassLayout);
			//s_RenderData->FrameBuffer = FrameBuffer::Create(s_RenderData->Renderpass);
			//
			//
			//ShaderAttributeLayout ShaderLayoutData = {
			//	{ ShaderAttributeType::Float2, "Position" },
			//	{ ShaderAttributeType::Float2, "TexCoord" },
			//	{ ShaderAttributeType::Float3, "Color" }
			//};
			//
			//s_RenderData->Shader = Shader::Create(s_RenderData->Renderpass, ShaderLayoutData, "Assets/uniform.vert.spv", "Assets/uniform.frag.spv");
			//
			//QuadVertex Data[4] =
			//{
			//	{ { 0.25f, 0.25f }, { 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
			//	{ { -0.25f, 0.25f }, { 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
			//	{ { -0.25f, -0.25f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } },
			//	{ { 0.25f, -0.25f }, { 1.0f, 1.0f }, { 1.0f, 0.0f, 1.0f } }
			//};
			//
			//uint32 IndexBufferData[6] = { 0, 1, 2, 0, 2, 3 };
			//
			//UniformAttributeLayout UniformLayoutData = {
			//	{ UniformAttributeType::Mat4, "ProjectionMatrix" },
			//	{ UniformAttributeType::Mat4, "ViewMatrix" },
			//	{ UniformAttributeType::Mat4, "TransformMatrix" }
			//};
			//
			//for (uint32 x = 0; x < CraftSizeX; x++) {
			//	Vector<Ref<VertexArray>> yArrays;
			//	Vector<Matrix4> mArrays;
			//	for (uint32 y = 0; y < CraftSizeY; y++)
			//	{
			//		Ref<VertexArray> VAO = VertexArray::Create();
			//		VAO->SetVertexBuffer(VertexBuffer::Create(Data, sizeof(Data)));
			//		VAO->SetIndexBuffer(IndexBuffer::Create(IndexBufferData, sizeof(IndexBufferData)));
			//		VAO->SetTextureBuffer(TextureBuffer::Create("Assets/Textures/texture.jpg"));
			//		VAO->SetUniformBuffer(UniformBuffer::Create(UniformLayoutData));
			//
			//		Matrix4 Mat = glm::translate(Vector3(x * 0.5f, y * 0.5f, -1.00f));
			//		mArrays.push_back(Mat);
			//		yArrays.push_back(VAO);
			//	}
			//	s_RenderData->VertexArrays.push_back(yArrays);
			//	s_RenderData->PositionMatrix.push_back(mArrays);
			//}
		}

		static void Shutdown()
		{
			MORP_PROFILE_FUNCTION();
			RenderCommand::Shutdown();
		}
	
		static void BeginScene(Camera& Camera)
		{
			MORP_PROFILE_FUNCTION();

			//RenderCommand::SetClearColor({ 0.15f, 0.15f, 0.15f, 1.0f });
			s_RenderData->ProjectionMatrix = Camera.GetProjectionMatrix();
			s_RenderData->ViewMatrix = Camera.GetViewMatrix();
		}
	
		static void EndScene()
		{
			MORP_PROFILE_FUNCTION();

			//s_RenderData->FrameBuffer->Bind();
			//
			//for (uint32 x = 0; x < CraftSizeX; x++) 
			//	for (uint32 y = 0; y < CraftSizeY; y++) {
			//		s_RenderData->Shader->Bind();
			//		s_RenderData->VertexArrays[x][y]->GetUniformBuffer()->SetMat4("ProjectionMatrix", s_RenderData->ProjectionMatrix);
			//		s_RenderData->VertexArrays[x][y]->GetUniformBuffer()->SetMat4("ViewMatrix", s_RenderData->ViewMatrix);
			//		s_RenderData->VertexArrays[x][y]->GetUniformBuffer()->SetMat4("TransformMatrix", s_RenderData->PositionMatrix[x][y]);
			//		s_RenderData->VertexArrays[x][y]->Bind();
			//		RenderCommand::DrawIndexed(s_RenderData->VertexArrays[x][y]);
			//	}
			//
			////s_RenderData->Image = s_RenderData->FrameBuffer->GetImage();
			//RenderCommand::Flush();
		}
	
	private:
		struct RenderData 
		{
		public:
			Vector<Vector<Ref<VertexArray>>> VertexArrays;
			Vector<Vector<Matrix4>> PositionMatrix;

			Ref<Renderpass> Renderpass;
			Ref<FrameBuffer> FrameBuffer;
			Ref<Shader> Shader;
	
			Matrix4 ProjectionMatrix = Matrix4(1.00f);
			Matrix4 ViewMatrix = Matrix4(1.00f);
			Matrix4 TransformMatrix = Matrix4(1.00f);

			float32 RotationValue = 0.00f;
		};
	
		static RenderData* s_RenderData;
	
	};

}