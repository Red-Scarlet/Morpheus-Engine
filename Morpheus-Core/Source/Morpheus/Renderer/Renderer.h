#pragma once

#include "Morpheus/Core/Common.h"
#include "RenderCommand.h"
#include "Morpheus/Mathematics/Mathematics.h"

namespace Morpheus {

	class Renderer
	{
	public:
		static void Init()
		{
			RenderCommand::Init();
		}

		static void Shutdown()
		{
			RenderCommand::Shutdown();
		}

		static void BeginScene()
		{
			RenderCommand::Begin();
		}

		static void EndScene()
		{
			RenderCommand::End();
			RenderCommand::Flush();
		}

		static void DrawVertexBuffer()
		{
		}

	};

}