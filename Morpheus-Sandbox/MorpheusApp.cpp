#define ENTRYPOINT
#include <Morpheus.h>

#include <IMGUI/imgui.h>

class ExampleLayer : public Morpheus::Layer
{
public:
	ExampleLayer()
		: Layer("ExampleLayer"), m_Camera(90.00f, (1280.00f / 720.00f))
	{	
		m_CameraSpeed = 4.8f;
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
		float CurrentSpeed = m_CameraSpeed * _Delta;

		if (Morpheus::Input::IsKeyPressed(MORP_KEY_W))
			m_CameraPos += CurrentSpeed * m_CameraFront;
		else if (Morpheus::Input::IsKeyPressed(MORP_KEY_S))
			m_CameraPos -= CurrentSpeed * m_CameraFront;

		if (Morpheus::Input::IsKeyPressed(MORP_KEY_A))
			m_CameraPos -= Morpheus::Vector3::Normalize(Morpheus::Vector3::Cross(m_CameraFront, m_CameraUp)) * CurrentSpeed;
		else if (Morpheus::Input::IsKeyPressed(MORP_KEY_D))
			m_CameraPos += Morpheus::Vector3::Normalize(Morpheus::Vector3::Cross(m_CameraFront, m_CameraUp)) * CurrentSpeed;

		m_Camera.SetPosition(m_CameraPos);
		m_Camera.SetView(m_CameraPos, m_CameraFront, m_CameraUp);

		m_CurrentDeltaTime = _Delta;
	}

	virtual void OnRender() override 
	{		
		Morpheus::Renderer::BeginScene(m_Camera);
		Morpheus::Renderer::EndScene();
	}

	virtual void OnEvent(Morpheus::Event& _Event) override
	{
		Morpheus::EventDispatcher dispatcher(_Event);
		dispatcher.Dispatch<Morpheus::MouseMovedEvent>(MORP_BIND_EVENT_FN(ExampleLayer::OnMouseMove));
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("DebugUtils");
		ImGui::Text("CPU Frametime: %f ms", m_CurrentDeltaTime);
		ImGui::Text("CPU Framerate: %f fps", 1.00f / m_CurrentDeltaTime);
		ImGui::End();

		//VK IMAGE FROM FBO
		//ImGui::Image((void*)m_textureID, ImVec2{ 1280, 720}, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	}


	bool OnMouseMove(Morpheus::MouseMovedEvent& _Event)
	{
		float PositionX = _Event.GetX();
		float PositionY = _Event.GetY();
		if (m_FirstMouse) {
			m_LastX = PositionX;
			m_LastY = PositionY;
			m_FirstMouse = false;
		}

		float xoffset = PositionX - m_LastX;
		float yoffset = m_LastY - PositionY;
		m_LastX = PositionX;
		m_LastY = PositionY;

		float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		m_Yaw += xoffset;
		m_Pitch += yoffset;

		if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;
		if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;

		Morpheus::Vector3 front;
		front.x = cos(Morpheus::ToRadians(m_Yaw)) * cos(Morpheus::ToRadians(m_Pitch));
		front.y = -sin(Morpheus::ToRadians(m_Pitch));
		front.z = sin(Morpheus::ToRadians(m_Yaw)) * cos(Morpheus::ToRadians(m_Pitch));

		m_CameraFront = Morpheus::Vector3::Normalize(front);
		
		return false;
	}

private:
	Morpheus::PerspectiveCamera m_Camera;
	Morpheus::Vector3 m_CameraPos = { 0.0f, 0.0f, 0.0f };
	Morpheus::Vector3 m_CameraFront = { 0.0f, 0.0f, 1.0f };
	Morpheus::Vector3 m_CameraUp = { 0.0f, 1.0f, 0.0f };

	bool m_FirstMouse = true;
	float m_CameraSpeed = 0.0f;
	float m_Yaw = -90.0f;
	float m_Pitch = 0.0f;
	float m_LastX = 1280.0f / 2.0;
	float m_LastY = 720.0f / 2.0;

	float m_CurrentDeltaTime = 0;
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