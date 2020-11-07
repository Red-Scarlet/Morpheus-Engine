#include "Morppch.h"
#include "Renderer2D.h"

#include "Morpheus/Renderer/RendererCore/RenderCommand.h" 
#include "Morpheus/Renderer/RendererBindables/VertexArray.h"
#include "Morpheus/Renderer/RendererBindables/Shader.h"

#include "Morpheus/Renderer/RendererUnknown/FrameBuffer.h"
#include "Morpheus/Renderer/RendererUnknown/Renderpass.h"

//#include "Concealing/Core/Application.h"

namespace Morpheus {

	struct QuadVertex
	{
		Vector4 Position;
		Vector4 Color;
		//Vector2 TexCoord;
		//float32 TexIndex;
	};

	struct Renderer2DData
	{
		static const uint32 MaxQuads = 10000;
		static const uint32 MaxVertices = MaxQuads * 4;
		static const uint32 MaxIndices = MaxQuads * 6;
		static const uint32 MaxTextureSlots = 32;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> Shader;
		Ref<UniformBuffer> Uniform;

		Ref<FrameBuffer> FrameBuffer;
		Ref<Renderpass> Renderpass;

		uint32 QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		Vector4 QuadVertexPositions[4];

		Renderer2D::Statistics Stats;
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{ 
		
		RenderpassLayout RPLayout = {
			{ RenderpassTypes::ATTACHMENT_COLOR, RenderpassAttachment::ATTACHMENT_CLEAR, RenderpassAttachment::ATTACHMENT_STORE },
			{ RenderpassTypes::ATTACHMENT_DEPTH, RenderpassAttachment::ATTACHMENT_CLEAR, RenderpassAttachment::ATTACHMENT_DONTCARE }
		};

		s_Data.Renderpass = Renderpass::Create(RPLayout);
		s_Data.FrameBuffer = FrameBuffer::Create(s_Data.Renderpass);

		s_Data.QuadVertexArray = VertexArray::Create();

		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));

		s_Data.QuadVertexArray->SetVertexBuffer(s_Data.QuadVertexBuffer);
		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		uint32_t* _QuadIndices = new uint32_t[s_Data.MaxIndices];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6) {
			_QuadIndices[i + 0] = offset + 0;
			_QuadIndices[i + 1] = offset + 1;
			_QuadIndices[i + 2] = offset + 2;

			_QuadIndices[i + 3] = offset + 2;
			_QuadIndices[i + 4] = offset + 3;
			_QuadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> _QuadIndexBuffer = IndexBuffer::Create(_QuadIndices, s_Data.MaxIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(_QuadIndexBuffer);
		delete[] _QuadIndices;

		BufferLayout LayoutData = {
			{ UniformDataType::Mat4, "ProjectionMatrix", Matrix4(1.00f) },
			{ UniformDataType::Mat4, "ViewMatrix", Matrix4(1.00f) }
		};

		s_Data.Uniform = UniformBuffer::Create(LayoutData);
		s_Data.QuadVertexArray->SetUniformBuffer(s_Data.Uniform);

		s_Data.Shader = Shader::Create("Assets/BatchR2D.vert.spv", "Assets/BatchR2D.frag.spv");
		s_Data.Shader->Bind();

		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };

		ResetStats();
	}

	void Renderer2D::Shutdown()
	{
	}


	void Renderer2D::BeginScene(Camera& _Camera)
	{
		BufferLayout LayoutData = {
			{ UniformDataType::Mat4, "ProjectionMatrix",  _Camera.ProjectionMatrix },
			{ UniformDataType::Mat4, "ViewMatrix", _Camera.ViewMatrix }
		};
		s_Data.Uniform->SetLayout(LayoutData);
		
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
	}

	void Renderer2D::EndScene()
	{	
		uint32 _DataSize = (uint8*)s_Data.QuadVertexBufferPtr - (uint8*)s_Data.QuadVertexBufferBase;
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, _DataSize);
		// Compile VAO

		Flush();
	}

	void Renderer2D::Flush()
	{
		if (s_Data.QuadIndexCount == 0)
			return; // Nothing to draw

		RenderCommand::Flush();
		s_Data.Stats.DrawCalls++;
	}

	void Renderer2D::FlushAndReset()
	{
		EndScene();

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
	}

	void Renderer2D::DrawQuad(const Vector2& _Position, const Vector2& _Size, const Vector4& _Color)
	{
		DrawQuad({ _Position.x, _Position.y, 0.0f }, _Size, _Color);
	}

	void Renderer2D::DrawQuad(const Vector3& _Position, const Vector2& _Size, const Vector4& _Color)
	{
		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		Matrix4 transform = Matrix4::Translate(_Position);
		transform *= Matrix4::Scale(Vector3(_Size.x, _Size.y, 1.0f));
		float32 TextureIndex = 0.0f;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
		s_Data.QuadVertexBufferPtr->Color = Vector4(_Color.x, _Color.y, _Color.z, _Color.w);
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
		s_Data.QuadVertexBufferPtr->Color = Vector4(_Color.x, _Color.y, _Color.z, _Color.w);
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
		s_Data.QuadVertexBufferPtr->Color = Vector4(_Color.x, _Color.y, _Color.z, _Color.w);
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
		s_Data.QuadVertexBufferPtr->Color = Vector4(_Color.x, _Color.y, _Color.z, _Color.w);
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}

}