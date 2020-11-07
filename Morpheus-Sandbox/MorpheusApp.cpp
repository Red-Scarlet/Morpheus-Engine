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
		//m_Cam = ;
	}

	virtual void OnRender() override 
	{		
		Morpheus::RenderCommand::SetClearColor({ 0.15f, 0.15f, 0.15f, 1.0f});

		Morpheus::Renderer2D::BeginScene(m_Cam);

		Morpheus::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.0f }, { 0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f });

		Morpheus::Renderer2D::EndScene();
	}

	virtual void OnEvent(Morpheus::Event& _Event) override
	{

	}


	virtual void OnImGuiRender() override
	{
		//ImGui::Begin("Viewport");

		//VK IMAGE FROM FBO
		//ImGui::Image((void*)m_textureID, ImVec2{ 1280, 720}, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		//ImGui::End();
	}

private:
	Morpheus::Camera m_Cam;

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