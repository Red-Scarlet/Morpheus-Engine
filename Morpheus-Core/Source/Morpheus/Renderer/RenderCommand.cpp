#include "Morppch.h"
#include "RenderCommand.h"

namespace Morpheus {

	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();

}