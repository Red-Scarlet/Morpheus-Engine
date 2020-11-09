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
		ImGui::Begin("Viewport");
		ImGui::End();

		//VK IMAGE FROM FBO
		//ImGui::Image((void*)m_textureID, ImVec2{ 1280, 720}, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
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