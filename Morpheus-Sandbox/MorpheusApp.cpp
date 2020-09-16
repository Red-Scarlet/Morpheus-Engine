#define ENTRYPOINT
#include <Morpheus.h>

class ExampleLayer : public Morpheus::Layer
{
public:
	ExampleLayer()
		: Layer("ExampleLayer")
	{	
	}

	~ExampleLayer()
	{
	}

	virtual void OnAttach() override 
	{	
	}

	virtual void OnDetach() override 
	{
	}

	virtual void OnUpdate(const Morpheus::DeltaTime& _Delta) override
	{
	}

	virtual void OnRender() override 
	{
		Morpheus::Renderer::BeginScene();
		Morpheus::Renderer::DrawVertexBuffer();
		Morpheus::Renderer::EndScene();
	}

};

class SandboxApp : public Morpheus::Application
{
public:
	SandboxApp()
		: Morpheus::Application()
	{
		PushLayer(new ExampleLayer());
	}

	~SandboxApp()
	{
	}

};

Morpheus::Application* CreateApplication()
{
	return new SandboxApp();
}