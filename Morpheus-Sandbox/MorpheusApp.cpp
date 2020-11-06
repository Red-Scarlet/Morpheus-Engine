#define ENTRYPOINT
#include <Morpheus.h>

#include <IMGUI/imgui.h>

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
		Morpheus::Renderer::EndScene();

	}

	virtual void OnEvent(Morpheus::Event& _Event) override
	{

	}


	virtual void OnImGuiRender() override
	{
		bool state = true;		

		ImGui::ShowDemoWindow(&state);
		//ImGui::Begin("Cunt");
		//ImGui::End();
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